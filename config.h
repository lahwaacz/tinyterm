/*
 * MIT/X Consortium License
 *
 * © 2013 Jakub Klinkovský
 * © 2009 Sebastian Linke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

/* Path to window icon */
#define TINYTERM_ICON_PATH      "/usr/local/share/pixmaps/tinyterm.png"

/* Modifier keys for copy/paste/execute callbacks */
/* http://www.gtk.org/api/2.6/gdk/gdk-Windows.html#GdkModifierType */
#define TINYTERM_MODIFIERS      (GDK_CONTROL_MASK | GDK_SHIFT_MASK)

/* Terminal emulation (value of $TERM) (default: xterm) */
#define TINYTERM_TERMINFO 		"xterm-256color"

/* Uncomment to show scrollbar */
//#define TINYTERM_SCROLLBAR_VISIBLE

#define TINYTERM_AUDIBLE_BELL   FALSE
#define TINYTERM_VISIBLE_BELL   FALSE

/* One of VTE_CURSOR_SHAPE_BLOCK, VTE_CURSOR_SHAPE_IBEAM, VTE_CURSOR_SHAPE_UNDERLINE */
#define TINYTERM_CURSOR_SHAPE    VTE_CURSOR_SHAPE_BLOCK

/* One of VTE_CURSOR_BLINK_SYSTEM, VTE_CURSOR_BLINK_ON, VTE_CURSOR_BLINK_OFF */
#define TINYTERM_CURSOR_BLINK    VTE_CURSOR_BLINK_OFF

/* Selection behavior for double-clicks */
#define TINYTERM_WORD_CHARS "-A-Za-z0-9:./?%&#_=+@~"

/* Number of lines to save in scrollback buffer */
#define TINYTERM_SCROLLBACK_LINES   10000

/* Font description */
#define TINYTERM_FONT       "monospace 9"

/* One of VTE_ANTI_ALIAS_USE_DEFAULT, VTE_ANTI_ALIAS_FORCE_ENABLE, VTE_ANTI_ALIAS_FORCE_DISABLE */
#define TINYTERM_ANTIALIAS  VTE_ANTI_ALIAS_FORCE_ENABLE

/* Custom color scheme */
#define TINYTERM_COLOR_BACKGROUND   "#000000"
#define TINYTERM_COLOR_FOREGROUND   "#bebebe"
/* black */
#define TINYTERM_COLOR0     "#000000"
#define TINYTERM_COLOR8     "#4d4d4d"
/* red */
#define TINYTERM_COLOR1     "#B22222"
#define TINYTERM_COLOR9     "#ED2939"
/* green */
#define TINYTERM_COLOR2     "#00a000"
#define TINYTERM_COLOR10    "#32cd32"
/* yellow */
#define TINYTERM_COLOR3     "#cdcd00"
#define TINYTERM_COLOR11    "#ffff00"
/* blue */
#define TINYTERM_COLOR4     "#2346AE"
#define TINYTERM_COLOR12    "#2b65ec"
/* magenta */
#define TINYTERM_COLOR5     "#AA00AA"
#define TINYTERM_COLOR13    "#C154C1"
/* cyan */
#define TINYTERM_COLOR6     "#58C6ED"
#define TINYTERM_COLOR14    "#00DFFF"
/* white */
#define TINYTERM_COLOR7     "#e5e5e5"
#define TINYTERM_COLOR15    "#ffffff"

///* Default color scheme - matches default colors of urxvt */
//#define TINYTERM_COLOR_BACKGROUND   "#000000"
//#define TINYTERM_COLOR_FOREGROUND   "#bebebe"
///* black */
//#define TINYTERM_COLOR0      "#000000"
//#define TINYTERM_COLOR8      "#4d4d4d"
///* red */
//#define TINYTERM_COLOR1      "#cd0000"
//#define TINYTERM_COLOR9      "#ff0000"
///* green */
//#define TINYTERM_COLOR2      "#00cd00"
//#define TINYTERM_COLOR10     "#00ff00"
///* yellow */
//#define TINYTERM_COLOR3      "#cdcd00"
//#define TINYTERM_COLOR11     "#ffff00"
///* blue */
//#define TINYTERM_COLOR4      "#0000cd"
//#define TINYTERM_COLOR12     "#2b65ec"
///* magenta */
//#define TINYTERM_COLOR5      "#cd00cd"
//#define TINYTERM_COLOR13     "#ff00ff"
///* cyan */
//#define TINYTERM_COLOR6      "#00cdcd"
//#define TINYTERM_COLOR14     "#00ffff"
///* white */
//#define TINYTERM_COLOR7      "#e5e5e5"
//#define TINYTERM_COLOR15     "#ffffff"
