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
 *      Backward compatibility stuff.
 *
 *      By Shawn Hargreaves.
 *
 *      See readme.txt for copyright information.
 */


#ifndef ALLEGRO_COMPAT_H
#define ALLEGRO_COMPAT_H

#ifdef __cplusplus
   extern "C" {
#endif


#ifndef ALLEGRO_LIB_BUILD

   #ifndef ALLEGRO_NO_CLEAR_BITMAP_ALIAS
      #if (defined ALLEGRO_GCC)
         static __attribute__((unused)) __inline__ void clear(BITMAP *bmp)
         {
            clear_bitmap(bmp);
         }
      #else
         static INLINE void clear(BITMAP *bmp)
         {
            clear_bitmap(bmp);
         }
      #endif
   #endif

   #ifndef ALLEGRO_NO_FIX_ALIASES
      #if (defined ALLEGRO_GCC)
         #define AL_ALIAS(DECL, CALL)                      \
         static __attribute__((unused)) __inline__ DECL    \
         {                                                 \
            return CALL;                                   \
         }
      #else
         #define AL_ALIAS(DECL, CALL)              \
         static INLINE DECL                        \
         {                                         \
            return CALL;                           \
         }
      #endif
      AL_ALIAS(fixed fadd(fixed x, fixed y), fixadd(x, y))
      AL_ALIAS(fixed fsub(fixed x, fixed y), fixsub(x, y))
      AL_ALIAS(fixed fmul(fixed x, fixed y), fixmul(x, y))
      AL_ALIAS(fixed fdiv(fixed x, fixed y), fixdiv(x, y))
      AL_ALIAS(int fceil(fixed x), fixceil(x))
      AL_ALIAS(int ffloor(fixed x), fixfloor(x))
      AL_ALIAS(fixed fcos(fixed x), fixcos(x))
      AL_ALIAS(fixed fsin(fixed x), fixsin(x))
      AL_ALIAS(fixed ftan(fixed x), fixtan(x))
      AL_ALIAS(fixed facos(fixed x), fixacos(x))
      AL_ALIAS(fixed fasin(fixed x), fixasin(x))
      AL_ALIAS(fixed fatan(fixed x), fixatan(x))
      AL_ALIAS(fixed fatan2(fixed y, fixed x), fixatan2(y, x))
      AL_ALIAS(fixed fsqrt(fixed x), fixsqrt(x))
      AL_ALIAS(fixed fhypot(fixed x, fixed y), fixhypot(x, y))
      #undef AL_ALIAS
   #endif

#endif  /* !defined ALLEGRO_LIB_BUILD */


#define KB_NORMAL       1
#define KB_EXTENDED     2

#define SEND_MESSAGE    object_message

#define cpu_fpu         (cpu_capabilities & CPU_FPU)
#define cpu_mmx         (cpu_capabilities & CPU_MMX)
#define cpu_3dnow       (cpu_capabilities & CPU_3DNOW)
#define cpu_cpuid       (cpu_capabilities & CPU_ID)

#define joy_x           (joy[0].stick[0].axis[0].pos)
#define joy_y           (joy[0].stick[0].axis[1].pos)
#define joy_left        (joy[0].stick[0].axis[0].d1)
#define joy_right       (joy[0].stick[0].axis[0].d2)
#define joy_up          (joy[0].stick[0].axis[1].d1)
#define joy_down        (joy[0].stick[0].axis[1].d2)
#define joy_b1          (joy[0].button[0].b)
#define joy_b2          (joy[0].button[1].b)
#define joy_b3          (joy[0].button[2].b)
#define joy_b4          (joy[0].button[3].b)
#define joy_b5          (joy[0].button[4].b)
#define joy_b6          (joy[0].button[5].b)
#define joy_b7          (joy[0].button[6].b)
#define joy_b8          (joy[0].button[7].b)

#define joy2_x          (joy[1].stick[0].axis[0].pos)
#define joy2_y          (joy[1].stick[0].axis[1].pos)
#define joy2_left       (joy[1].stick[0].axis[0].d1)
#define joy2_right      (joy[1].stick[0].axis[0].d2)
#define joy2_up         (joy[1].stick[0].axis[1].d1)
#define joy2_down       (joy[1].stick[0].axis[1].d2)
#define joy2_b1         (joy[1].button[0].b)
#define joy2_b2         (joy[1].button[1].b)

#define joy_throttle    (joy[0].stick[2].axis[0].pos)

#define joy_hat         ((joy[0].stick[1].axis[0].d1) ? 1 :             \
                           ((joy[0].stick[1].axis[0].d2) ? 3 :          \
                              ((joy[0].stick[1].axis[1].d1) ? 4 :       \
                                 ((joy[0].stick[1].axis[1].d2) ? 2 :    \
                                    0))))

