/* altkwindow.h -
 */

#ifndef _ALTKWINDOW_H_
#define _ALTKWINDOW_H_

#include "altk/altkbase.h"
#include "altk/altkregion.h"
#include "altk/altkevent.h"
#include "altk/altkwindow-def.h"



struct _AltkDisplay;



/* AltkWindowFlags:
 */
typedef enum _AltkWindowFlags
  {
    ALTK_WINDOW_FLAG_ROOT = 1 << 0,
    ALTK_WINDOW_FLAG_INPUT_ONLY = 1 << 1,
  }
  AltkWindowFlags;



/* AltkDrawingContext:
 */
typedef struct _AltkDrawingContext
{
  AltkRegion *area;
  gint offset_x;
  gint offset_y;
}
  AltkDrawingContext;



/* AltkWindow:
 */
struct _AltkWindow
{
  ALTK_WINDOW_INSTANCE_HEADER;
  /* private datas */
  gpointer private;
  /* window flags */
  AltkWindowFlags flags;
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
  /* event mask */
  AltkEventMask event_mask;
  /* area of the window which needs redraw */
  AltkRegion *update_area;
  /* drawing offset */
  gint offset_x;
  gint offset_y;
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
/* #define ALTK_DEBUG_UPDATES */
#define ALTK_DEBUG_UPDATE_DELAY 100000 /* µs */

#ifdef ALTK_DEBUG_UPDATES
#  define ALTK_WINDOW_DRAW_UPDATE(win, area, ofsx, ofsy, hcol)   \
  _altk_window_draw_update((win), (area), (ofsx), (ofsy), (hcol))
#else
#  define ALTK_WINDOW_DRAW_UPDATE(win, area, ofsx, ofsy, hcol)
#endif

void _altk_window_draw_update ( AltkWindow *window,
                                AltkRegion *area,
                                gint ofsx,
                                gint ofsy,
                                guint32 hcol );



AltkWindow *altk_window_new_root ( struct _AltkDisplay *display );
AltkWindow *altk_window_new ( AltkWindow *parent,
                              gint x,
                              gint y,
                              gint width,
                              gint height,
                              AltkWindowFlags flags );
void altk_window_destroy ( AltkWindow *window );
void altk_window_set_event_mask ( AltkWindow *window,
                                  AltkEventMask mask );
void altk_window_set_bounds ( AltkWindow *window,
                              gint x,
                              gint y,
                              gint width,
                              gint height );
void altk_window_invalidate ( AltkWindow *window,
                              AltkRegion *area );
AltkRegion *altk_window_get_visible_area ( AltkWindow *window );
void altk_window_begin_draw ( AltkWindow *window,
                              AltkDrawingContext *context );
void altk_window_end_draw ( AltkWindow *window,
                            AltkDrawingContext *context );
AltkWindow *altk_window_get_child_at ( AltkWindow *window,
                                       gint x,
                                       gint y );



#endif
