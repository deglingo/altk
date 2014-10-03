/* ex_label.c -
 */

#include "altk/private.h"
#include "altk/altk.h"



extern const char EX_BUILDER_DIALOG[];



/* create_dialog:
 */
static AltkWidget *create_dialog ( AltkDisplay *display )
{
  AltkBuilder *builder;
  GError *error = NULL;
  builder = altk_builder_new();
  CL_DEBUG("parsing xml:\n%s", EX_BUILDER_DIALOG);
  if (!altk_builder_parse_text(builder, EX_BUILDER_DIALOG, &error))
    CL_ERROR("parse error");
  ASSERT(!error);
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
