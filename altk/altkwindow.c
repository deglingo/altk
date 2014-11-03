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



static void _on_draw_bitmap_region ( AltkDrawable *drawable,
                                     AltkBitmap *bitmap,
                                     AltkRegion *region,
                                     gint dest_x,
                                     gint dest_y );

static void _on_draw_rectangle ( AltkDrawable *drawable,
                                 AltkGC *gc,
                                 gboolean filled,
                                 gint x,
                                 gint y,
                                 gint width,
                                 gint height );
static void _on_draw_text ( AltkDrawable *drawable,
                            AltkGC *gc,
                            gint x,
                            gint y,
                            const gchar *text );



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
  ((AltkDrawableClass *) cls)->draw_bitmap_region = _on_draw_bitmap_region;
  ((AltkDrawableClass *) cls)->draw_rectangle = _on_draw_rectangle;
  ((AltkDrawableClass *) cls)->draw_text = _on_draw_text;
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
  ALTK_WINDOW_DRAW_UPDATE(window, event.expose.area, 0xffff00);
  altk_event_process(&event);
  /* [FIXME] clear update_area */
  altk_region_destroy(window->update_area);
  window->update_area = altk_region_new();
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



/* _on_draw_bitmap_region:
 */
static void _on_draw_bitmap_region ( AltkDrawable *drawable,
                                     AltkBitmap *bitmap,
                                     AltkRegion *region,
                                     gint dest_x,
                                     gint dest_y )
{
  gint r;
  AltkRegionBox *box;
  ALLEGRO_STATE state;
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(ALTK_WINDOW(drawable)->dblbuf);
  for (r = 0, box = region->rects; r < region->n_rects; r++, box++)
    {
      al_draw_bitmap_region(bitmap->al_bitmap,
                            box->x1,
                            box->y1,
                            box->x2 - box->x1,
                            box->y2 - box->y1,
                            box->x1 + ALTK_WINDOW(drawable)->offset_x,
                            box->y1 + ALTK_WINDOW(drawable)->offset_y,
                            0);
    }
  al_restore_state(&state);
}



/* _on_draw_rectangle:
 */
static void _on_draw_rectangle ( AltkDrawable *drawable,
                                 AltkGC *gc,
                                 gboolean filled,
                                 gint x,
                                 gint y,
                                 gint width,
                                 gint height )
{
  float x1 = ((float) x) + 0.5;
  float y1 = ((float) y) + 0.5;
  float x2 = ((float) (x + width)) - 0.5;
  float y2 = ((float) (y + height)) - 0.5;
  ALLEGRO_STATE state;
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(ALTK_WINDOW(drawable)->dblbuf);
  if (filled)
    al_draw_filled_rectangle(x1, y1, x2, y2, gc->fg);
  else
    al_draw_rectangle(x1 + ALTK_WINDOW(drawable)->offset_x, y1 + ALTK_WINDOW(drawable)->offset_y, x2 + ALTK_WINDOW(drawable)->offset_x, y2 + ALTK_WINDOW(drawable)->offset_y, al_map_rgb(0, 0, 0) /* [fixme] gc->fg */, 1.0);
  al_restore_state(&state);
}



/* _on_draw_text:
 */
static void _on_draw_text ( AltkDrawable *drawable,
                            AltkGC *gc,
                            gint x,
                            gint y,
                            const gchar *text )
{
  ALLEGRO_COLOR col = al_map_rgb(255, 255, 0); /* [FIXME] */
  ALLEGRO_STATE state;
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(ALTK_WINDOW(drawable)->dblbuf);
  al_draw_text(gc->font->al_font, 
               col,
               x + ALTK_WINDOW(drawable)->offset_x,
               y + ALTK_WINDOW(drawable)->offset_y,
               ALLEGRO_ALIGN_LEFT,
               text);
  al_restore_state(&state);
}



/* _grow_double_buffer:
 */
static void _grow_double_buffer ( AltkWindow *window,
                                  gint width,
                                  gint height )
{
  if (window->dblbuf) {
    gint dw = al_get_bitmap_width(window->dblbuf);
    gint dh = al_get_bitmap_height(window->dblbuf);
    if (width > dw || height > dh) {
      al_destroy_bitmap(window->dblbuf);
      window->dblbuf = NULL;
      width = MAX(width, dw);
      height = MAX(height, dh);
    }
  }
  if (!window->dblbuf) {
    ALLEGRO_STATE state;
    al_store_state(&state, ALLEGRO_STATE_DISPLAY | ALLEGRO_STATE_TARGET_BITMAP);
    al_set_target_backbuffer(PRIVROOT(window)->display->al_display);
    window->dblbuf = al_create_bitmap(width, height);
    al_restore_state(&state);
  }
}



/* altk_window_begin_draw:
 */
void altk_window_begin_draw ( AltkWindow *window,
                              AltkRegion *area )
{
  AltkRectangle clip;
  gint r;
  AltkRegionBox *box;
  ALLEGRO_COLOR bg = al_map_rgba(0, 0, 0, 0);
  ALLEGRO_STATE state;
  altk_region_get_clipbox(area, &clip);
  /* give an appropriate size to the double buffer */
  _grow_double_buffer(window, clip.width, clip.height);
  /* clear area */
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(window->dblbuf);
  for (r = 0, box = area->rects; r < area->n_rects; r++, box++)
    {
      al_set_clipping_rectangle(box->x1 - clip.x,
                                box->y1 - clip.y,
                                box->x2 - box->x1,
                                box->y2 - box->y1);
      al_clear_to_color(bg);
    }
  al_restore_state(&state);
  /* adjust drawing offset */
  window->offset_x = -clip.x;
  window->offset_y = -clip.y;
}



/* altk_window_end_draw:
 */
void altk_window_end_draw ( AltkWindow *window,
                            AltkRegion *area )
{
  AltkRectangle clip;
  gint r;
  AltkRegionBox *box;
  ALLEGRO_STATE state;
  altk_region_get_clipbox(area, &clip);
  /* blit dblbuf -> backbuf */
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(altk_display_get_backbuf(PRIVROOT(window)->display));
  for (r = 0, box = area->rects; r < area->n_rects; r++, box++)
    {
      al_draw_bitmap_region(window->dblbuf,
                            box->x1 - clip.x,
                            box->y1 - clip.y,
                            box->x2 - box->x1,
                            box->y2 - box->y1,
                            window->root_x + box->x1,
                            window->root_y + box->y1,
                            0);
    }
  al_restore_state(&state);
  altk_region_offset(area, window->root_x, window->root_y);
  altk_display_invalidate_region(PRIVROOT(window)->display, area);
  altk_region_offset(area, -window->root_x, -window->root_y);
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
