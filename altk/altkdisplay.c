/* altkdisplay.c -
 */

#include "altk/altkdisplay.h"
#include "altk/altkmain.h"

#include "altk/altkdisplay.inl"



/* altk_display_new:
 */
AltkDisplay *altk_display_new ( void )
{
  AltkDisplay *display;
  display = ALTK_DISPLAY(l_object_new(ALTK_CLASS_DISPLAY, NULL));
  return display;
}



/* altk_display_open:
 */
void altk_display_open ( AltkDisplay *display )
{
  display->al_display = al_create_display(640, 480);
  altk_main_register_al_source(al_get_display_event_source(display->al_display));
}



/* altk_display_attach_widget:
 */
void altk_display_attach_widget ( AltkDisplay *display,
                                  struct _AltkWidget *widget )
{
}
