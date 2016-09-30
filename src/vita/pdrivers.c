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
 *      List of vita drivers.
 *
 *      By enrique somolinos.
 *
 *      See readme.txt for copyright information.
 */


#include "allegro.h"

#ifndef ALLEGRO_VITA
#error Something is wrong with the makefile
#endif


_DRIVER_INFO _system_driver_list[] =
{
   { SYSTEM_VITA,              &system_vita,              TRUE  },
   { SYSTEM_NONE,             &system_none,             FALSE },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _keyboard_driver_list[] =
{
   { KEYSIM_VITA,              &keybd_simulator_vita,     TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _timer_driver_list[] =
{
 //  { TIMER_VITA,               &timer_vita,               TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _mouse_driver_list[] =
{
// { MOUSE_VITA,               &mouse_vita,               TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _gfx_driver_list[] =
{
   { GFX_VITA,                 &gfx_vita,                 TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _digi_driver_list[] =
{
   { DIGI_VITA,                 &digi_vita,               TRUE  },
   { 0,                        NULL,                    0     }
};


_DRIVER_INFO _midi_driver_list[] =
{
   { 0,                        NULL,                    0     }
};


BEGIN_JOYSTICK_DRIVER_LIST
//{   JOYSTICK_VITA,              &joystick_vita,           TRUE  },
END_JOYSTICK_DRIVER_LIST
