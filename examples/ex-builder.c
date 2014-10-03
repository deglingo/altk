/* ex_label.c -
 */

#include <clog.h>
#include "altk/altk.h"



extern const char EX_BUILDER_DIALOG[];



/* create_dialog:
 */
static AltkWidget *create_dialog ( AltkDisplay *display )
{
  AltkBuilder *builder;
  builder = altk_builder_new();
  CL_DEBUG("parsing xml:\n%s", EX_BUILDER_DIALOG);
  l_object_unref(builder);
  return altk_dialog_new(display);
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
