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

#include <stdlib.h>
#include <glib.h>
#include <gdk/gdkkeysyms.h>
#include <vte/vte.h>
#include <wordexp.h>
#include "config.h"

static void
xdg_open_selection_cb (GtkClipboard *clipboard, const char* string, gpointer data)
{
    char* command;
    wordexp_t result;
    int ret;
    GError* error = NULL;

    command = g_strconcat("xdg-open ", string, NULL);
    ret = wordexp(command, &result, WRDE_NOCMD);
    switch (ret) {
        case 0:
            g_spawn_async(NULL, result.we_wordv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, &error);
            if (error) {
                g_printerr("xdg-open: %s\n", error->message);
                g_error_free(error);
            }
            break;
        case WRDE_BADCHAR:
            g_printerr("'%s' contains an invalid character\n", string);
            break;
        case WRDE_CMDSUB:
            g_printerr("'%s' uses command substitution, which is not allowed\n", string);
            break;
        case WRDE_NOSPACE:
            g_printerr("Could not allocate enough memory when parsing '%s'\n", string);
            break;
        case WRDE_SYNTAX:
            g_printerr("Syntax error in '%s'\n", string);
            break;
    }
    wordfree(&result);
}

static void
xdg_open_selection (GtkWidget* terminal)
{
    GdkDisplay* display;
    GtkClipboard* clipboard;

    vte_terminal_copy_primary(VTE_TERMINAL (terminal));
    display = gtk_widget_get_display(terminal);
    clipboard = gtk_clipboard_get_for_display(display, GDK_SELECTION_PRIMARY);
    gtk_clipboard_request_text(clipboard, xdg_open_selection_cb, NULL);
}

static gboolean
on_key_press (GtkWidget* terminal, GdkEventKey* event)
{
    if ((event->state & TINYTERM_MODIFIERS) == (TINYTERM_MODIFIERS)) {
        switch (event->keyval) {
            case GDK_C:
                vte_terminal_copy_clipboard(VTE_TERMINAL (terminal));
                return TRUE;
            case GDK_V:
                vte_terminal_paste_clipboard(VTE_TERMINAL (terminal));
                return TRUE;
            case GDK_X:
                xdg_open_selection(terminal);
                return TRUE;
        }
    }
    return FALSE;
}

static void
vte_config(VteTerminal* vte)
{
    GdkColor color_fg, color_bg;
    GdkColor color_palette[16];

    vte_terminal_set_audible_bell           (vte, TINYTERM_AUDIBLE_BELL);
    vte_terminal_set_visible_bell           (vte, TINYTERM_VISIBLE_BELL);
    vte_terminal_set_cursor_shape           (vte, TINYTERM_CURSOR_SHAPE);
    vte_terminal_set_cursor_blink_mode      (vte, TINYTERM_CURSOR_BLINK);
    vte_terminal_set_word_chars             (vte, TINYTERM_WORD_CHARS);
    vte_terminal_set_scrollback_lines       (vte, TINYTERM_SCROLLBACK_LINES);
    vte_terminal_set_font_from_string_full  (vte, TINYTERM_FONT, TINYTERM_ANTIALIAS);

    /* init colors */
    gdk_color_parse(TINYTERM_COLOR_FOREGROUND, &color_fg);
    gdk_color_parse(TINYTERM_COLOR_BACKGROUND, &color_bg);
    gdk_color_parse(TINYTERM_COLOR0,  &color_palette[0]);
    gdk_color_parse(TINYTERM_COLOR1,  &color_palette[1]);
    gdk_color_parse(TINYTERM_COLOR2,  &color_palette[2]);
    gdk_color_parse(TINYTERM_COLOR3,  &color_palette[3]);
    gdk_color_parse(TINYTERM_COLOR4,  &color_palette[4]);
    gdk_color_parse(TINYTERM_COLOR5,  &color_palette[5]);
    gdk_color_parse(TINYTERM_COLOR6,  &color_palette[6]);
    gdk_color_parse(TINYTERM_COLOR7,  &color_palette[7]);
    gdk_color_parse(TINYTERM_COLOR8,  &color_palette[8]);
    gdk_color_parse(TINYTERM_COLOR9,  &color_palette[9]);
    gdk_color_parse(TINYTERM_COLOR10, &color_palette[10]);
    gdk_color_parse(TINYTERM_COLOR11, &color_palette[11]);
    gdk_color_parse(TINYTERM_COLOR12, &color_palette[12]);
    gdk_color_parse(TINYTERM_COLOR13, &color_palette[13]);
    gdk_color_parse(TINYTERM_COLOR14, &color_palette[14]);
    gdk_color_parse(TINYTERM_COLOR15, &color_palette[15]);

    vte_terminal_set_colors(vte,
                            &color_fg,
                            &color_bg,
                            &color_palette,
                            16);
}

