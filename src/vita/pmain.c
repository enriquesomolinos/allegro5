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
 *      Replacement for the Allegro program main() function to execute
 *      the VITA stuff: call the VITA* macros and create the standard
 *      exit callback.
 *
 *      By enrique somolinos.
 *
 *      See readme.txt for copyright information.
 */

#include <psp2/kernel/processmgr.h>

#undef main


extern void *_mangled_main_address;






/* exit_callback:
 *
 */
static int exit_callback(int arg1, int arg2, void *common)
{
   sceKernelExitProcess(0);
   return 0;
}



/* callback_thread:
 *  Registers the exit callback.
 */
static int callback_thread(SceSize args, void *argp)
{
   int cbid = sceKernelCreateCallback("Exit Callback",0, (void *) exit_callback, NULL);
   //TODO
  /* sceKernelRegisterExitCallback(cbid);
   sceKernelSleepThreadCB();*/
   return 0;
}



/* setup_callback:
 *  Sets up the callback thread and returns its thread id.
 */
static int setup_callback(void)
{
   int thid = 0;
   thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, 0,NULL);
   if (thid >= 0) {
      sceKernelStartThread(thid, 0, 0);
   }
   return thid;
}



/* main:
 *  Replacement for main function.
 */
int main(void)
{
   int (*real_main) (void) = (int (*) (void)) _mangled_main_address;

   setup_callback();
   return (*real_main)();
}

