/* altkwidget.c -
 */

#include "altk/private.h"
#include "altk/altkwidget.h"
#include "altk/altkstyle.h"
#include "altk/altkgc.h"
#include "altk/altkdisplay.h"
#include "altk/altkwindow.h"
#include "altk/altkwm.h"
#include "altk/altkcontainer.h"
#include "altk/altkwidget.inl"






/* Properties:
 */
enum
  {
    PROP_0,
    PROP_NAME,
    _PROP_COUNT,
  };

static LParamSpec *pspecs[_PROP_COUNT] = { NULL, };



static LObject *_get_property ( LObject *object,
                                LParamSpec *pspec );
static void _set_property ( LObject *object,
                            LParamSpec *pspec,
                            LObject *value );
static AltkDisplay *_on_get_display ( AltkWidget *widget );
static void _on_map ( AltkWidget *widget );
static void _on_realize ( AltkWidget *widget );
static void _on_unrealize ( AltkWidget *widget );
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



/* altk_widget_init:
 */
static void altk_widget_init ( LObject *obj )
{
  /* [fixme] prop default */
  ALTK_WIDGET(obj)->name = g_strdup("");
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_ENABLE_SHOW_ALL;
  /* /\* debug *\/ */
  /* l_object_trace_ref(obj, TRUE); */
}



/* altk_widget_class_init:
 */
static void altk_widget_class_init ( LObjectClass *cls )
{
  cls->get_property = _get_property;
  cls->set_property = _set_property;
  ((AltkWidgetClass *) cls)->get_display = _on_get_display;
  ((AltkWidgetClass *) cls)->map = _on_map;
  ((AltkWidgetClass *) cls)->realize = _on_realize;
  ((AltkWidgetClass *) cls)->unrealize = _on_unrealize;
  ((AltkWidgetClass *) cls)->size_allocate = _on_size_allocate;
  ((AltkWidgetClass *) cls)->expose_event = _on_expose_event;

  pspecs[PROP_NAME] =
    l_param_spec_string("name",
                        "");

  l_object_class_install_properties(cls, _PROP_COUNT, pspecs);
}



/* _get_property:
 */
static LObject *_get_property ( LObject *object,
                                LParamSpec *pspec )
{
  switch (pspec->param_id)
    {
    case PROP_NAME:
      return L_OBJECT(l_string_new(ALTK_WIDGET(object)->name));
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
    case PROP_NAME:
      ASSERT(L_IS_STRING(value)); /* [removeme] */
      altk_widget_set_name(ALTK_WIDGET(object),
                           L_STRING(value)->str);
      break;
    default:
      ASSERT(0);
    }
}



/* _altk_widget_set_parent:
 */
void _altk_widget_set_parent ( AltkWidget *widget,
                               AltkWidget *parent )
{
  ASSERT(parent);
  ASSERT(ALTK_IS_CONTAINER(parent));
  ASSERT(!widget->parent);
  ASSERT(!widget->next);
  widget->parent = parent;
  if (parent->children)
    parent->children->prev = widget;
  widget->next = parent->children;
  parent->children = widget;
  if (ALTK_WIDGET_MAPPED(parent))
    {
      altk_widget_map(widget);
    }
  if (ALTK_WIDGET_VISIBLE(widget))
    {
      altk_widget_queue_resize(widget);
      if (ALTK_WIDGET_REALIZED(parent))
        {
          altk_widget_realize(widget);
          altk_widget_queue_draw(widget);
        }
    }
}



/* _altk_widget_unset_parent:
 */
