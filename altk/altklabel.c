/* altklabel.c -
 */

#include "altk/private.h"
#include "altk/altklabel.h"
#include "altk/altkstyle.h"

#include "altk/altklabel.inl"



static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req );
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event );



/* altk_label_class_init:
 */
static void altk_label_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->size_request = _on_size_request;
  ((AltkWidgetClass *) cls)->expose_event = _on_expose_event;
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



/* _on_expose_event:
 */
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event )
{
  altk_style_draw_text(wid->style,
                       event->expose.window,
                       wid->gc[ALTK_STATE_NORMAL/*[fixme]*/],
                       0, 0, ALTK_LABEL(wid)->text);
}
