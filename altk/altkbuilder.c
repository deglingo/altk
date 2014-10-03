/* altkbuilder.c -
 */

#include "altk/altkbuilder.h"
#include "altk/altkbuilder.inl"



/* altk_builder_new:
 */
AltkBuilder *altk_builder_new ( void )
{
  AltkBuilder *b = ALTK_BUILDER(l_object_new(ALTK_CLASS_BUILDER, NULL));
  return b;
}



/* altk_builder_parse_text:
 */
gboolean altk_builder_parse_text ( AltkBuilder *builder,
                                   const gchar *text,
                                   GError **error )
{
  return TRUE;
}



/* [REMOVEME] */
#include "altk/altkdialog.h"

/* altk_builder_get_object:
 */
LObject *altk_builder_get_object ( AltkBuilder *builder,
                                   const gchar *name )
{
  return L_OBJECT(altk_dialog_new(NULL));
}
