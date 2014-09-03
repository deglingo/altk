/* altkdisplay.h -
 */

#ifndef _ALTKDISPLAY_H_
#define _ALTKDISPLAY_H_

#include "altk/altkbase.h"
#include "altk/altkregion.h"
#include "altk/altkdrawable.h"
#include "altk/altkdisplay-def.h"



struct _AltkWidget;
struct _AltkWindow;



/* AltkDisplay:
 */
struct _AltkDisplay
{
  ALTK_DISPLAY_INSTANCE_HEADER;
  /* the underlying allegro display */
  ALLEGRO_DISPLAY *al_display;
  /* [REMOVEME] top-widgets attached to this display */
  GList *top_widgets;
  /* the root window */
  struct _AltkWindow *root_window;
  /* id of the redraw idle source */
  guint redraw_source_id;
  /* area which needs a redraw */
  AltkRegion *update_area;
  /* the display back-buffer */
  AltkDrawable *backbuf;
  /* the double-buffer to safely draw widgets */
  AltkDrawable *dblbuf;
  /* this one should be in private, only defined if DEBUG_UPDATES is
     enabled */
  AltkDrawable *debugbuf;
  /* resize queue */
  GSList *resize_queue;
  guint resize_source_id;
};



/* AltkDisplayClass:
 */
struct _AltkDisplayClass
{
  ALTK_DISPLAY_CLASS_HEADER;
};



AltkDisplay *altk_display_new ( void );
void altk_display_open ( AltkDisplay *display );
void altk_display_attach_widget ( AltkDisplay *display,
                                  struct _AltkWidget *widget );
void altk_display_queue_draw ( AltkDisplay *display,
                               AltkRegion *area );
void altk_display_queue_resize ( AltkDisplay *display,
                                 struct _AltkWidget *widget );
struct _AltkWidget *altk_display_get_widget_at ( AltkDisplay *display,
                                                 gint x,
                                                 gint y );



#endif
