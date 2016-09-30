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
 *      Internal header file for the Vita allegro library port.
 *
 *      By enrique somolinos.
 *
 *      See readme.txt for copyright information.
 */


#ifndef AINTVITA_H
#define AINTVITA_H


#define DEFAULT_SCREEN_WIDTH       960
#define DEFAULT_SCREEN_HEIGHT      544
#define DEFAULT_COLOR_DEPTH         32

#define BMP_EXTRA(bmp)		    ((BMP_EXTRA_INFO *)((bmp)->extra))

typedef struct BMP_EXTRA_INFO
{
   int pitch;
   BITMAP *parent;
} BMP_EXTRA_INFO;


#endif

