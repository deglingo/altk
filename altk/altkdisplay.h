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
  /* [FIXME] remove ? - the display back-buffer */
  AltkDrawable *backbuf;
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
void altk_display_attach_widget ( AltkDisplay *display,
                                  struct _AltkWidget *widget );



#endif
