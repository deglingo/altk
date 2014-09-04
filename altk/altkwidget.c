/* altkwidget.c -
 */

#include "altk/private.h"
#include "altk/altkwidget.h"
#include "altk/altkstyle.h"
#include "altk/altkgc.h"
#include "altk/altkdisplay.h"
#include "altk/altkwidget.inl"



static AltkDisplay *_on_get_display ( AltkWidget *widget );
static void _on_map ( AltkWidget *widget,
                      AltkDisplay *display );
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc );
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event );



/* altk_widget_class_init:
 */
static void altk_widget_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->get_display = _on_get_display;
  ((AltkWidgetClass *) cls)->map = _on_map;
  ((AltkWidgetClass *) cls)->size_allocate = _on_size_allocate;
  ((AltkWidgetClass *) cls)->expose_event = _on_expose_event;
}



/* _altk_widget_set_parent:
 */
void _altk_widget_set_parent ( AltkWidget *widget,
                               AltkWidget *parent )
{
  ASSERT(!widget->parent);
  widget->parent = parent;
  /* [TODO] map, resize, queue_draw, realize... */
}



/* _on_get_display:
 */
static AltkDisplay *_on_get_display ( AltkWidget *widget )
{
  while (widget) {
    if (ALTK_WIDGET_TOP_WIDGET(widget))
      return altk_widget_get_display(widget);
    widget = widget->parent;
  }
  return NULL;
}



/* altk_widget_get_display:
 */
struct _AltkDisplay *altk_widget_get_display ( AltkWidget *widget )
{
  return ALTK_WIDGET_GET_CLASS(widget)->get_display(widget);
}



/* altk_widget_map:
 */
void altk_widget_map ( AltkWidget *widget,
                       struct _AltkDisplay *display )
{
  ALTK_WIDGET_GET_CLASS(widget)->map(widget, display);
}



/* _on_map:
 */
static void _on_map ( AltkWidget *widget,
                      AltkDisplay *display )
{
  /* [REMOVEME] */
  /* gint s; */
  /* widget->display = display; */
  /* widget->style = altk_style_new(); */
  /* for (s = 0; s < ALTK_STATE_COUNT; s++) { */
  /*   widget->gc[s] = altk_gc_new(); */
  /* } */
  /* /\* [REMOVEME] *\/ */
  /* widget->flags |= ALTK_WIDGET_FLAG_NEEDS_RESIZE; */
}



/* _realize_child:
 */
static gboolean _realize_child ( AltkWidget *widget,
                                 gpointer data )
{
  ASSERT(!ALTK_WIDGET_REALIZED(widget));
  ASSERT(ALTK_WIDGET_TOP_WIDGET(widget) ||
         (widget->parent && ALTK_WIDGET_REALIZED(widget->parent)));
  ASSERT(altk_display_is_open(altk_widget_get_display(widget)));
  CL_DEBUG("[TODO] widget_realize(%p)", widget);
  /* Widget.realize(widget) */
  widget->flags |= ALTK_WIDGET_FLAG_REALIZED; /* [removeme] ?? */
  altk_widget_foreach(widget, (AltkForeachFunc) _realize_child, NULL);
  return ALTK_FOREACH_CONT;
}



/* altk_widget_realize:
 */
void altk_widget_realize ( AltkWidget *widget )
{
  _realize_child(widget, NULL);
}



/* altk_widget_size_request:
 */
void altk_widget_size_request ( AltkWidget *widget,
                                AltkRequisition *req )
{
  if (widget->flags & ALTK_WIDGET_FLAG_NEEDS_RESIZE)
    {
      ALTK_WIDGET_GET_CLASS(widget)->size_request(widget, &widget->size_request);
      widget->flags &= ~ALTK_WIDGET_FLAG_NEEDS_RESIZE;
    }
  *req = widget->size_request;
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
}



/* altk_widget_show:
 */
void altk_widget_show ( AltkWidget *widget )
{
  if (ALTK_WIDGET_VISIBLE(widget))
    return;
  widget->flags |= ALTK_WIDGET_FLAG_VISIBLE;
  CL_DEBUG("[TODO] widget_show(%p)", widget);
}