void _altk_widget_unset_parent ( AltkWidget *widget )
{
  ASSERT(widget->parent);
  if (ALTK_WIDGET_REALIZED(widget))
    altk_widget_unrealize(widget);
  if (ALTK_WIDGET_MAPPED(widget))
    altk_widget_unmap(widget);
  /* remove from parent list */
  if (widget->prev)
    widget->prev->next = widget->next;
  if (widget->next)
    widget->next->prev = widget->prev;
  if (widget->parent->children == widget)
    widget->parent->children = widget->next;
  altk_widget_queue_resize(widget->parent);
  altk_widget_queue_draw(widget->parent);
  widget->parent = widget->prev = widget->next = NULL;
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



/* altk_widget_destroy:
 */
void altk_widget_destroy ( AltkWidget *widget )
{
  AltkWidget *child;
  l_object_ref(widget);
  if (ALTK_WIDGET_DESTROYED(widget))
    return;
  for (child = widget->children; child; child = child->next)
    altk_widget_destroy(child);
  if (widget->parent)
    altk_container_remove(ALTK_CONTAINER(widget->parent), widget);
  widget->flags |= ALTK_WIDGET_FLAG_DESTROYED;
  l_object_dispose(L_OBJECT(widget));
  l_object_unref(widget);
}



/* _map_recursive:
 */
static void _map_recursive ( AltkWidget *widget )
{
  AltkWidget *child;
  ASSERT(!ALTK_WIDGET_MAPPED(widget));
  ASSERT(ALTK_WIDGET_TOP_WIDGET(widget) ||
         (widget->parent && ALTK_WIDGET_MAPPED(widget->parent)));
  ASSERT(altk_display_is_open(altk_widget_get_display(widget)));

  /* if (ALTK_WIDGET_VISIBLE(widget)) */
  /*   { */
      ALTK_WIDGET_GET_CLASS(widget)->map(widget);
      widget->flags |= ALTK_WIDGET_FLAG_MAPPED;
      for (child = widget->children; child; child = child->next)
        _map_recursive(child);
    /* } */
}



/* altk_widget_map:
 */
void altk_widget_map ( AltkWidget *widget )
{

  _map_recursive(widget);
}



/* _on_map:
 */
static void _on_map ( AltkWidget *widget )
{
  /* CL_DEBUG("widget_map(%p)", widget); */
  widget->style = altk_style_new();
  widget->style_context = altk_style_context_new(widget->style);
}



/* _unmap_recursive:
 */
static void _unmap_recursive ( AltkWidget *widget )
{
  AltkWidget *child;
  ASSERT(ALTK_WIDGET_MAPPED(widget));
  /* CL_DEBUG("[TODO] widget unmap (%p)", widget); */
  for (child = widget->children; child; child = child->next)
    _unmap_recursive(child);
  L_OBJECT_CLEAR(widget->style);
  L_OBJECT_CLEAR(widget->style_context);
  widget->flags &= ~ALTK_WIDGET_FLAG_MAPPED;
}



/* altk_widget_unmap:
 */
void altk_widget_unmap ( AltkWidget *widget )
{
  /* [FIXME] */
  _unmap_recursive(widget);
}



/* _realize_child:
 */
static void _realize_child ( AltkWidget *widget )
{
  AltkWidget *child;
  ASSERT(!ALTK_WIDGET_REALIZED(widget));
  ASSERT(ALTK_WIDGET_TOP_WIDGET(widget) ||
         (widget->parent && ALTK_WIDGET_REALIZED(widget->parent)));
  ASSERT(altk_display_is_open(altk_widget_get_display(widget)));
  if (ALTK_WIDGET_VISIBLE(widget))
    {
      ALTK_WIDGET_GET_CLASS(widget)->realize(widget);
      for (child = widget->children; child; child = child->next)
        _realize_child(child);
    }
}



/* altk_widget_realize:
 */
void altk_widget_realize ( AltkWidget *widget )
{
  _realize_child(widget);
}



static void _unrealize_child ( AltkWidget *widget )
{
  AltkWidget *child;
  if (!ALTK_WIDGET_REALIZED(widget))
    return;
  for (child = widget->children; child; child = child->next)
    _unrealize_child(child);
  ALTK_WIDGET_GET_CLASS(widget)->unrealize(widget);
}



/* altk_widget_unrealize:
 */
void altk_widget_unrealize ( AltkWidget *widget )
{
  _unrealize_child(widget);
}



/* _on_realize:
 */
static void _on_realize ( AltkWidget *widget )
{
  /* CL_DEBUG("[TODO] realize(%p)", widget); */
  widget->flags |= ALTK_WIDGET_FLAG_REALIZED; /* [removeme] ?? */
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
      ASSERT(parent_window);
      widget->window = altk_window_new(parent_window,
                                       widget->x,
                                       widget->y,
                                       widget->width,
                                       widget->height,
                                       0);
      widget->window->user_data = widget;
    }
}



/* _on_unrealize:
 */
static void _on_unrealize ( AltkWidget *widget )
{
  if (!ALTK_WIDGET_NOWINDOW(widget))
    CL_ERROR("[TODO]");
  widget->window = NULL;
  widget->flags &= ~ALTK_WIDGET_FLAG_REALIZED;
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
  /* CL_DEBUG("size_request(%s) -> %d, %d", */
  /*          l_object_to_string(L_OBJECT(widget)), */
  /*          req->width, req->height); */
}



/* altk_widget_size_allocate:
 */
void altk_widget_size_allocate ( AltkWidget *widget,
                                 AltkAllocation *alloc )
{
  ASSERT(ALTK_WIDGET_GET_CLASS(widget)->size_allocate);
  /* CL_DEBUG("size_allocate(%s, %d, %d, %d, %d)", */
  /*          l_object_to_string(L_OBJECT(widget)), */
  /*          alloc->x, alloc->y, alloc->width, alloc->height); */
  ALTK_WIDGET_GET_CLASS(widget)->size_allocate(widget, alloc);
  /* CL_DEBUG("size_allocate(%p) OK", widget); */
}



