/* ex_label.c -
 */

#include <clog.h>
#include "altk/altk.h"



/* main:
 */
gint main ()
{
  AltkDisplay *display;
  AltkWidget *dlg, *lbl;
  CL_DEBUG("HELLO!");
  altk_init();
  display = altk_display_new();
  dlg = altk_dialog_new(display);
  lbl = altk_label_new("Hello!");
  altk_container_add(ALTK_CONTAINER(dlg), lbl);
  altk_widget_show_all(dlg);
  altk_display_open(display);
  altk_main();
  return 0;
}
