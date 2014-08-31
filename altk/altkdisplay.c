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



/* altk_display_open:
 */
void altk_display_open ( AltkDisplay *display )
{
  display->al_display = al_create_display(640, 480);
  g_dataset_id_set_data(display->al_display, ALTK_QUARK_AL_OWNER, display);
  altk_main_register_al_source(al_get_display_event_source(display->al_display));
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



static void _grow_backbuf ( AltkDisplay *display,
                            gint width,
                            gint height )
{
  /* [FIXME] !!!! */
  display->backbuf = NULL;
  display->backbuf = altk_bitmap_new(display, width, height);
}



static void _process_child_redraw ( AltkWidget *widget,
                                    struct child_redraw_data *data )
{
  AltkRegion *wid_area;
  AltkEvent event;
  AltkRectangle wid_extents;
  /* process children first */
  altk_widget_forall(widget, (AltkForeachFunc) _process_child_redraw, data);
  /* get the widget's update area (and subract it from 'area') */
  wid_area = altk_widget_get_shape(widget);
  altk_region_offset(wid_area, widget->root_x, widget->root_y);
  altk_region_intersect(wid_area, data->area);
  altk_region_subtract(data->area, wid_area);
  altk_region_offset(wid_area, -widget->root_x, -widget->root_y);
  /* create/grow the backbuf */
  altk_region_get_clipbox(wid_area, &wid_extents);
  _grow_backbuf(data->display, wid_extents.width, wid_extents.height);
  altk_drawable_set_offset(data->display->backbuf, -wid_extents.x, -wid_extents.y);
  /* create the expose event */
  event.type = ALTK_EVENT_EXPOSE;
  event.expose.widget = widget;
  event.expose.area = wid_area;
  event.expose.window = data->display->backbuf;
  altk_event_process(&event);
  /* [TODO] widget_redraw(wid_area) */
  altk_region_destroy(wid_area);
}



/* _process_widget_redraw:
 */
static void _process_widget_redraw ( AltkDisplay *display,
                                     AltkWidget *widget,
                                     AltkRegion *area )
{
  struct child_redraw_data data;
  data.display = display;
  data.area = area;
  _process_child_redraw(widget, &data);
}



/* _idle_redraw:
 */
static gboolean _idle_redraw ( AltkDisplay *display )
{
  GList *l;
  AltkRegion *update_area = display->update_area;
  display->update_area = altk_region_new();
  /* process widgets */
  for (l = display->top_widgets; l; l = l->next)
    {
      AltkWidget *wid = ALTK_WIDGET(l->data);
      _process_widget_redraw(display, wid, update_area);
    }
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
  if (area) {
    CL_ERROR("[TODO]");
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
