/*         ______   ___    ___ 
 *        /\  _  \ /\_ \  /\_ \ 
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Linux joystick driver.
 *
 *      By George Foot and Peter Wang.
 *
 *      Updated for new joystick API by Peter Wang.
 *
 *      See readme.txt for copyright information.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#define ALLEGRO_NO_KEY_DEFINES
#define ALLEGRO_NO_COMPATIBILITY

#include "allegro.h"
#include "allegro/internal/aintern.h"
#include ALLEGRO_INTERNAL_HEADER
#include "allegro/internal/aintern2.h"

#ifdef HAVE_LINUX_JOYSTICK_H

#include <linux/joystick.h>



#define TOTAL_JOYSTICK_AXES  (_AL_MAX_JOYSTICK_STICKS * _AL_MAX_JOYSTICK_AXES)



/* map a Linux joystick axis number to an Allegro (stick,axis) pair */
typedef struct {
   int stick, axis;
} AXIS_MAPPING;


typedef struct AL_JOYSTICK_LINUX
{
   AL_JOYSTICK parent;
   int fd;
   AXIS_MAPPING axis_mapping[TOTAL_JOYSTICK_AXES];
   AL_JOYSTATE joystate;
} AL_JOYSTICK_LINUX;



/* forward declarations */
static bool ljoy_init(void);
static void ljoy_exit(void);
static int ljoy_num_joysticks(void);
static AL_JOYSTICK *ljoy_request_joystick(int num);
static void ljoy_release_joystick(AL_JOYSTICK *joy_);
static void ljoy_get_state(AL_JOYSTICK *joy_, AL_JOYSTATE *ret_state);

static void ljoy_process_new_data(void *data);
static void ljoy_generate_axis_event(AL_JOYSTICK_LINUX *joy, int stick, int axis, int pos, int d);
static void ljoy_generate_button_event(AL_JOYSTICK_LINUX *joy, int button, unsigned int event_type);



/* the driver vtable */
AL_JOYSTICK_DRIVER _al_joydrv_linux_analogue =
{
   AL_JOY_TYPE_LINUX_ANALOGUE,
   empty_string,
   empty_string,
   "Linux analogue joystick(s)",
   ljoy_init,
   ljoy_exit,
   ljoy_num_joysticks,
   ljoy_request_joystick,
   ljoy_release_joystick,
   ljoy_get_state
};


/* set once when the joystick is initialised */
static int num_joysticks;



/* check_js_api_version: [primary thread]
 *
 *  Return true if the joystick API used by the device is supported by
 *  this driver.
 */
static bool check_js_api_version(int fd)
{
   unsigned int raw_version;

   if (ioctl(fd, JSIOCGVERSION, &raw_version) < 0) {
      /* NOTE: IOCTL fails if the joystick API is version 0.x */
      uszprintf(allegro_error, ALLEGRO_ERROR_SIZE, get_config_text("Your Linux joystick API is version 0.x which is unsupported."));
      return false;
   }

   /*
   struct { unsigned char build, minor, major; } version;

   version.major = (raw_version & 0xFF0000) >> 16;
   version.minor = (raw_version & 0xFF00) >> 8;
   version.build = (raw_version & 0xFF);
   */

   return true;
}



/* try_open_joy_device: [primary thread]
 *
 *  Try to open joystick device number NUM, returning the fd on success
 *  or -1 on failure.
 */
static int try_open_joy_device(int num)
{
   AL_CONST char *device_name = NULL;
   char tmp[128], tmp1[128], tmp2[128];
   int fd;

   /* Check for a user override on the device to use. */
   uszprintf(tmp, sizeof(tmp), uconvert_ascii("joystick_device_%d", tmp1), num);
   device_name = get_config_string(uconvert_ascii("joystick", tmp1), tmp, NULL);

   /* Special case for the first joystick. */
   if (!device_name && (num == 0))
      device_name = get_config_string(uconvert_ascii("joystick", tmp1),
                                      uconvert_ascii("joystick_device", tmp2),
                                      NULL);

   if (device_name)
      fd = open(uconvert_toascii(device_name, tmp), O_RDONLY|O_NONBLOCK);
   else {
      snprintf(tmp, sizeof(tmp), "/dev/input/js%d", num);
      tmp[sizeof(tmp)-1] = 0;

      fd = open(tmp, O_RDONLY|O_NONBLOCK);
      if (fd == -1) {
         snprintf(tmp, sizeof(tmp), "/dev/js%d", num);
         tmp[sizeof(tmp)-1] = 0;

         fd = open(tmp, O_RDONLY|O_NONBLOCK);
      }
   }

   if (!check_js_api_version(fd)) {
      close(fd);
      return -1;
   }

   return fd;   
}



/* count_num_joysticks: [primary thread]
 *
 *  Count the number of joysticks on the system.  It just tries to
 *  open joystick devices in turn, stopping as soon as it fails.  I
 *  don't know if that is the correct behaviour.
 */
static int count_num_joysticks(void)
{
   int i, fd;

   for (i = 0; ; i++) {
      fd = try_open_joy_device(i);
      if (fd == -1)
         return i;
      close(fd);
   }
}



