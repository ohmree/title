/* It's a good idea to paste this and other long code examples
   into a text editor for easier reading */

/* #include <stdlib.h> */
/* #include <stdio.h> */
/* #include <string.h> */
#include <xcb/xcb.h>
#include <unistd.h>

#ifndef __GNUC__
char*
strdup(const char* s)
{
    int n = strlen(s) + 1;

    char *dup = malloc(n);

    if(dup)
        strcpy(dup, s);

    return dup;
}
#endif

/* Variables */

static const xcb_setup_t *setup;
static const uint32_t values[] = { 10, 20, 200, 300 };

static int screen_nbr;
static xcb_connection_t *c;
static xcb_screen_iterator_t iter;
static xcb_screen_t *screen;
static xcb_window_t win;

int
main()
{
    /* Open a connection to the X11 server. Use the DISPLAY environment variable. */
    c = xcb_connect(NULL, &screen_nbr);


    /* Get the screen whose number is screen_num */
    setup = xcb_get_setup(c);
    iter = xcb_setup_roots_iterator(setup);

    /* We want the screen at index screen_num of the iterator */
    for (int i = 0; i < screen_nbr; ++i)
        xcb_screen_next(&iter);

    screen = iter.data;
    /* Ask for window ID */
    win = xcb_generate_id(c);

    /* Create the window */
    xcb_create_window (c,                             /* Connection          */
                       XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
                       win,                           /* window Id           */
                       screen->root,                  /* parent window       */
                       0, 0,                          /* x, y                */
                       150, 150,                      /* width, height       */
                       10,                            /* border_width        */
                       XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                       screen->root_visual,           /* visual              */
                       0, NULL);                      /* masks, not used yet */

    /* Move the window to coordinates x = 10 and y = 20 */
    xcb_configure_window (c, win, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);

    /* Map the window on the screen */
    xcb_map_window(c, win);

    /* Make sure commands are sent before we pause, so window is shown */
    xcb_flush(c);

    /* Wait */
    sleep(5);

    xcb_unmap_window(c, win);
    xcb_flush(c);

    /* Pause until Ctrl-C */
    pause();

    xcb_disconnect(c);
    return 0;
}
