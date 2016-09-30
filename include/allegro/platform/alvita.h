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
 *      VITA specific header defines.
 *
 *      By enrique somolinos.
 *
 *      See readme.txt for copyright information.
 */


#ifndef ALVITA_H
#define ALVITA_H

#ifndef ALLEGRO_VITA
   #error bad include
#endif

#include <stdio.h>

/* System driver */
#define SYSTEM_VITA             AL_ID('V','I','T',' ')
AL_VAR(SYSTEM_DRIVER, system_vita);

/* Timer driver */
#define TIMER_VITA                AL_ID('V','I','T','T')
AL_VAR(TIMER_DRIVER, timer_vita);

/* Keyboard driver */
#define KEYSIM_VITA              AL_ID('V','I','T','K')
AL_VAR(KEYBOARD_DRIVER, keybd_simulator_vita);

/* Gfx driver */
#define GFX_VITA                  AL_ID('V','I','T','G')
AL_VAR(GFX_DRIVER, gfx_vita);

/* Digital sound driver */
#define DIGI_VITA                 AL_ID('V','I','T','S')
AL_VAR(DIGI_DRIVER, digi_vita);

/* Joystick drivers */
#define JOYSTICK_VITA             AL_ID('V','I','T','J')
AL_VAR(JOYSTICK_DRIVER, joystick_vita);

#endif
