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
  LObject *dlg;
  GError *error = NULL;
  builder = altk_builder_new();
  /* CL_DEBUG("parsing xml:\n%s", EX_BUILDER_DIALOG); */
  if (!altk_builder_parse_text(builder,
                               EX_BUILDER_DIALOG,
                               strlen(EX_BUILDER_DIALOG), /* [FIXME] */
                               &error))
    CL_ERROR("parse error");
  ASSERT(!error);
  if (!(dlg = altk_builder_get_object(builder, "ex-builder-dialog")))
    CL_ERROR("object not found");
  ASSERT(ALTK_IS_WIDGET(dlg));
  l_object_ref(dlg);
  l_object_unref(builder);
  altk_dialog_set_display(ALTK_DIALOG(dlg), display);
  return ALTK_WIDGET(dlg);
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
