/* altklabel.c -
 */

#include "altk/private.h"
#include "altk/altklabel.h"

#include "altk/altklabel.inl"



/* altk_label_class_init:
 */
static void altk_label_class_init ( LObjectClass *cls )
{
}



/* altk_label_new:
 */
AltkWidget *altk_label_new ( const gchar *text )
{
  AltkWidget *wid = ALTK_WIDGET(l_object_new(ALTK_CLASS_LABEL, NULL));
  /* [FIXME] instance init */
  ALTK_LABEL(wid)->text = g_strdup(text);
  return wid;
}
