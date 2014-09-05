/* altkwidget.c -
 */

#include "altk/private.h"
#include "altk/altkwidget.h"
#include "altk/altkstyle.h"
#include "altk/altkgc.h"
#include "altk/altkdisplay.h"
#include "altk/altkwindow.h"
#include "altk/altkwidget.inl"



static AltkDisplay *_on_get_display ( AltkWidget *widget );
static void _on_map ( AltkWidget *widget );
static void _on_realize ( AltkWidget *widget );
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc );
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event );



/* Globals */
static GSList *resize_queue = NULL;
static guint resize_source_id = 0;



/* altk_widget_event_handler:
 */
void altk_widget_event_handler ( AltkEvent *event,
                                 gpointer data )
{
  /* [FIXME] user_data is NULL for the root window */
  if (event->any.window->user_data)
    altk_widget_event(ALTK_WIDGET(event->any.window->user_data), event);
}



/* altk_widget_class_init:
 */
static void altk_widget_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->get_display = _on_get_display;
  ((AltkWidgetClass *) cls)->map = _on_map;
  ((AltkWidgetClass *) cls)->realize = _on_realize;
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
  altk_widget_queue_resize(widget);
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



/* _map_recursive:
 */
static gboolean _map_recursive ( AltkWidget *widget,
                                 gpointer data )
{
  ASSERT(!ALTK_WIDGET_MAPPED(widget));
  ASSERT(ALTK_WIDGET_TOP_WIDGET(widget) ||
         (widget->parent && ALTK_WIDGET_MAPPED(widget->parent)));
  ASSERT(altk_display_is_open(altk_widget_get_display(widget)));

  if (ALTK_WIDGET_VISIBLE(widget))
    {
      ALTK_WIDGET_GET_CLASS(widget)->map(widget);
      widget->flags |= ALTK_WIDGET_FLAG_MAPPED;
      altk_widget_foreach(widget, _map_recursive, NULL);
    }
  return ALTK_FOREACH_CONT;
}



/* altk_widget_map:
 */
void altk_widget_map ( AltkWidget *widget )
{

  _map_recursive(widget, NULL);
}



/* _on_map:
 */
static void _on_map ( AltkWidget *widget )
{
  CL_DEBUG("[TODO] widget_map(%p)", widget);
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
  ALTK_WIDGET_GET_CLASS(widget)->realize(widget);
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



/* _on_realize:
 */
static void _on_realize ( AltkWidget *widget )
{
  CL_DEBUG("[TODO] realize(%p)", widget);
  if (ALTK_WIDGET_NOWINDOW(widget))
    {
      widget->window = widget->parent->window;
    }
  else
    {
      AltkWindow *parent_window;
      if (ALTK_WIDGET_TOP_WIDGET(widget)) {
        parent_window = altk_display_get_root_window(altk_widget_get_display(widget));
      } else {
        parent_window = widget->parent->window;
      }
      widget->window = altk_window_new(parent_window,
                                       widget->x,
                                       widget->y,
                                       widget->width,
                                       widget->height);
      widget->window->user_data = widget;
    }
}



/* altk_widget_size_request:
 */
void altk_widget_size_request ( AltkWidget *widget,
                                AltkRequisition *req )
{
  ASSERT(ALTK_WIDGET_GET_CLASS(widget)->size_request);
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
  ASSERT(ALTK_WIDGET_GET_CLASS(widget)->size_allocate);
  CL_DEBUG("size_allocate(%p, %d, %d, %d, %d)",
           widget, alloc->x, alloc->y, alloc->width, alloc->height);
  ALTK_WIDGET_GET_CLASS(widget)->size_allocate(widget, alloc);
  CL_DEBUG("size_allocate(%p) OK", widget);
}



/* _on_size_allocate:
 */
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc )
{
  CL_DEBUG("_on_size_allocate(%p)", wid);
  wid->x = alloc->x;
  wid->y = alloc->y;
  wid->width = alloc->width;
  wid->height = alloc->height;
  CL_DEBUG("_on_size_allocate OK");
}



/* altk_widget_show:
 */
void altk_widget_show ( AltkWidget *widget )
{
  if (ALTK_WIDGET_VISIBLE(widget))
    return;
  widget->flags |= ALTK_WIDGET_FLAG_VISIBLE;
  /* [FIXME] */
  CL_DEBUG("[TODO] widget_show(%p)", widget);
  altk_widget_queue_resize(widget);
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



/* _event_expose:
 */
static gboolean _event_expose ( AltkWidget *widget,
                                AltkEvent *event )
{
  /* stop traversal for widget having a window */
  if ((gpointer) widget != event->expose.window->user_data
      && !ALTK_WIDGET_NOWINDOW(widget))
    return ALTK_FOREACH_CONT;
  /* process the event */
  /* [FIXME] call altk_widget_event() for each child ? */
  ALTK_WIDGET_GET_CLASS(widget)->expose_event(widget, event);
  /* process children */
  altk_widget_forall(widget, (AltkForeachFunc) _event_expose, event);
  return ALTK_FOREACH_CONT;
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
    _event_expose(widget, event);
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



/* _negotiate_size:
 */
static void _negotiate_size ( AltkWidget *wid )
{
  AltkRequisition req = { 0, 0 };
  AltkAllocation alloc;
  altk_widget_size_request(wid, &req);
  alloc.x = 0;
  alloc.y = 0;
  alloc.width = req.width;
  alloc.height = req.height;
  altk_widget_size_allocate(wid, &alloc);
}



/* _idle_resize:
 */
static gboolean _idle_resize ( gpointer data )
{
  GSList *queue, *q;
  /* [FIXME] forbid resize_queue() during process */
  queue = resize_queue;
  resize_queue = NULL;
  for (q = queue; q; q = q->next)
    {
      _negotiate_size(ALTK_WIDGET(q->data));
    }
  g_slist_free(queue); /* [FIXME] free_full(l_object_unref) */
  /* remove the idle source */
  resize_source_id = 0;
  return G_SOURCE_REMOVE;
}



/* altk_widget_queue_resize:
 */
void altk_widget_queue_resize ( AltkWidget *widget )
{
  CL_DEBUG("widget_queue_resize(%p)", widget);
  AltkWidget *w = widget;
  while (w)
    {
      if (!ALTK_WIDGET_VISIBLE(w))
        break;
      /* [FIXME] can't we just stop here if NEEDS_RESIZE is already
         set ? */
      w->flags |= ALTK_WIDGET_FLAG_NEEDS_RESIZE;
      if (ALTK_WIDGET_TOP_WIDGET(w))
        {
          if (altk_display_is_open(altk_widget_get_display(w)))
            {
              if (!g_slist_find(resize_queue, w))
                resize_queue = g_slist_prepend(resize_queue, l_object_ref(w));
              if (resize_source_id == 0)
                resize_source_id = g_idle_add_full(ALTK_PRIORITY_RESIZE,
                                                   (GSourceFunc) _idle_resize,
                                                   NULL,
                                                   NULL);
              break;
            }
        }
      w = w->parent;
    }
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
