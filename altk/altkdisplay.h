/* altkdisplay.h -
 */

#ifndef _ALTKDISPLAY_H_
#define _ALTKDISPLAY_H_

#include "altk/altkbase.h"
#include "altk/altkdisplay-def.h"



struct _AltkWidget;



/* AltkDisplay:
 */
struct _AltkDisplay
{
  ALTK_DISPLAY_INSTANCE_HEADER;
  /* the underlying allegro display */
  ALLEGRO_DISPLAY *al_display;
  /* top-widgets attached to this display */
  GList *top_widgets;
};



/* AltkDisplayClass:
 */
struct _AltkDisplayClass
{
  ALTK_DISPLAY_CLASS_HEADER;
};



AltkDisplay *altk_display_new ( void );
AltkDisplay *altk_display_from_al_display ( ALLEGRO_DISPLAY *al_display );
void altk_display_open ( AltkDisplay *display );
void altk_display_attach_widget ( AltkDisplay *display,
                                  struct _AltkWidget *widget );



#endif