#define JOY_HAT_CENTRE        0
#define JOY_HAT_CENTER        0
#define JOY_HAT_LEFT          1
#define JOY_HAT_DOWN          2
#define JOY_HAT_RIGHT         3
#define JOY_HAT_UP            4

AL_FUNC_DEPRECATED(int, initialise_joystick, (void));


/* in case you want to spell 'palette' as 'pallete' */
#define PALLETE                        PALETTE
#define black_pallete                  black_palette
#define desktop_pallete                desktop_palette
#define set_pallete                    set_palette
#define get_pallete                    get_palette
#define set_pallete_range              set_palette_range
#define get_pallete_range              get_palette_range
#define fli_pallete                    fli_palette
#define pallete_color                  palette_color
#define DAT_PALLETE                    DAT_PALETTE
#define select_pallete                 select_palette
#define unselect_pallete               unselect_palette
#define generate_332_pallete           generate_332_palette
#define generate_optimised_pallete     generate_optimised_palette


/* a pretty vague name */
#define fix_filename_path              canonicalize_filename


/* the good old file selector */
#define OLD_FILESEL_WIDTH   -1
#define OLD_FILESEL_HEIGHT  -1

AL_INLINE_DEPRECATED(int, file_select, (AL_CONST char *message, char *path, AL_CONST char *ext),
{
   return file_select_ex(message, path, ext, 1024, OLD_FILESEL_WIDTH, OLD_FILESEL_HEIGHT);
})


/* the old (and broken!) file enumeration function */
AL_FUNC_DEPRECATED(int, for_each_file, (AL_CONST char *name, int attrib, AL_METHOD(void, callback, (AL_CONST char *filename, int attrib, int param)), int param));


/* the old state-based textout functions */
AL_VAR(int, _textmode);
AL_FUNC_DEPRECATED(int, text_mode, (int mode));

AL_INLINE_DEPRECATED(void, textout, (struct BITMAP *bmp, AL_CONST FONT *f, AL_CONST char *str, int x, int y, int color),
{
   textout_ex(bmp, f, str, x, y, color, _textmode);
})

AL_INLINE_DEPRECATED(void, textout_centre, (struct BITMAP *bmp, AL_CONST FONT *f, AL_CONST char *str, int x, int y, int color),
{
   textout_centre_ex(bmp, f, str, x, y, color, _textmode);
})

AL_INLINE_DEPRECATED(void, textout_right, (struct BITMAP *bmp, AL_CONST FONT *f, AL_CONST char *str, int x, int y, int color),
{
   textout_right_ex(bmp, f, str, x, y, color, _textmode);
})

AL_INLINE_DEPRECATED(void, textout_justify, (struct BITMAP *bmp, AL_CONST FONT *f, AL_CONST char *str, int x1, int x2, int y, int diff, int color),
{
   textout_justify_ex(bmp, f, str, x1, x2, y, diff, color, _textmode);
})

