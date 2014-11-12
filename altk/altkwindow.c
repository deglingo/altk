/* altkwindow.c -
 */

#include "altk/private.h"
#include "altk/altkwindow.h"
#include "altk/altkdisplay.h" /* ?? */
#include "altk/altkevent.h"
#include "altk/altkgc.h"
#include "altk/altkfont.h"
#include "altk/altkbitmap.h"
#include "altk/altkwindow.inl"



/* PrivateRoot:
 */
typedef struct _PrivateRoot
{
  AltkDisplay *display;
  GSList *redraw_queue;
  guint redraw_source_id;
  AltkDrawable *dblbuf;
  AltkGC *gc;
}
  PrivateRoot;



/* Private:
 */
typedef struct _Private
{
  AltkWindow *root;
  PrivateRoot *privroot;
}
  Private;

#define PRIVATE(win) ((Private *)(ALTK_WINDOW(win)->private))
#define PRIVROOT(win) (PRIVATE(win)->privroot)



/* _altk_window_draw_update:
 *
 * debug
 */
void _altk_window_draw_update ( AltkWindow *window,
                                AltkRegion *area,
                                guint32 hcol )
{
  ALLEGRO_BITMAP *save;
  ALLEGRO_STATE state;
  AltkDisplay *display = PRIVROOT(window)->display;
  ALLEGRO_COLOR col;
  gint display_width, display_height;
  col = al_map_rgba((hcol >> 16) & 0xff,
                    (hcol >> 8)  & 0xff,
                    hcol         & 0xff,
                    (hcol >> 24) & 0xff);
  display_width = al_get_display_width(display->al_display);
  display_height = al_get_display_height(display->al_display);
  /* store state */
  al_store_state(&state,
                 ALLEGRO_STATE_DISPLAY |
                 ALLEGRO_STATE_NEW_BITMAP_PARAMETERS |
                 ALLEGRO_STATE_TARGET_BITMAP);
  /* create some backup buffer */
  al_set_target_backbuffer(PRIVROOT(window)->display->al_display);
  al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY);
  al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
  if (!(save = al_create_bitmap(display_width,
                                display_height)))
    CL_ERROR("could not create backup bitmap");
  /* save the whole display */
  al_set_target_bitmap(save);
  al_draw_bitmap(al_get_backbuffer(display->al_display), 0, 0, 0);
  /* draw our area */
  al_set_target_backbuffer(display->al_display);
  {
    gint r;
    AltkRegionBox *box;
    for (r = 0, box = area->rects; r < area->n_rects; r++, box++)
      {
        float x1 = ((float) (box->x1 + window->root_x)) + 0.5;
        float y1 = ((float) (box->y1 + window->root_y)) + 0.5;
        float x2 = ((float) (box->x2 + window->root_x)) - 0.5;
        float y2 = ((float) (box->y2 + window->root_y)) - 0.5;
        al_draw_rectangle(x1, y1, x2, y2, col, 1.0);
        al_draw_line(x1, y1, x2, y2, col, 1.0);
        al_draw_line(x1, y2, x2, y1, col, 1.0);
      }
  }
  al_flip_display();
  /* sleep */
  g_usleep(ALTK_DEBUG_UPDATE_DELAY);
  /* restore the backbuffer */
  al_set_target_backbuffer(PRIVROOT(window)->display->al_display);
  al_draw_bitmap(save, 0, 0, 0);
  /* cleanup */
  al_destroy_bitmap(save);
  /* restore state */
  al_restore_state(&state);
}



/* altk_window_class_init:
 */
static void altk_window_class_init ( LObjectClass *cls )
{
}



/* altk_window_init:
 */
static void altk_window_init ( LObject *obj )
{
  ALTK_WINDOW(obj)->private = g_new0(Private, 1);
}



/* altk_window_new_root:
 */
AltkWindow *altk_window_new_root ( struct _AltkDisplay *display )
{
  AltkWindow *win;
  Private *priv;
  win = ALTK_WINDOW_NEW(NULL);
  priv = win->private;
  priv->root = win;
  priv->privroot = g_new0(PrivateRoot, 1);
  priv->privroot->display = display;
  priv->privroot->gc = altk_gc_new(NULL);
  win->flags = ALTK_WINDOW_FLAG_ROOT;
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
                              gint height,
                              AltkWindowFlags flags )
{
  AltkWindow *win;
  Private *priv;
  ASSERT(parent);
  win = ALTK_WINDOW_NEW(NULL);
  priv = win->private;
  priv->root = PRIVATE(parent)->root;
  priv->privroot = PRIVATE(parent)->privroot;
  win->flags = flags;
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
  /* setup output window */
  if (!(win->flags & ALTK_WINDOW_FLAG_INPUT_ONLY))
    {
      /* update_area */
      win->update_area = altk_region_new();
      /* invalidate the whole window */
      altk_window_invalidate(win, NULL);
    }
  /* all done */
  return win;
}



/* altk_window_set_event_mask:
 */
void altk_window_set_event_mask ( AltkWindow *window,
                                  AltkEventMask mask )
{
  window->event_mask = mask;
}



static void _fix_root_xy ( AltkWindow *window )
{
  AltkWindow *child;
  window->root_x = window->parent->root_x + window->x;
  window->root_y = window->parent->root_y + window->y;
  for (child = window->children; child; child = child->next)
    _fix_root_xy(child);
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
  /* fix root_x/y for all children */
  _fix_root_xy(window);
  if (!(window->flags & ALTK_WINDOW_FLAG_INPUT_ONLY))
    {
      /* [FIXME] only invalidate the revealed part ? */
      /* [FIXME] also revealed other window parts which have become
         visible */
      altk_window_invalidate(window, NULL);
    }
}



