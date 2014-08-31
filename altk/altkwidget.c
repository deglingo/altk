/* altkwidget.c -
 */

#include "altk/private.h"
#include "altk/altkwidget.h"
#include "altk/altkstyle.h"
#include "altk/altkgc.h"
#include "altk/altkwidget.inl"



static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc );
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event );



/* altk_widget_class_init:
 */
static void altk_widget_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->size_allocate = _on_size_allocate;
  ((AltkWidgetClass *) cls)->expose_event = _on_expose_event;
}



/* _altk_widget_set_parent:
 */
void _altk_widget_set_parent ( AltkWidget *widget,
                               AltkWidget *parent )
{
  /* [FIXME] use a weakref ? */
  ASSERT(!widget->parent);
  widget->parent = parent;
}



/* altk_widget_map:
 */
void altk_widget_map ( AltkWidget *widget,
                       struct _AltkDisplay *display )
{
  gint s;
  widget->style = altk_style_new();
  for (s = 0; s < ALTK_STATE_COUNT; s++) {
    widget->gc[s] = altk_gc_new();
  }
}



/* altk_widget_size_request:
 */
void altk_widget_size_request ( AltkWidget *widget,
                                AltkRequisition *req )
{
  ALTK_WIDGET_GET_CLASS(widget)->size_request(widget, req);
  CL_DEBUG("size_request(%p) -> %d, %d",
           widget, req->width, req->height);
}



/* altk_widget_size_allocate:
 */
void altk_widget_size_allocate ( AltkWidget *widget,
                                 AltkAllocation *alloc )
{
  CL_DEBUG("size_allocate(%p, %d, %d, %d, %d)",
           widget, alloc->x, alloc->y, alloc->width, alloc->height);
  ALTK_WIDGET_GET_CLASS(widget)->size_allocate(widget, alloc);
}



/* _on_size_allocate:
 */
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc )
{
  wid->x = alloc->x;
  wid->y = alloc->y;
  wid->width = alloc->width;
  wid->height = alloc->height;
  /* update root_x/y */
  if (wid->parent) {
    wid->root_x = wid->parent->root_x + alloc->x;
    wid->root_y = wid->parent->root_y + alloc->y;
  } else {
    wid->root_x = alloc->x;
    wid->root_y = alloc->y;
  }
}



/* altk_widget_get_shape:
 */
AltkRegion *altk_widget_get_shape ( AltkWidget *widget )
{
  AltkRectangle r;
  r.x = r.y = 0;
  r.width = widget->width;
  r.height = widget->height;
  return altk_region_rectangle(&r);
}



/* altk_widget_event:
 */
void altk_widget_event ( AltkWidget *widget,
                         AltkEvent *event )
{
  /* [FIXME] virtual func ? */
  switch (event->type) {
  case ALTK_EVENT_EXPOSE:
    ALTK_WIDGET_GET_CLASS(widget)->expose_event(widget, event);
    break;
  default:
    CL_DEBUG("[TODO] event %d", event->type);
  }
}



/* _on_expose_event:
 */
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event )
{
  CL_DEBUG("[TODO] not implemented: AltkWidget.expose_event()");
}



/* altk_widget_forall:
 */
void altk_widget_forall ( AltkWidget *widget,
                          AltkForeachFunc func,
                          gpointer data )
{
  if (ALTK_WIDGET_GET_CLASS(widget)->forall) {
    ALTK_WIDGET_GET_CLASS(widget)->forall(widget, func, data);
  }
}
