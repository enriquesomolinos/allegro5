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
 *      vita datafile bitmap objects reading routines.
 *      Used by the vita gfx drivers with scaling capabilities.
 *
 *      By enrqiue somolinos.
 *
 *      See readme.txt for copyright information.
 */


#include "allegro.h"
#include "allegro/internal/aintern.h"


static BITMAP *vita_scaled_read_bitmap(PACKFILE *f, int bits, int allowconv)
{
   /* Sustituimos la llamada original a create_bitmap_ex por esta
    * para que no haga escalado. */
   bmp = vita_create_bitmap_ex(color_depth, width, height, FALSE);

   /******************************************************************/
   /* Código justo a continuación del código original de read_bitmap */
   BITMAP *tmp = bmp;
   bmp = vita_create_bitmap_ex(destbits, w*scale_factor, h*scale_factor, FALSE);
   if (!bmp) {
      *allegro_errno = ENOMEM;
      return NULL;
   }

   /* Escalamos el bitmap leído del datafile */
   stretch_blit(tmp, bmp, 0, 0, tmp->w, tmp->h, 0, 0, bmp->w, bmp->h);

   destroy_bitmap(tmp);
}



static void *vita_load_bitmap_object(PACKFILE *f, long size)
{
   short bits = pack_mgetw(f);

   return vita_scaled_read_bitmap(f, bits, TRUE);
}
