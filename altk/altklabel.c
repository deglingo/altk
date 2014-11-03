/* altklabel.c -
 */

#include "altk/private.h"
#include "altk/altklabel.h"
#include "altk/altkstyle.h"
#include "altk/altkgc.h"
#include "altk/altkfont.h"
#include "altk/altkdrawable.h"

#include "altk/altklabel.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_TEXT,
    _PROP_COUNT,
  };

static LParamSpec *pspecs[_PROP_COUNT] = { NULL, };



static LObject *_get_property ( LObject *object,
                                LParamSpec *pspec );
static void _set_property ( LObject *object,
                            LParamSpec *pspec,
                            LObject *value );
static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req );
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event );



/* altk_label_class_init:
 */
static void altk_label_class_init ( LObjectClass *cls )
{
  cls->get_property = _get_property;
  cls->set_property = _set_property;
  ((AltkWidgetClass *) cls)->size_request = _on_size_request;
  ((AltkWidgetClass *) cls)->expose_event = _on_expose_event;

  pspecs[PROP_TEXT] =
    l_param_spec_string("text",
                        "");

  l_object_class_install_properties(cls, _PROP_COUNT, pspecs);
}



/* altk_label_init:
 */
static void altk_label_init ( LObject *obj )
{
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
  altk_widget_set_event_mask(ALTK_WIDGET(obj), ALTK_EVENT_MASK_EXPOSE);
  ALTK_LABEL(obj)->text = g_strdup("??"); /* [fixme] waiing for props */
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



/* _get_property:
 */
static LObject *_get_property ( LObject *object,
                                LParamSpec *pspec )
{
  switch (pspec->param_id)
    {
    case PROP_TEXT:
      return L_OBJECT(l_string_new(ALTK_LABEL(object)->text));
    default:
      ASSERT(0);
      return NULL;
    }
}



/* _set_property:
 */
static void _set_property ( LObject *object,
                            LParamSpec *pspec,
                            LObject *value )
{
  switch (pspec->param_id)
    {
    case PROP_TEXT:
      ASSERT(L_IS_STRING(value)); /* [removeme] */
      altk_label_set_text(ALTK_LABEL(object),
                          L_STRING(value)->str);
      break;
    default:
      ASSERT(0);
    }
}



/* altk_label_set_text:
 */
void altk_label_set_text ( AltkLabel *label,
                           const gchar *text )
{
  g_free(label->text);
  label->text = g_strdup(text);
  altk_widget_queue_resize(ALTK_WIDGET(label));
  altk_widget_queue_draw(ALTK_WIDGET(label)); /* [fixme] ?? */
}



/* _on_size_request:
 */
static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req )
{
  gint bx, by, bw, bh;
  altk_font_get_text_size(altk_style_context_get_font(wid->style_context),
                          ALTK_LABEL(wid)->text,
                          &bx, &by, &bw, &bh);
  req->width = bw;
  req->height = bh;
}



/* _on_expose_event:
 */
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event )
{
  altk_style_context_draw_text(wid->style_context,
                               ALTK_DRAWABLE(event->expose.window),
                               wid->x,
                               wid->y,
                               ALTK_LABEL(wid)->text);
}
