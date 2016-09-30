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
 *      vita mini-keyboard driver using the vita controller.
 *      TODO: Maybe implement it using the OSK.
 *
 *      By enrique somolinos.
 *
 *      See readme.txt for copyright information.
 */


#include "allegro.h"
#include "allegro/internal/aintern.h"
#include <psp2/ctrl.h>

#ifndef ALLEGRO_VITA
#error Something is wrong with the makefile
#endif

#define PREFIX_I                "al-pkey INFO: "
#define PREFIX_W                "al-pkey WARNING: "
#define PREFIX_E                "al-pkey ERROR: "

#define SAMPLING_CYCLE 0
#define SAMPLING_MODE  SCE_CTRL_MODE_DIGITAL


static int vita_keyboard_init(void);
static void vita_keyboard_exit(void);
static void vita_poll_keyboard(void);

/*
 * Lookup table for converting VITA_CTRL_* codes into Allegro KEY_* codes
 * TODO: Choose an alternative mapping?
 */
static const int vita_to_scancode[][2] = {
   { SCE_CTRL_SELECT,     KEY_ESC   },
   { SCE_CTRL_START,      KEY_ENTER },
   { SCE_CTRL_UP,         KEY_UP    },
   { SCE_CTRL_RIGHT,      KEY_RIGHT },
   { SCE_CTRL_DOWN,       KEY_DOWN  },
   { SCE_CTRL_LEFT,       KEY_LEFT  },
   { SCE_CTRL_TRIANGLE,   KEY_SPACE },
   { SCE_CTRL_CIRCLE,     KEY_SPACE },
   { SCE_CTRL_CROSS,      KEY_SPACE },
   { SCE_CTRL_SQUARE,     KEY_SPACE },
   { SCE_CTRL_LTRIGGER,     KEY_SPACE },
   { SCE_CTRL_RTRIGGER,     KEY_SPACE }
};

#define NKEYS (sizeof vita_to_scancode / sizeof vita_to_scancode[0])


/* The last polled input. */
static SceCtrlData old_pad = {0, 0, 0, 0};


KEYBOARD_DRIVER keybd_simulator_vita =
{
   KEYSIM_VITA,
   empty_string,
   empty_string,
   "PSP keyboard simulator",
   FALSE,  // int autorepeat;
   vita_keyboard_init,
   vita_keyboard_exit,
   vita_poll_keyboard,
   NULL,   // AL_METHOD(void, set_leds, (int leds));
   NULL,   // AL_METHOD(void, set_rate, (int delay, int rate));
   NULL,   // AL_METHOD(void, wait_for_input, (void));
   NULL,   // AL_METHOD(void, stop_waiting_for_input, (void));
   NULL,   // AL_METHOD(int,  scancode_to_ascii, (int scancode));
   NULL    // scancode_to_name
};



/* psp_keyboard_init:
 *  Installs the keyboard handler.
 */
static int vita_keyboard_init(void)
{
   //sceCtrlSetSamplingCycle(SAMPLING_CYCLE);
   sceCtrlSetSamplingMode(SAMPLING_MODE);
   TRACE(PREFIX_I "VITA keyboard installed\n");

   /* TODO: Maybe write a keyboard "interrupt" handler using a dedicated thread
    * that polls the VITA controller periodically. */

   return 0;
}


/* vita_keyboard_exit:
 *  Removes the keyboard handler.
 */
static void vita_keyboard_exit(void)
{
}


/* vita_poll_keyboard:
 *  Polls the VITA "mini-keyboard".
 */
static void vita_poll_keyboard(void)
{
   SceCtrlData pad;
   int buffers_to_read = 1;
   int i, changed;

   sceCtrlPeekBufferPositive(0,&pad, buffers_to_read);

   for (i = 0; i < NKEYS; i++) {
      changed = (pad.buttons ^ old_pad.buttons) & vita_to_scancode[i][0];
      if (changed) {
         if (pad.buttons & vita_to_scancode[i][0]) {
            TRACE(PREFIX_I "VITA Keyboard: [%d] pressed\n", vita_to_scancode[i][1]);
            _handle_key_press(0, vita_to_scancode[i][1]);
         }
         else {
            TRACE(PREFIX_I "VITA Keyboard: [%d] released\n", vita_to_scancode[i][1]);
            _handle_key_release(vita_to_scancode[i][1]);
         }
      }
   }

   old_pad = pad;
}
