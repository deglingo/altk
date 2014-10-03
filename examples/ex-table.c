/* ex_table.c -
 */

#include <clog.h>
#include "altk/altk.h"



/* create_dialog:
 */
static AltkWidget *create_dialog ( AltkDisplay *display )
{
  AltkWidget *dlg, *table, *title;
  l_trash_push();
  dlg = altk_dialog_new(display);
  /* table */
  table = L_TRASH_OBJECT
    (altk_table_new());
  altk_container_add(ALTK_CONTAINER(dlg), table);
  /* title */
  title = L_TRASH_OBJECT
    (altk_label_new("TABLE EXAMPLE"));
  altk_table_attach(ALTK_TABLE(table), title, 0, 0, 4, 1, 0);
  /* cleanup */
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
