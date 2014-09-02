/* altkdisplay.c -
 */

#include "altk/private.h"
#include "altk/altkdisplay.h"
#include "altk/altkmain.h"
#include "altk/altkwidget.h"
#include "altk/altkevent.h"
#include "altk/altkbitmap.h"

#include "altk/altkdisplay.inl"



/* [FIXME] */
#define ALTK_PRIORITY_RESIZE 0
#define ALTK_PRIORITY_REDRAW 100



/* altk_display_class_init:
 */
static void altk_display_class_init ( LObjectClass *cls )
{
}



/* altk_display_new:
 */
AltkDisplay *altk_display_new ( void )
{
  AltkDisplay *display;
  display = ALTK_DISPLAY(l_object_new(ALTK_CLASS_DISPLAY, NULL));
  /* [FIXME] insance init */
  display->update_area = altk_region_new();
  return display;
}



/* altk_display_from_al_display:
 */
AltkDisplay *altk_display_from_al_display ( ALLEGRO_DISPLAY *al_display )
{
  gpointer display = g_dataset_id_get_data(al_display, ALTK_QUARK_AL_OWNER);
  return display ? ALTK_DISPLAY(display) : NULL;
}



/* _idle_resize:
 */
static gboolean _idle_resize ( AltkDisplay *display )
{
  GList *l;
  for (l = display->top_widgets; l; l = l->next) {
    AltkWidget *wid = ALTK_WIDGET(l->data);
    AltkRequisition req = { 0, };
    AltkAllocation alloc;
    altk_widget_size_request(wid, &req);
    alloc.x = 0;
    alloc.y = 0;
    alloc.width = req.width;
    alloc.height = req.height;
    altk_widget_size_allocate(wid, &alloc);
  }
  return FALSE;
}



/* _map_widget:
 */
static void _map_widget ( AltkWidget *widget,
                          AltkDisplay *display )
{
  altk_widget_map(widget, display);
  altk_widget_forall(widget, (AltkForeachFunc) _map_widget, display);
}



/* altk_display_open:
 */
void altk_display_open ( AltkDisplay *display )
{
  ALLEGRO_STATE state;
  GList *l;
  al_store_state(&state, ALLEGRO_STATE_DISPLAY | ALLEGRO_STATE_NEW_DISPLAY_PARAMETERS | ALLEGRO_STATE_TARGET_BITMAP);
  al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_REQUIRE);
  /* al_set_new_display_option(ALLEGRO_SWAP_METHOD, 1, ALLEGRO_REQUIRE); */
  if (!(display->al_display = al_create_display(640, 480)))
    CL_ERROR("could not create allegro display");
  al_restore_state(&state);
  CL_DEBUG("allegro display created:");
  CL_DEBUG(" - size         : %dx%d",
           al_get_display_width(display->al_display),
           al_get_display_height(display->al_display));
  CL_DEBUG(" - color_size   : %d", al_get_display_option(display->al_display, ALLEGRO_COLOR_SIZE));
  CL_DEBUG(" - render_method: %d", al_get_display_option(display->al_display, ALLEGRO_RENDER_METHOD));
  CL_DEBUG(" - single_buffer: %d", al_get_display_option(display->al_display, ALLEGRO_SINGLE_BUFFER));
  CL_DEBUG(" - swap_method  : %d", al_get_display_option(display->al_display, ALLEGRO_SWAP_METHOD));
  if (!al_get_display_option(display->al_display, ALLEGRO_SINGLE_BUFFER) == 1)
    CL_ERROR("[FIXME] display is not single-buffered!");
  display->backbuf = altk_bitmap_new(display,
                                     al_get_display_width(display->al_display),
                                     al_get_display_height(display->al_display));
  g_dataset_id_set_data(display->al_display, ALTK_QUARK_AL_OWNER, display);
  altk_main_register_al_source(al_get_display_event_source(display->al_display));
  /* map all widgets */
  for (l = display->top_widgets; l; l = l->next) {
    _map_widget(ALTK_WIDGET(l->data), display);
  }
  /* [FIXME] */
  g_idle_add_full(ALTK_PRIORITY_RESIZE,
                  (GSourceFunc) _idle_resize,
                  display,
                  NULL);
  /* [FIXME] */
  altk_display_queue_draw(display, NULL);
}



/* altk_display_attach_widget:
 */
void altk_display_attach_widget ( AltkDisplay *display,
                                  struct _AltkWidget *widget )
{
  display->top_widgets = g_list_append(display->top_widgets,
                                       l_object_ref(widget));
}



struct child_redraw_data
{
  AltkDisplay *display;
  AltkRegion *area;
};



static void _grow_dblbuf ( AltkDisplay *display,
                           gint width,
                           gint height )
{
  /* [FIXME] !!!! */
  display->dblbuf = NULL;
  display->dblbuf = altk_bitmap_new(display, width, height);
}



