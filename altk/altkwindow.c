/* altkwindow.c -
 */

#include "altk/private.h"
#include "altk/altkwindow.h"
#include "altk/altkdisplay.h" /* ?? */
#include "altk/altkevent.h"
#include "altk/altkwindow.inl"



/* globals */
static GSList *redraw_queue = NULL;
static guint redraw_source_id = 0;



/* altk_window_new_root:
 */
AltkWindow *altk_window_new_root ( struct _AltkDisplay *display )
{
  AltkWindow *win;
  win = ALTK_WINDOW(l_object_new(ALTK_CLASS_WINDOW, NULL));
  /* set size */
  altk_display_get_size(display, &win->width, &win->height);
  /* update_area */
  win->update_area = altk_region_new();
  return win;
}



/* altk_window_new:
 */
AltkWindow *altk_window_new ( AltkWindow *parent,
                              gint x,
                              gint y,
                              gint width,
                              gint height )
{
  AltkWindow *win;
  ASSERT(parent);
  win = ALTK_WINDOW(l_object_new(ALTK_CLASS_WINDOW, NULL));
  /* attach to parent [FIXME] ref ? */
  win->parent = parent;
  win->next = parent->children;
  if (parent->children) {
    parent->children->prev = win;
    parent->children = win;
  } else {
    parent->children = parent->children_tail = win;
  }
  /* set coordinates */
  win->x = x;
  win->y = y;
  win->width = width;
  win->height = height;
  win->root_x = parent->root_x + x;
  win->root_y = parent->root_y + y;
  /* update_area */
  win->update_area = altk_region_new();
  /* invalidate the whole window */
  altk_window_invalidate(win, NULL);
  /* all done */
  return win;
}



/* altk_window_set_bounds:
 */
void altk_window_set_bounds ( AltkWindow *window,
                              gint x,
                              gint y,
                              gint width,
                              gint height )
{
  window->x = x;
  window->y = y;
  window->width = width;
  window->height = height;
  window->root_x = window->parent->root_x + x;
  window->root_y = window->parent->root_y + y;
  /* [FIXME] only invalidate the revealed part ? */
  altk_window_invalidate(window, NULL);
}



/* _process_redraw:
 */
static void _process_redraw ( AltkWindow *window )
{
  AltkEvent event;
  GSList *q;
  event.type = ALTK_EVENT_EXPOSE;
  event.expose.window = window;
  altk_event_process(&event);
  /* remove from redraw_queue */
  if ((q = g_slist_find(redraw_queue, window))) {
    /* [TODO] l_object_unref(window) */
    redraw_queue = g_slist_delete_link(redraw_queue, q);
  }
}



/* _process_all_redraw:
 */
static void _process_all_redraw ( void )
{
  /* [FIXME] forbid invalidate() while processing */
  while (redraw_queue)
    _process_redraw(ALTK_WINDOW(redraw_queue->data));
}



/* _idle_redraw:
 */
static gboolean _idle_redraw ( gpointer data )
{
  _process_all_redraw();
  /* remove the source */
  redraw_source_id = 0;
  return G_SOURCE_REMOVE;
}



/* altk_window_invalidate:
 */
void altk_window_invalidate ( AltkWindow *window,
                              AltkRegion *area )
{
  AltkRegion *vis_area;
  vis_area = altk_window_get_visible_area(window);
  if (!area) {
    AltkRectangle rect;
    rect.x = rect.y = 0;
    rect.width = window->width;
    rect.height = window->height;
    /* [FIXME] altk_region_intersect_rect() */
    area = altk_region_rectangle(&rect);
    altk_region_intersect(vis_area, area);
    altk_region_destroy(area);
    area = NULL;
  } else {
    altk_region_intersect(vis_area, area);
  }
  altk_region_union(window->update_area, vis_area);
  altk_region_destroy(vis_area);
  if (!altk_region_empty(window->update_area))
    {
      /* add window to redraw_queue */
      if (!g_slist_find(redraw_queue, window))
        redraw_queue = g_slist_prepend(redraw_queue, l_object_ref(window));
      /* install the expose event source */
      if (redraw_source_id == 0)
        redraw_source_id = g_idle_add_full(ALTK_PRIORITY_EXPOSE,
                                           (GSourceFunc) _idle_redraw,
                                           NULL,
                                           NULL);
    }
}



/* altk_window_get_visible_area:
 */
AltkRegion *altk_window_get_visible_area ( AltkWindow *window )
{
  /* [FIXME] */
  AltkRectangle r;
  r.x = r.y = 0;
  r.width = window->width;
  r.height = window->height;
  return altk_region_rectangle(&r);
}