/* ljoy_init: [primary thread]
 *  Initialise the joystick driver.
 */
static bool ljoy_init(void)
{
   /* cache the number of joysticks on the system */
   num_joysticks = count_num_joysticks();

   return true;
}



/* ljoy_exit: [primary thread]
 *  Shut down the joystick driver.
 */
static void ljoy_exit(void)
{
   num_joysticks = 0; /* not really necessary */
}



/* ljoy_num_joysticks: [primary thread]
 *
 *  Return the number of joysticks available on the system.
 */
static int ljoy_num_joysticks(void)
{
   return num_joysticks;
}



/* ljoy_request_joystick: [primary thread]
 *
 *  Returns the address of a AL_JOYSTICK structure for the device
 *  number NUM.  The top-level joystick functions will not call this
 *  function if joystick number NUM was already requested.  If the
 *  device cannot be opened, NULL is returned.
 */
static AL_JOYSTICK *ljoy_request_joystick(int num)
{
   AL_JOYSTICK_LINUX *joy;
   int fd;

   /* Try to open the device. */
   fd = try_open_joy_device(num);
   if (fd == -1)
      return NULL;

   /* Allocate a structure for the joystick. */
   joy = calloc(1, sizeof *joy);
   if (!joy) {
      close(fd);
      return NULL;
   }

   /* Initialise the event source part of it. */
   _al_event_source_init(&joy->parent.es, _AL_ALL_JOYSTICK_EVENTS, sizeof(AL_JOYSTICK_EVENT));

   /* Fill in the joystick information fields. */
   {
      char tmp[128], tmp1[128], tmp2[128];
      char num_axes;
      char num_buttons;
      int throttle;
      int s, a, b;

      ioctl(fd, JSIOCGAXES, &num_axes);
      ioctl(fd, JSIOCGBUTTONS, &num_buttons);

      if (num_axes > TOTAL_JOYSTICK_AXES)
         num_axes = TOTAL_JOYSTICK_AXES;

      if (num_buttons > _AL_MAX_JOYSTICK_BUTTONS)
         num_buttons = _AL_MAX_JOYSTICK_BUTTONS;

      /* User is allowed to override our simple assumption of which
       * axis number (kernel) the throttle is located at. */
      uszprintf(tmp, sizeof(tmp), uconvert_ascii("throttle_axis_%d", tmp1), num);
      throttle = get_config_int(uconvert_ascii("joystick", tmp1), tmp, -1);
      if (throttle == -1) {
         throttle = get_config_int(uconvert_ascii("joystick", tmp1), 
                                   uconvert_ascii("throttle_axis", tmp2), -1);
      }

      /* Each pair of axes is assumed to make up a stick unless it 
       * is the sole remaining axis, or has been user specified, in 
       * which case it is a throttle. */

      for (s = 0, a = 0;
           s < _AL_MAX_JOYSTICK_STICKS && a < num_axes;
           s++)
      {
         if ((a == throttle) || (a == num_axes-1)) {
            /* One axis throttle. */
            joy->parent.info.stick[s].flags = AL_JOYFLAG_ANALOGUE;
            joy->parent.info.stick[s].num_axes = 1;
            joy->parent.info.stick[s].axis[0].name = get_config_text("Throttle");
            joy->parent.info.stick[s].name = ustrdup(joy->parent.info.stick[s].axis[0].name);
            joy->axis_mapping[a].stick = s;
            joy->axis_mapping[a].axis = 0;
            a++;
         }
         else {
            /* Two axis stick. */
            joy->parent.info.stick[s].flags = AL_JOYFLAG_ANALOGUE;
            joy->parent.info.stick[s].num_axes = 2;
            joy->parent.info.stick[s].axis[0].name = get_config_text("X");
            joy->parent.info.stick[s].axis[1].name = get_config_text("Y");
            joy->parent.info.stick[s].name = malloc (32);
            uszprintf((char *)joy->parent.info.stick[s].name, 32, get_config_text("Stick %d"), s+1);
            joy->axis_mapping[a].stick = s;
            joy->axis_mapping[a].axis = 0;
            a++;
            joy->axis_mapping[a].stick = s;
            joy->axis_mapping[a].axis = 1;
            a++;
         }
      }

      joy->parent.info.num_sticks = s;

      /* Do the buttons. */

      for (b = 0; b < num_buttons; b++) {
         joy->parent.info.button[b].name = malloc (32);
         uszprintf((char *)joy->parent.info.button[b].name, 32, get_config_text("B%d"), b+1);
      }

      joy->parent.info.num_buttons = num_buttons;
   }

   joy->parent.num = num;

   joy->fd = fd;

   /* Register the joystick with the fdwatch subsystem.  */
   _al_unix_start_watching_fd(joy->fd, ljoy_process_new_data, joy);

   return (AL_JOYSTICK *) joy;
}



/* ljoy_release_joystick: [primary thread]
 *
 *  Close the device for a joystick then free the joystick structure.
 */