static void _clear_dblbuf ( AltkDisplay *display,
                            AltkRegion *area )
{
  gint r;
  AltkRegionBox *box;
  gint cx, cy, cw, ch;
  ALLEGRO_STATE state;
  ALLEGRO_COLOR col = al_map_rgba(0, 0, 0, 0);
  al_store_state(&state, ALLEGRO_STATE_DISPLAY | ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(ALTK_BITMAP(display->dblbuf)->al_bitmap);
  al_get_clipping_rectangle(&cx, &cy, &cw, &ch);
  for (r = 0, box = area->rects; r < area->n_rects; r++, box++)
    {
      al_set_clipping_rectangle(box->x1,
                                box->y1,
                                box->x2 - box->x1,
                                box->y2 - box->y1);
      al_clear_to_color(col);
    }
  al_set_clipping_rectangle(cx, cy, cw, ch);
  al_restore_state(&state);
}



static void _process_widget_redraw ( AltkDisplay *display,
                                     AltkWidget *widget,
                                     AltkRegion *area )
{
  AltkRegion *wid_area;
  AltkRectangle wid_extents;
  gint root_x, root_y;
  altk_widget_get_root_coords(widget, &root_x, &root_y);
  /* get the widget's update area */
  wid_area = altk_widget_get_shape(widget);
  altk_region_offset(wid_area, root_x, root_y);
  altk_region_intersect(wid_area, area);
  if (!altk_region_empty(wid_area))
    {
      AltkWidget *child;
      /* [FIXME] avoid offset if the widget does not need exposure */
      altk_region_offset(wid_area, -root_x, -root_y);
      if (widget->event_mask & ALTK_EVENT_EXPOSE)
        {
          AltkEvent event;
          /* create/grow the dblbuf */
          altk_region_get_clipbox(wid_area, &wid_extents);
          _grow_dblbuf(display,
                       wid_extents.x + wid_extents.width,
                       wid_extents.y + wid_extents.height);
          _clear_dblbuf(display, wid_area);
          /* create the expose event */
          event.type = ALTK_EVENT_EXPOSE;
          event.expose.widget = widget;
          event.expose.area = wid_area;
          event.expose.window = display->dblbuf;
          altk_event_process(&event);
          altk_drawable_draw_bitmap_region(display->backbuf,
                                           ALTK_BITMAP(display->dblbuf),
                                           wid_area,
                                           root_x,
                                           root_y);
        }
      /* process children */
      altk_region_offset(wid_area, root_x, root_y);
      for (child = widget->children_tail; child; child = child->prev) {
        _process_widget_redraw(display, child, wid_area);
      }
    }
  /* cleanup */
  altk_region_destroy(wid_area);
}



static void _update_display ( AltkDisplay *display,
                              AltkRegion *area )
{
  /* [FIXME] don't know how to handle vsync */
  ALLEGRO_STATE state;
  gint r;
  AltkRegionBox *box;
  al_store_state(&state, ALLEGRO_STATE_DISPLAY | ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_backbuffer(display->al_display);
  for (r = 0, box = area->rects; r < area->n_rects; r++, box++)
    {
      al_draw_bitmap_region(ALTK_BITMAP(display->backbuf)->al_bitmap,
                            box->x1,
                            box->y1,
                            box->x2 - box->x1,
                            box->y2 - box->y1,
                            box->x1,
                            box->y1,
                            0);
    }
  al_flip_display();
  al_restore_state(&state);
}



/* _idle_redraw:
 */
static gboolean _idle_redraw ( AltkDisplay *display )
{
  GList *l;
  AltkRegion *update_area = display->update_area;
  display->update_area = altk_region_new();
  for (l = g_list_last(display->top_widgets); l; l = l->prev)
    {
      AltkWidget *wid = ALTK_WIDGET(l->data);
      _process_widget_redraw(display, wid, update_area);
    }
  /* update the display */
  _update_display(display, update_area);
  /* cleanup */
  altk_region_destroy(update_area);
  /* uninstall the event source */
  display->redraw_source_id = 0;
  return FALSE;
}



/* altk_display_queue_draw:
 */
void altk_display_queue_draw ( AltkDisplay *display,
                               AltkRegion *area )
{
  /* [TODO] clip to display bounds */
  if (area) {
    altk_region_union(display->update_area, area);
  } else {
    AltkRectangle rect;
    rect.x = rect.y = 0;
    rect.width = al_get_display_width(display->al_display);
    rect.height = al_get_display_height(display->al_display);
    altk_region_union_with_rect(display->update_area, &rect);
  }
  /* install the event source */
  if (display->redraw_source_id == 0) {
    display->redraw_source_id = g_idle_add_full(ALTK_PRIORITY_REDRAW,
                                                (GSourceFunc) _idle_redraw,
                                                display,
                                                NULL);
  }
}



static AltkWidget *_widget_at ( AltkDisplay *display,
                                AltkWidget *wid,
                                gint x,
                                gint y )
{
  if (x >= wid->x && x < (wid->x + wid->width) &&
      y >= wid->y && y < (wid->y + wid->height))
    {
      AltkWidget *child = wid->children;
      while (child) {
        AltkWidget *r = _widget_at(display, child, x - wid->x, y - wid->y);
        if (r) return r;
        child = child->next;
      }
      return wid;
    }
  else
    {
      return NULL;
    }
}



/* altk_display_get_widget_at:
 */
AltkWidget *altk_display_get_widget_at ( AltkDisplay *display,
                                         gint x,
                                         gint y )
{
  GList *l;
  for (l = display->top_widgets; l; l = l->next) {
    AltkWidget *w = _widget_at(display, ALTK_WIDGET(l->data), x, y);
    if (w) return w;
  }
  return NULL;
}
