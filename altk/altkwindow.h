/* altkwindow.h -
 */

#ifndef _ALTKWINDOW_H_
#define _ALTKWINDOW_H_

#include "altk/altkdrawable.h"
#include "altk/altkregion.h"
#include "altk/altkwindow-def.h"



struct _AltkDisplay;



/* AltkWindow:
 */
struct _AltkWindow
{
  ALTK_WINDOW_INSTANCE_HEADER;
  /* the display to which this window is attached */
  struct _AltkDisplay *display;
  /* parent and children (where first child is above) */
  struct _AltkWindow *parent;
  struct _AltkWindow *children;
  struct _AltkWindow *children_tail;
  struct _AltkWindow *next;
  struct _AltkWindow *prev;
  /* window bounds (relative to parent window) */
  gint x;
  gint y;
  gint width;
  gint height;
  /* global coordinates (relative to root window) */
  gint root_x;
  gint root_y;
  /* area of the window which needs redraw */
  AltkRegion *update_area;
  /* some data for the user, totally ignored by the window itself */
  gpointer user_data;
};



/* AltkWindowClass:
 */
struct _AltkWindowClass
{
  ALTK_WINDOW_CLASS_HEADER;
};



/* debug update stuff */
#define ALTK_DEBUG_UPDATES
#define ALTK_DEBUG_UPDATE_DELAY 1000000 /* µs */

#ifdef ALTK_DEBUG_UPDATES
#  define ALTK_WINDOW_DRAW_UPDATE(win, area, hcol) \
  _altk_window_draw_update((win), (area), (hcol))
#else
#  define ALTK_WINDOW_DRAW_UPDATE(win, area, hcol)
#endif

void _altk_window_draw_update ( AltkWindow *window,
                                AltkRegion *area,
                                guint32 hcol );



AltkWindow *altk_window_new_root ( struct _AltkDisplay *display );
AltkWindow *altk_window_new ( AltkWindow *parent,
                              gint x,
                              gint y,
                              gint width,
                              gint height );
void altk_window_set_bounds ( AltkWindow *window,
                              gint x,
                              gint y,
                              gint width,
                              gint height );
void altk_window_invalidate ( AltkWindow *window,
                              AltkRegion *area );
AltkRegion *altk_window_get_visible_area ( AltkWindow *window );



#endif