/* _process_redraw:
 */
static void _process_redraw ( AltkWindow *window )
{
  AltkEvent event;
  GSList *q;
  event.type = ALTK_EVENT_EXPOSE;
  event.expose.window = window;
  /* [TODO] clip area to visible area */
  event.expose.area = window->update_area;
  window->update_area = altk_region_new();
  event.expose.gc = PRIVROOT(window)->gc;
  /* [FIXME] prepare gc */
  ALTK_WINDOW_DRAW_UPDATE(window, event.expose.area, 0xffff00);
  altk_event_process(&event);
  /* [FIXME] clear update_area */
  altk_region_destroy(event.expose.area);
  /* remove from redraw_queue */
  if ((q = g_slist_find(PRIVROOT(window)->redraw_queue, window))) {
    PRIVROOT(window)->redraw_queue = g_slist_delete_link(PRIVROOT(window)->redraw_queue, q);
    l_object_unref(window);
  }
}



/* _process_all_redraw:
 */
static void _process_all_redraw ( AltkWindow *root )
{
  /* [FIXME] forbid invalidate() while processing */
  while (PRIVROOT(root)->redraw_queue)
    _process_redraw(PRIVROOT(root)->redraw_queue->data);
  /* flip display */
  altk_display_flip(PRIVROOT(root)->display);
}



/* _idle_redraw:
 */
static gboolean _idle_redraw ( gpointer data )
{
  AltkWindow *win = ALTK_WINDOW(data);
  _process_all_redraw(win);
  /* remove the source */
  PRIVROOT(win)->redraw_source_id = 0;
  return G_SOURCE_REMOVE;
}



/* altk_window_invalidate:
 */
void altk_window_invalidate ( AltkWindow *window,
                              AltkRegion *area )
{
  AltkRegion *vis_area;
  ASSERT(!(window->flags & ALTK_WINDOW_FLAG_INPUT_ONLY));
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
      if (!g_slist_find(PRIVROOT(window)->redraw_queue, window))
        PRIVROOT(window)->redraw_queue = g_slist_prepend
          (PRIVROOT(window)->redraw_queue, l_object_ref(window));
      /* install the expose event source */
      if (PRIVROOT(window)->redraw_source_id == 0)
        PRIVROOT(window)->redraw_source_id = g_idle_add_full(ALTK_PRIORITY_EXPOSE,
                                                             (GSourceFunc) _idle_redraw,
                                                             PRIVATE(window)->root, /* [fixme] ref ? */
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



/* _grow_double_buffer:
 */
static void _grow_double_buffer ( AltkWindow *window,
                                  gint width,
                                  gint height )
{
  if (PRIVROOT(window)->dblbuf) {
    gint dw, dh;
    altk_drawable_get_size(PRIVROOT(window)->dblbuf, &dw, &dh);
    if (width > dw || height > dh) {
      L_OBJECT_CLEAR(PRIVROOT(window)->dblbuf);
      width = MAX(width, dw);
      height = MAX(height, dh);
    }
  }
  if (!PRIVROOT(window)->dblbuf) {
    PRIVROOT(window)->dblbuf = altk_bitmap_new(PRIVROOT(window)->display,
                                               width, height);
    altk_gc_set_surface(PRIVROOT(window)->gc, PRIVROOT(window)->dblbuf);
  }
}



/* altk_window_begin_draw:
 */
void altk_window_begin_draw ( AltkWindow *window,
                              AltkDrawingContext *context )
{
  AltkRectangle clip;
  altk_region_get_clipbox(context->area, &clip);
  /* give an appropriate size to the double buffer */
  _grow_double_buffer(window, clip.width, clip.height);
  /* clear area */
  /* [FIXME] save/restore color */
  altk_gc_set_offset(PRIVROOT(window)->gc,
                     (-clip.x),
                     (-clip.y));
  altk_gc_set_color_hargb(PRIVROOT(window)->gc, 0x0);
  altk_gc_clear_region(PRIVROOT(window)->gc, context->area);
  /* altk_gc_set_clip_rect(PRIVROOT(window)->gc, &clip); */
}



/* altk_window_end_draw:
 */
void altk_window_end_draw ( AltkWindow *window,
                            AltkDrawingContext *context )
{
  AltkRectangle clip;
  altk_region_get_clipbox(context->area, &clip);
  /* blit dblbuf -> backbuf */
  altk_display_draw_bitmap_region
    (PRIVROOT(window)->display,
     ALTK_BITMAP(PRIVROOT(window)->dblbuf),
     context->area,
     (-clip.x),
     (-clip.y),
     window->root_x - context->offset_x,
     window->root_y - context->offset_y);
  /* [FIXME] restore gc state */
}



/* altk_window_get_child_at:
 */
AltkWindow *altk_window_get_child_at ( AltkWindow *window,
                                       gint x,
                                       gint y )
{
  if (x >= 0 && x < window->width && y >= 0 && y < window->height)
    {
      AltkWindow *child, *r;
      for (child = window->children; child; child = child->next)
        {
          if ((r = altk_window_get_child_at(child, x - child->x, y - child->y)))
            return r;
        }
      return window;
    }
  else
    {
      return NULL;
    }
}
