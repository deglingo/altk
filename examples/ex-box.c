/* ex_box.c -
 */

#include <clog.h>
#include "altk/altk.h"



/* create_dialog:
 */
static AltkWidget *create_dialog ( AltkDisplay *display )
{
  AltkWidget *dlg, *top;
  gint c, r;
  l_trash_push();
  /* dialog */
  dlg = altk_dialog_new(display);
  /* top box */
  top = L_TRASH_OBJECT(altk_box_new(ALTK_HORIZONTAL));
  altk_container_add(ALTK_CONTAINER(dlg), top);
  /* columns */
  for (c = 0; c < 2; c++) {
    AltkWidget *col = L_TRASH_OBJECT(altk_box_new(ALTK_VERTICAL));
    altk_box_pack_start(ALTK_BOX(top), col, 0);
    /* rows */
    for (r = 0; r < 5; r++) {
      gchar *text = g_strdup_printf("COL %d, ROW %d", c, r);
      AltkWidget *lbl = L_TRASH_OBJECT
        (altk_button_new_with_label(text));
      altk_box_pack_start(ALTK_BOX(col), lbl, 0);
      g_free(text);
    }
  }
  l_trash_pop();
  return dlg;
}



/* main:
 */
gint main ()
{
  AltkDisplay *display;
  AltkWidget *dlg;
  CL_DEBUG("HELLO!");
  altk_init();
  display = altk_display_new();
  dlg = create_dialog(display);
  altk_widget_show_all(dlg);
  altk_display_open(display);
  altk_main();
  return 0;
}