AL_PRINTFUNC_DEPRECATED(void, textprintf, (struct BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...), 6, 7);
AL_PRINTFUNC_DEPRECATED(void, textprintf_centre, (struct BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...), 6, 7);
AL_PRINTFUNC_DEPRECATED(void, textprintf_right, (struct BITMAP *bmp, AL_CONST FONT *f, int x, int y, int color, AL_CONST char *format, ...), 6, 7);
AL_PRINTFUNC_DEPRECATED(void, textprintf_justify, (struct BITMAP *bmp, AL_CONST FONT *f, int x1, int x2, int y, int diff, int color, AL_CONST char *format, ...), 8, 9);

AL_INLINE_DEPRECATED(void, draw_character, (BITMAP *bmp, BITMAP *sprite, int x, int y, int color),
{
   draw_character_ex(bmp, sprite, x, y, color, _textmode);
})

AL_INLINE_DEPRECATED(int, gui_textout, (struct BITMAP *bmp, AL_CONST char *s, int x, int y, int color, int centre),
{
   return gui_textout_ex(bmp, s, x, y, color, _textmode, centre);
})


/* the old close button functions */
AL_INLINE_DEPRECATED(int, set_window_close_button, (int enable),
{
   (void)enable;
   return 0;
})

AL_INLINE_DEPRECATED(void, set_window_close_hook, (void (*proc)(void)),
{
   set_close_button_callback(proc);
})


/* the weird old clipping API */
AL_FUNC_DEPRECATED(void, set_clip, (BITMAP *bitmap, int x1, int y1, int x2, int y2));


/* unnecessary, can use rest(0) */
AL_INLINE_DEPRECATED(void, yield_timeslice, (void),
{
   ASSERT(system_driver);

   if (system_driver->yield_timeslice)
      system_driver->yield_timeslice();
})


/* the old timer API
 *
 * Note: TIMERS_PER_SECOND was changed to a nicer value (was 1193181L).
 * The old timer API is emulated using the new timer API, which works with
 * milliseconds.  With the previous value of TIMERS_PER_SECOND, the two-step
 * conversion to "timers" then to milliseconds would introduce rounding
 * errors, e.g. causing the three extimer counters to go badly out of sync.
 */
#define TIMERS_PER_SECOND     1000000L
#define SECS_TO_TIMER(x)      ((long)(x) * TIMERS_PER_SECOND)
#define MSEC_TO_TIMER(x)      ((long)(x) * (TIMERS_PER_SECOND / 1000))
#define BPS_TO_TIMER(x)       (TIMERS_PER_SECOND / (long)(x))
#define BPM_TO_TIMER(x)       ((60 * TIMERS_PER_SECOND) / (long)(x))

typedef struct TIMER_DRIVER
{
   int  id;
   AL_CONST char *name;
   AL_CONST char *desc;
   AL_CONST char *ascii_name;
} TIMER_DRIVER;

AL_VAR(TIMER_DRIVER *, timer_driver);

AL_FUNC(int, install_timer, (void));
AL_FUNC(void, remove_timer, (void));

AL_FUNC(int, install_int_ex, (AL_METHOD(void, proc, (void)), long speed));
AL_FUNC(int, install_int, (AL_METHOD(void, proc, (void)), long speed));
AL_FUNC(void, remove_int, (AL_METHOD(void, proc, (void))));

AL_FUNC(int, install_param_int_ex, (AL_METHOD(void, proc, (void *param)), void *param, long speed));
AL_FUNC(int, install_param_int, (AL_METHOD(void, proc, (void *param)), void *param, long speed));
AL_FUNC(void, remove_param_int, (AL_METHOD(void, proc, (void *param)), void *param));

AL_VAR(volatile int, retrace_count);
AL_FUNCPTR(void, retrace_proc, (void));

#ifdef ALLEGRO_LIB_BUILD
   AL_FUNC(int,  timer_can_simulate_retrace, (void));
   AL_FUNC(void, timer_simulate_retrace, (int enable));