/* _show_all:
 */
static gboolean _show_all ( AltkWidget *widget,
                            gpointer data )
{
  /* show children first */
  altk_widget_foreach(widget, (AltkForeachFunc) _show_all, NULL);
  altk_widget_show(widget);
  return ALTK_FOREACH_CONT;
}



/* altk_widget_show_all:
 */
void altk_widget_show_all ( AltkWidget *widget )
{
  _show_all(widget, NULL);
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
  case ALTK_EVENT_EXPOSE_BACKGROUND:
    ALTK_WIDGET_GET_CLASS(widget)->expose_background_event(widget, event);
    break;
  case ALTK_EVENT_EXPOSE:
    ALTK_WIDGET_GET_CLASS(widget)->expose_event(widget, event);
    break;
  case ALTK_EVENT_MOUSE_ENTER:
    ALTK_WIDGET_GET_CLASS(widget)->mouse_enter_event(widget, event);
    break;
  case ALTK_EVENT_MOUSE_LEAVE:
    ALTK_WIDGET_GET_CLASS(widget)->mouse_leave_event(widget, event);
    break;
  case ALTK_EVENT_MOUSE_BUTTON_DOWN:
    ALTK_WIDGET_GET_CLASS(widget)->mouse_button_down_event(widget, event);
    break;
  case ALTK_EVENT_MOUSE_BUTTON_UP:
    ALTK_WIDGET_GET_CLASS(widget)->mouse_button_up_event(widget, event);
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



/* altk_widget_foreach:
 */
void altk_widget_foreach ( AltkWidget *widget,
                           AltkForeachFunc func,
                           gpointer data )
{
  if (ALTK_WIDGET_GET_CLASS(widget)->foreach) {
    ALTK_WIDGET_GET_CLASS(widget)->foreach(widget, func, data);
  }
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



/* altk_widget_set_event_mask:
 */
void altk_widget_set_event_mask ( AltkWidget *widget,
                                  AltkEventType mask )
{
  widget->event_mask = mask;
}



/* altk_widget_get_event_mask:
 */
AltkEventType altk_widget_get_event_mask ( AltkWidget *widget )
{
  return widget->event_mask;
}



/* altk_widget_set_state:
 */
void altk_widget_set_state ( AltkWidget *widget,
                             AltkState state )
{
  if (widget->state == state)
    return;
  widget->state = state;
  altk_widget_queue_draw(widget, TRUE);
}



/* altk_widget_queue_draw:
 */
void altk_widget_queue_draw ( AltkWidget *widget,
                              gboolean children )
{
  CL_DEBUG("[TODO] widget_queue_draw(%p)", widget);
  /* AltkDisplay *display; */
  /* AltkRegion *area; */
  /* display = altk_widget_get_display(widget); */
  /* ASSERT(display); /\* [FIXME] *\/ */
  /* area = altk_widget_get_visible_area(widget, !children); */
  /* altk_display_queue_draw(display, area); */
  /* altk_region_destroy(area); */
}



/* altk_widget_queue_resize:
 */
void altk_widget_queue_resize ( AltkWidget *widget )
{
  CL_DEBUG("[TODO] widget_queue_resize(%p)", widget);
  /* AltkWidget *w = widget; */
  /* while (TRUE) */
  /*   { */
  /*     w->flags |= ALTK_WIDGET_FLAG_NEEDS_RESIZE; */
  /*     if (w->parent) { */
  /*       w = w->parent; */
  /*     } else { */
  /*       AltkDisplay *display = altk_widget_get_display(w); */
  /*       if (display) */
  /*         altk_display_queue_resize(display, w); */
  /*       break; */
  /*     } */
  /*   } */
}



/* altk_widget_get_root_coords:
 */
void altk_widget_get_root_coords ( AltkWidget *widget,
                                   gint *root_x,
                                   gint *root_y )
{
  AltkWidget *w;
  *root_x = *root_y = 0;
  for (w = widget; w; w = w->parent) {
    *root_x += w->x;
    *root_y += w->y;
  }
}
