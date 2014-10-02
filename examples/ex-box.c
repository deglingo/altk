/* ex_box.c -
 */

#include <clog.h>
#include "altk/altk.h"



/* main:
 */
gint main ()
{
  AltkDisplay *display;
  AltkWidget *dlg, *top, *lbl1, *lbl2;
  CL_DEBUG("HELLO!");
  altk_init();
  display = altk_display_new();
  dlg = altk_dialog_new(display);
  top = altk_box_new();
  altk_container_add(ALTK_CONTAINER(dlg), top);
  lbl1 = altk_button_new_with_label("Hello 1!");
  altk_box_pack_start(ALTK_BOX(top), lbl1, 0);
  lbl2 = altk_button_new_with_label("Hello 2!");
  altk_box_pack_start(ALTK_BOX(top), lbl2, 0);
  altk_widget_show_all(dlg);
  altk_display_open(display);
  altk_main();
  return 0;
}