static void
vte_spawn(VteTerminal* vte, char* working_directory, char* command, char** environment)
{
    GError* error = NULL;
    GPid ppid;
    char** command_argv = NULL;

    /* Create pty object */
    VtePty* pty = vte_terminal_pty_new(vte, (VTE_PTY_NO_HELPER | VTE_PTY_NO_FALLBACK), &error);
    if (!pty) {
        g_printerr("Failed to create pty: %s\n", error->message);
        g_error_free(error);
        exit(EXIT_FAILURE);
    }
    vte_pty_set_term(pty, TINYTERM_TERMINFO);
    vte_terminal_set_pty_object(vte, pty);

    /* Parse command into array */
    if (!command)
        command = vte_get_user_shell();
    g_shell_parse_argv(command, NULL, &command_argv, &error);
    if (error) {
        g_printerr("Failed to parse command: %s\n", error->message);
        g_error_free(error);
        exit(EXIT_FAILURE);
    }

    /* Spawn default shell (or specified command) */
    g_spawn_async(working_directory,   // working directory (NULL=CWD)
                  command_argv,        // arguments
                  environment,         // environment
                  (G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_SEARCH_PATH | G_SPAWN_LEAVE_DESCRIPTORS_OPEN),  // flags from GSpawnFlags
                  (GSpawnChildSetupFunc)vte_pty_child_setup, // an extra child setup function to run in the child just before exec()
                  pty,     // user data for child_setup
                  &ppid,   // a location to store the child PID
                  &error); // return location for a GError
    if (error) {
        g_printerr("%s\n", error->message);
        g_error_free(error);
        exit(EXIT_FAILURE);
    }
    vte_terminal_watch_child(vte, ppid);
    g_strfreev(command_argv);
}

static void
parse_arguments(int argc, char* argv[], char** command, char** directory)
{
    GError* error = NULL;

    GOptionContext* context = g_option_context_new(NULL);
    g_option_context_set_help_enabled(context, TRUE);
    const GOptionEntry entries[] = {
        {"execute",   'e', 0, G_OPTION_ARG_STRING, command,   "Execute command instead of default shell.", "COMMAND"},
        {"directory", 'd', 0, G_OPTION_ARG_STRING, directory, "Sets the working directory for the shell (or the command specified via -e).", "PATH"},
        { NULL }
    };
    g_option_context_add_main_entries(context, entries, NULL);
    g_option_context_parse(context, &argc, &argv, &error);
    if (error) {
        g_printerr("option parsing failed: %s\n", error->message);
        g_error_free(error);
        exit(EXIT_FAILURE);
    }
    g_option_context_free(context);
}

int
main (int argc, char* argv[])
{
    GtkWidget* window;
    GtkWidget* box;
    GdkPixbuf* icon;
    GdkGeometry geo_hints;
    char* command = NULL;
    char* directory = NULL;

    gtk_init(&argc, &argv);
    parse_arguments(argc, argv, &command, &directory);

    /* Create window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
    gtk_window_set_title(GTK_WINDOW (window), "TinyTerm");
    icon = gdk_pixbuf_new_from_file(TINYTERM_ICON_PATH, NULL);
    if (icon)
        gtk_window_set_icon(GTK_WINDOW (window), icon);

    /* Create main box */
    box = gtk_hbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER (window), box);

    /* Create vte terminal widget */
    GtkWidget* vte_widget = vte_terminal_new();
    g_signal_connect(vte_widget, "child-exited", gtk_main_quit, NULL);
    g_signal_connect(vte_widget, "key-press-event", G_CALLBACK (on_key_press), NULL);
    gtk_box_pack_start(GTK_BOX (box), vte_widget, TRUE, TRUE, 0);
    VteTerminal* vte = VTE_TERMINAL (vte_widget);

    /* Show scrollbar */
    #ifdef TINYTERM_SCROLLBAR_VISIBLE
    GtkWidget* scrollbar;
    scrollbar = gtk_vscrollbar_new(vte->adjustment);
    gtk_box_pack_start(GTK_BOX (box), scrollbar, FALSE, FALSE, 0);
    #endif // TINYTERM_SCROLLBAR_VISIBLE

    /* Apply geometry hints to handle terminal resizing */
    geo_hints.base_width  = vte->char_width;
    geo_hints.base_height = vte->char_height;
    geo_hints.min_width   = vte->char_width;
    geo_hints.min_height  = vte->char_height;
    geo_hints.width_inc   = vte->char_width;
    geo_hints.height_inc  = vte->char_height;
    gtk_window_set_geometry_hints(GTK_WINDOW (window), vte_widget, &geo_hints,
                                  GDK_HINT_RESIZE_INC | GDK_HINT_MIN_SIZE | GDK_HINT_BASE_SIZE);

    vte_config(vte);
    vte_spawn(vte, directory, command, NULL);
    g_free(command);
    g_free(directory);

    /* Show widgets and run main loop */
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