static void ljoy_release_joystick(AL_JOYSTICK *joy_)
{
   AL_JOYSTICK_LINUX *joy = (AL_JOYSTICK_LINUX *) joy_;
   int i;
   
   _al_unix_stop_watching_fd(joy->fd);

   _al_event_source_free(&joy->parent.es);
   close(joy->fd);
   for (i = 0; i < joy->parent.info.num_sticks; i++)
      free((void *)joy->parent.info.stick[i].name);
   for (i = 0; i < joy->parent.info.num_buttons; i++)
      free((void *)joy->parent.info.button[i].name);
   free(joy);
}



/* ljoy_get_state: [primary thread]
 *
 *  Copy the internal joystick state to a user-provided structure.
 */
static void ljoy_get_state(AL_JOYSTICK *joy_, AL_JOYSTATE *ret_state)
{
   AL_JOYSTICK_LINUX *joy = (AL_JOYSTICK_LINUX *) joy_;

   _al_event_source_lock(&joy->parent.es);
   {
      *ret_state = joy->joystate;
   }
   _al_event_source_unlock(&joy->parent.es);
}



/* ljoy_process_new_data: [fdwatch thread]
 *
 *  Process new data arriving in the joystick's fd.
 */
static void ljoy_process_new_data(void *data)
{
   AL_JOYSTICK_LINUX *joy = data;

   _al_event_source_lock(&joy->parent.es);
   {
      struct js_event js_events[32];
      int bytes, nr, i;

      while ((bytes = read(joy->fd, &js_events, sizeof js_events)) > 0) {

         nr = bytes / sizeof(struct js_event);

         for (i = 0; i < nr; i++) {

            int type   = js_events[i].type;
            int number = js_events[i].number;
            int value  = js_events[i].value;

            if (type & JS_EVENT_BUTTON) {
               if (number < _AL_MAX_JOYSTICK_BUTTONS) {
                  if (value)
                     joy->joystate.button[number] = 32767;
                  else
                     joy->joystate.button[number] = 0;

                  ljoy_generate_button_event(joy, number,
                                             (value
                                              ? AL_EVENT_JOYSTICK_BUTTON_DOWN
                                              : AL_EVENT_JOYSTICK_BUTTON_UP));
               }
            }
            else if (type & JS_EVENT_AXIS) {
               if (number < TOTAL_JOYSTICK_AXES) {
                  int stick = joy->axis_mapping[number].stick;
                  int axis  = joy->axis_mapping[number].axis;
                  int d;

                  if (value < -16384)
                     d = -1; 
                  else if (value > +16384)
                     d = +1; 
                  else
                     d = 0;

                  joy->joystate.stick[stick].axis[axis].pos = value;
                  joy->joystate.stick[stick].axis[axis].d = d;

                  ljoy_generate_axis_event(joy, stick, axis, value, d);
               }
            }
         }
      }
   }
   _al_event_source_unlock(&joy->parent.es);
}



/* ljoy_generate_axis_event: [fdwatch thread]
 *
 *  Helper to generate an event after an axis is moved.
 *  The joystick must be locked BEFORE entering this function.
 */
static void ljoy_generate_axis_event(AL_JOYSTICK_LINUX *joy, int stick, int axis, int pos, int d)
{
   AL_EVENT *event;

   if (!_al_event_source_needs_to_generate_event(&joy->parent.es, AL_EVENT_JOYSTICK_AXIS))
      return;

   event = _al_event_source_get_unused_event(&joy->parent.es);
   if (!event)
      return;

   event->joystick.type = AL_EVENT_JOYSTICK_AXIS;
   event->joystick.timestamp = al_current_time();
   event->joystick.stick = stick;
   event->joystick.axis = axis;
   event->joystick.pos = pos;
   event->joystick.d = d;
   event->joystick.button = 0;

   _al_event_source_emit_event(&joy->parent.es, event);
}



/* ljoy_generate_button_event: [fdwatch thread]
 *
 *  Helper to generate an event after a button is pressed or released.
 *  The joystick must be locked BEFORE entering this function.
 */
static void ljoy_generate_button_event(AL_JOYSTICK_LINUX *joy, int button, unsigned int event_type)
{
   AL_EVENT *event;

   if (!_al_event_source_needs_to_generate_event(&joy->parent.es, event_type))
      return;

   event = _al_event_source_get_unused_event(&joy->parent.es);
   if (!event)
      return;

   event->joystick.type = event_type;
   event->joystick.timestamp = al_current_time();
   event->joystick.stick = 0;
   event->joystick.axis = 0;
   event->joystick.pos = 0;
   event->joystick.d = 0;
   event->joystick.button = button;

   _al_event_source_emit_event(&joy->parent.es, event);
}

#endif /* HAVE_LINUX_JOYSTICK_H */



/* list the available drivers */
_DRIVER_INFO _al_linux_joystick_driver_list[] =
{
#ifdef HAVE_LINUX_JOYSTICK_H
   { AL_JOY_TYPE_LINUX_ANALOGUE,  &_al_joydrv_linux_analogue,  TRUE  },
#endif
   { 0,                           NULL,                        FALSE }
};



/*
 * Local Variables:
 * c-basic-offset: 3
 * indent-tabs-mode: nil
 * End:
 */
