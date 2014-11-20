/* ex-button.c -
 */

#include <clog.h>
#include "altk/altk.h"



/* _on_click:
 */
static void _on_click ( AltkWidget *button )
{
  CL_DEBUG("click!");
}



/* main:
 */
gint main ()
{
  AltkDisplay *display;
  AltkWidget *dlg, *but;
  CL_DEBUG("HELLO!");
  altk_init();
  display = altk_display_new();
  dlg = altk_dialog_new(display);
  but = altk_button_new_with_label("Hello!");
  altk_container_add(ALTK_CONTAINER(dlg), but);
  l_signal_connect(L_OBJECT(but),
                   "clicked",
                   (LSignalHandler) _on_click,
                   NULL, NULL);
  altk_widget_show_all(dlg);
  altk_display_open(display);
  altk_main();
  return 0;
}