/* _on_size_allocate:
 */
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc )
{
  /* CL_DEBUG("_on_size_allocate(%p)", wid); */
  if (wid->x == alloc->x && wid->y == alloc->y &&
      wid->width == alloc->width && wid->height == alloc->height)
    return;
  wid->x = alloc->x;
  wid->y = alloc->y;
  wid->width = alloc->width;
  wid->height = alloc->height;
  if (ALTK_WIDGET_REALIZED(wid))
    {
      if (!ALTK_WIDGET_NOWINDOW(wid))
        {
          altk_window_set_bounds(wid->window,
                                 wid->x,
                                 wid->y,
                                 wid->width,
                                 wid->height);
        }
      altk_widget_queue_draw(wid); /* ?? */
    }
  /* CL_DEBUG("_on_size_allocate OK"); */
}



/* altk_widget_set_name:
 */
void altk_widget_set_name ( AltkWidget *widget,
                            const gchar *name )
{
  g_free(widget->name);
  widget->name = g_strdup(name);
}



static AltkWidget *_widget_find ( AltkWidget *widget,
                                  const gchar *name )
{
  AltkWidget *child;
  AltkWidget *found;
  if (widget->name && !strcmp(widget->name, name))
    {
      return widget;
    }
  for (child = widget->children; child; child = child->next)
    {
      if ((found = _widget_find(child, name)))
        return found;
    }
  return NULL;
}



/* altk_widget_find:
 */
AltkWidget *altk_widget_find ( AltkWidget *widget,
                               const gchar *name )
{
  return _widget_find(widget, name);
}



/* altk_widget_show:
 */
void altk_widget_show ( AltkWidget *widget )
{
  if (ALTK_WIDGET_VISIBLE(widget))
    return;
  widget->flags |= ALTK_WIDGET_FLAG_VISIBLE;
  altk_widget_queue_resize(widget);
  if (ALTK_WIDGET_TOP_WIDGET(widget))
    {
      if (ALTK_WIDGET_MAPPED(widget))
        {
          altk_widget_realize(widget);
          altk_widget_queue_draw(widget);
        }
    }
  else if (widget->parent && ALTK_WIDGET_REALIZED(widget->parent))
    {
      altk_widget_realize(widget);
      altk_widget_queue_draw(widget);
    }
}



/* altk_widget_hide:
 */
void altk_widget_hide ( AltkWidget *widget )
{
  if (!ALTK_WIDGET_VISIBLE(widget))
    return;
  /* [fixme] unrealize ? */
  widget->flags &= ~ALTK_WIDGET_FLAG_VISIBLE;
  if (ALTK_WIDGET_REALIZED(widget))
    altk_widget_unrealize(widget);
  if (widget->parent)
    altk_widget_queue_resize(widget->parent);
}



/* _show_all:
 */
static void _show_all ( AltkWidget *widget )
{
  AltkWidget *child;
  /* show children first */
  for (child = widget->children; child; child = child->next)
    _show_all(child);
  if (ALTK_WIDGET_ENABLE_SHOW_ALL(widget))
    {
      altk_widget_show(widget);
    }
}



/* altk_widget_show_all:
 */
void altk_widget_show_all ( AltkWidget *widget )
{
  _show_all(widget);
}



/* altk_widget_set_enable_show_all:
 */
void altk_widget_set_enable_show_all ( AltkWidget *widget,
                                       gboolean enable )
{
  if (enable)
    widget->flags |= ALTK_WIDGET_FLAG_ENABLE_SHOW_ALL;
  else
    widget->flags &= ~ALTK_WIDGET_FLAG_ENABLE_SHOW_ALL;
}



/* altk_widget_get_shape:
 */
AltkRegion *altk_widget_get_shape ( AltkWidget *widget )
{
  CL_ERROR("[TODO] altk_widget_get_shape() is deprecated");
  return NULL;
  /* AltkRectangle r; */
  /* r.x = r.y = 0; */
  /* r.width = widget->width; */
  /* r.height = widget->height; */
  /* return altk_region_rectangle(&r); */
}



/* _event_expose:
 */