#else
   /* deprecated since 4.1.16 */
   AL_FUNC_DEPRECATED(int,  timer_can_simulate_retrace, (void));
   AL_FUNC_DEPRECATED(void, timer_simulate_retrace, (int enable));
#endif
AL_FUNC(int,  timer_is_using_retrace, (void));

AL_FUNC(void, rest, (unsigned int time));
AL_FUNC(void, rest_callback, (unsigned int time, AL_METHOD(void, callback, (void))));


/* the old joystick API */
#define JOY_TYPE_AUTODETECT      -1
#define JOY_TYPE_NONE            0


#define MAX_JOYSTICKS            8
#define MAX_JOYSTICK_AXIS        _AL_MAX_JOYSTICK_AXES
#define MAX_JOYSTICK_STICKS      _AL_MAX_JOYSTICK_STICKS
#define MAX_JOYSTICK_BUTTONS     _AL_MAX_JOYSTICK_BUTTONS


/* information about a single joystick axis */
typedef struct JOYSTICK_AXIS_INFO
{
   int pos;
   int d1, d2;
   AL_CONST char *name;
} JOYSTICK_AXIS_INFO;


/* information about one or more axis (a slider or directional control) */
typedef struct JOYSTICK_STICK_INFO
{
   int flags;
   int num_axis;
   JOYSTICK_AXIS_INFO axis[MAX_JOYSTICK_AXIS];
   AL_CONST char *name;
} JOYSTICK_STICK_INFO;


/* information about a joystick button */
typedef struct JOYSTICK_BUTTON_INFO
{
   int b;
   AL_CONST char *name;
} JOYSTICK_BUTTON_INFO;


/* information about an entire joystick */
typedef struct JOYSTICK_INFO
{
   int flags;
   int num_sticks;
   int num_buttons;
   JOYSTICK_STICK_INFO stick[MAX_JOYSTICK_STICKS];
   JOYSTICK_BUTTON_INFO button[MAX_JOYSTICK_BUTTONS];
} JOYSTICK_INFO;


/* joystick status flags */
#define JOYFLAG_DIGITAL             1
#define JOYFLAG_ANALOGUE            2
#define JOYFLAG_CALIB_DIGITAL       4
#define JOYFLAG_CALIB_ANALOGUE      8
#define JOYFLAG_CALIBRATE           16
#define JOYFLAG_SIGNED              32
#define JOYFLAG_UNSIGNED            64


/* alternative spellings */
#define JOYFLAG_ANALOG              JOYFLAG_ANALOGUE
#define JOYFLAG_CALIB_ANALOG        JOYFLAG_CALIB_ANALOGUE


/* global joystick information */
AL_ARRAY(JOYSTICK_INFO, joy);
AL_VAR(int, num_joysticks);


typedef struct JOYSTICK_DRIVER         /* driver for reading joystick input */
{
   int  id;
   AL_CONST char *name;
   AL_CONST char *desc;
   AL_CONST char *ascii_name;
} JOYSTICK_DRIVER;


/* AL_VAR(JOYSTICK_DRIVER, joystick_none); */
AL_VAR(JOYSTICK_DRIVER *, joystick_driver);


/* these were internals; I dunno where to put them --pw */
AL_VAR(int, _joy_type);
AL_VAR(int, _joystick_installed);


AL_FUNC(int, install_joystick, (int type));
AL_FUNC(void, remove_joystick, (void));

AL_FUNC(int, poll_joystick, (void));

AL_FUNC(int, save_joystick_data, (AL_CONST char *filename));
AL_FUNC(int, load_joystick_data, (AL_CONST char *filename));

AL_FUNC(AL_CONST char *, calibrate_joystick_name, (int n));
AL_FUNC(int, calibrate_joystick, (int n));


#ifdef __cplusplus
   }
#endif

#endif          /* ifndef ALLEGRO_COMPAT_H */

