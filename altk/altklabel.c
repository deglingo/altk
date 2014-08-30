/* altklabel.c -
 */

#include "altk/private.h"
#include "altk/altklabel.h"

#include "altk/altklabel.inl"



static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req );



/* altk_label_class_init:
 */
static void altk_label_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->size_request = _on_size_request;
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



/* _on_size_request:
 */
static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req )
{
  /* [FIXME] */
  req->width = 64;
  req->height = 12;
}
