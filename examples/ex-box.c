/* ex_box.c -
 */

#include <clog.h>
#include "altk/altk.h"



/* main:
 */
gint main ()
{
  AltkDisplay *display;
  AltkWidget *dlg, *top, *lbl;
  CL_DEBUG("HELLO!");
  altk_init();
  display = altk_display_new();
  dlg = altk_dialog_new(display);
  top = altk_box_new();
  altk_container_add(ALTK_CONTAINER(dlg), top);
  lbl = altk_label_new("Hello!");
  altk_box_pack_start(ALTK_BOX(top), lbl, 0);
  altk_widget_show_all(dlg);
  altk_display_open(display);
  altk_main();
  return 0;
}