static void _event_expose ( AltkWidget *widget,
                            AltkEvent *event )
{
  AltkWidget *child;
  if (!ALTK_WIDGET_REALIZED(widget))
    return;
  if (widget->event_mask & ALTK_EVENT_MASK_EXPOSE)
    {
      /* clip area to widget's visible part */
      AltkDrawingContext context;
      AltkRegion *old_area = event->expose.area;
      /* [FIXME] altk_region_copy_offset() would help here */
      event->expose.area = altk_region_copy(old_area);
      context.area = event->expose.area;
      if (ALTK_WIDGET_NOWINDOW(widget)) {
        altk_region_offset(event->expose.area, -widget->x, -widget->y);
        context.offset_x = -widget->x;
        context.offset_y = -widget->y;
      } else {
        context.offset_x = context.offset_y = 0;
      }
      altk_widget_intersect_visible_area(widget, event->expose.area);
      /* debug */
      ALTK_WINDOW_DRAW_UPDATE(event->expose.window, event->expose.area, -context.offset_x, -context.offset_y, 0x0000ff);
      /* process the event */
      altk_window_begin_draw(event->expose.window, &context);
      ALTK_WIDGET_GET_CLASS(widget)->expose_event(widget, event);
      altk_window_end_draw(event->expose.window, &context);
      /* restore event */
      altk_region_destroy(event->expose.area);
      event->expose.area = old_area;
    }
  /* process children */
  for (child = widget->children; child; child = child->next)
    _event_expose(child, event);
}



/* altk_widget_event:
 */
void altk_widget_event ( AltkWidget *widget,
                         AltkEvent *event )
{
  /* [FIXME] virtual func ? */
  /* [FIXME] check event mask !? */
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
  case ALTK_EVENT_MOUSE_MOTION:
    ALTK_WIDGET_GET_CLASS(widget)->mouse_motion_event(widget, event);
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



/* altk_widget_set_event_mask:
 */
void altk_widget_set_event_mask ( AltkWidget *widget,
                                  AltkEventMask mask )
{
  widget->event_mask = mask;
}



/* altk_widget_get_event_mask:
 */
AltkEventMask altk_widget_get_event_mask ( AltkWidget *widget )
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
  altk_style_context_set_state(widget->style_context, state);
  altk_widget_queue_resize(widget); /* [fixme] ?? */
  altk_widget_queue_draw(widget);
}



/* altk_widget_queue_draw:
 */
void altk_widget_queue_draw ( AltkWidget *widget )
{
  AltkRectangle r;
  AltkRegion *wr;
  if (!ALTK_WIDGET_REALIZED(widget))
    return;
  if (ALTK_WIDGET_NOWINDOW(widget)) {
    r.x = widget->x;
    r.y = widget->y;
  } else {
    r.x = r.y = 0;
  }
  r.width = widget->width;
  r.height = widget->height;
  /* [fixme] widget shape */
  wr = altk_region_rectangle(&r);
  altk_window_invalidate(widget->window, wr);
  altk_region_destroy(wr);
}



/* _negotiate_size:
 */
static void _negotiate_size ( AltkWidget *wid )
{
  AltkRequisition req = { 0, 0 };
  AltkAllocation alloc;
  ASSERT(ALTK_WIDGET_TOP_WIDGET(wid));
  AltkDisplay *display = altk_widget_get_display(wid);
  altk_widget_size_request(wid, &req);
  /* CL_DEBUG("NEGOTIATE SIZE: %s", l_object_to_string(wid)); */
  if (display && altk_display_is_open(display)) {
    gint w, h;
    AltkSizeHints hints;
    hints = altk_wm_get_top_widget_size_hints(wid);
    altk_display_get_size(display, &w, &h);
    /* CL_DEBUG(" -> display open (%dx%d)", w, h); */
    if (hints & ALTK_SIZE_HINT_MAXIMIZED) {
      alloc.x = alloc.y = 0;
      alloc.width = w;
      alloc.height = h;
    } else {
      alloc.x = (w - req.width) / 2;
      alloc.y = (h - req.height) / 2;
      alloc.width = req.width;
      alloc.height = req.height;
    }
  } else {
    alloc.x = 0;
    alloc.y = 0;
    alloc.width = req.width;
    alloc.height = req.height;
  }
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
      l_object_unref(q->data);
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
  /* CL_DEBUG("widget_queue_resize(%p)", widget); */
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
          if (ALTK_WIDGET_MAPPED(w))
            {
              if (!g_slist_find(resize_queue, w))
                resize_queue = g_slist_prepend(resize_queue, l_object_ref(w));
              if (resize_source_id == 0)
                resize_source_id = g_idle_add_full(ALTK_PRIORITY_RESIZE,
                                                   (GSourceFunc) _idle_resize,
                                                   NULL,
                                                   NULL);
            }
          break;
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



/* altk_widget_intersect_visible_area:
 */
void altk_widget_intersect_visible_area ( AltkWidget *widget,
                                          AltkRegion *area )
{
  AltkRectangle r;
  AltkRegion *wr;
  r.x = r.y = 0;
  r.width = widget->width;
  r.height = widget->height;
  wr = altk_region_rectangle(&r);
  altk_region_intersect(area, wr);
  altk_region_destroy(wr);
}



/* altk_widget_get_style_context:
 */
AltkStyleContext *altk_widget_get_style_context ( AltkWidget *widget )
{
  return widget->style_context;
}
