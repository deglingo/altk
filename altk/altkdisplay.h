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
  /* the root window */
  struct _AltkWindow *root_window;
  /* [FIXME] remove ? - the display back-buffer */
  ALLEGRO_BITMAP *backbuf;
  AltkRegion *update_area;
};



/* AltkDisplayClass:
 */
struct _AltkDisplayClass
{
  ALTK_DISPLAY_CLASS_HEADER;
};



AltkDisplay *altk_display_new ( void );
void altk_display_open ( AltkDisplay *display );
gboolean altk_display_is_open ( AltkDisplay *display );
void altk_display_get_size ( AltkDisplay *display,
                             gint *width,
                             gint *height );
struct _AltkWindow *altk_display_get_root_window ( AltkDisplay *display );
void altk_display_flip ( AltkDisplay *display );
ALLEGRO_BITMAP *altk_display_get_backbuf ( AltkDisplay *display );
void altk_display_invalidate_region ( AltkDisplay *display,
                                      AltkRegion *area );



#endif
