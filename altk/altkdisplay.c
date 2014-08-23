/* altkdisplay.c -
 */

#include "altk/altkdisplay.h"

#include "altk/altkdisplay.inl"



/* altk_display_new:
 */
AltkDisplay *altk_display_new ( void )
{
  return ALTK_DISPLAY(l_object_new(ALTK_CLASS_DISPLAY, NULL));
}



/* altk_display_open:
 */
void altk_display_open ( AltkDisplay *display )
{
}



/* altk_display_attach_widget:
 */
void altk_display_attach_widget ( AltkDisplay *display,
                                  struct _AltkWidget *widget )
{
}
