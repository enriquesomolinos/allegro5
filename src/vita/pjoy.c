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
 *      Joystick driver routines for VITA.
 *
 *      By enrique somolinos.
 *
 *      See readme.txt for copyright information.
 */

#include "allegro.h"
#include "allegro/internal/aintern.h"

#ifndef ALLEGRO_VITA
#error something is wrong with the makefile
#endif


static int vita_joy_init(void);
static void vita_joy_exit(void);
static int vita_joy_poll(void);


JOYSTICK_DRIVER joystick_vita = {
   JOYSTICK_VITA,         // int  id;
   empty_string,         // AL_CONST char *name;
   empty_string,         // AL_CONST char *desc;
   "VITA Controller",     // AL_CONST char *ascii_name;
   vita_joy_init,         // AL_METHOD(int, init, (void));
   vita_joy_exit,         // AL_METHOD(void, exit, (void));
   vita_joy_poll,         // AL_METHOD(int, poll, (void));
   NULL,                 // AL_METHOD(int, save_data, (void));
   NULL,                 // AL_METHOD(int, load_data, (void));
   NULL,                 // AL_METHOD(AL_CONST char *, calibrate_name, (int n));
   NULL                  // AL_METHOD(int, calibrate, (int n));
};



/* vita_joy_init:
 *  Initializes the vita joystick driver.
 */
static int vita_joy_init(void)
{
}



/* vita_joy_exit:
 *  Shuts down the vita joystick driver.
 */
static void vita_joy_exit(void)
{
}



/* vita_joy_poll:
 *  Polls the active joystick devices and updates internal states.
 */
static int vita_joy_poll(void)
{
}
