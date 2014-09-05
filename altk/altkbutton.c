/* altkbutton.h -
 */

#include "altk/private.h"
#include "altk/altkbutton.h"
#include "altk/altklabel.h"
#include "altk/altkbitmap.h" /* [REMOVEME] */
#include "altk/altkwindow.h"
#include "altk/altkbutton.inl"



static void _on_realize ( AltkWidget *wid );
static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req );
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc );
static void _on_mouse_enter_event ( AltkWidget *wid,
                                    AltkEvent *event );
static void _on_mouse_leave_event ( AltkWidget *wid,
                                    AltkEvent *event );
static void _on_mouse_button_down_event ( AltkWidget *wid,
                                          AltkEvent *event );
static void _on_mouse_button_up_event ( AltkWidget *wid,
                                        AltkEvent *event );
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event );



/* altk_button_class_init:
 */
static void altk_button_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->realize = _on_realize;
  ((AltkWidgetClass *) cls)->size_request = _on_size_request;
  ((AltkWidgetClass *) cls)->size_allocate = _on_size_allocate;
  ((AltkWidgetClass *) cls)->mouse_enter_event = _on_mouse_enter_event;
  ((AltkWidgetClass *) cls)->mouse_leave_event = _on_mouse_leave_event;
  ((AltkWidgetClass *) cls)->mouse_button_down_event = _on_mouse_button_down_event;
  ((AltkWidgetClass *) cls)->mouse_button_up_event = _on_mouse_button_up_event;
  ((AltkWidgetClass *) cls)->expose_event = _on_expose_event;
}



/* altk_button_init:
 */
static void altk_button_init ( LObject *obj )
{
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
}



/* altk_button_new_with_label:
 */
AltkWidget *altk_button_new_with_label ( const gchar *text )
{
  AltkWidget *but, *lbl;
  but = ALTK_WIDGET(l_object_new(ALTK_CLASS_BUTTON, NULL));
  altk_widget_set_event_mask(but,
                             ALTK_EVENT_EXPOSE |
                             ALTK_EVENT_MOUSE_ENTER |
                             ALTK_EVENT_MOUSE_BUTTON_DOWN |
                             ALTK_EVENT_MOUSE_BUTTON_UP);
  lbl = altk_label_new(text);
  altk_container_add(ALTK_CONTAINER(but), lbl);
  /* [FIXME] l_object_unref(lbl); */
  return but;
}



/* _on_realize:
 */
static void _on_realize ( AltkWidget *wid )
{
  wid->window = wid->parent->window;
  ALTK_BUTTON(wid)->event_window = altk_window_new(wid->parent->window,
                                                   wid->x,
                                                   wid->y,
                                                   wid->width,
                                                   wid->height);
  /* [TODO] set event_mask */
}



/* _on_size_request:
 */
static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req )
{
  if (ALTK_BIN(wid)->child) {
    altk_widget_size_request(ALTK_BIN(wid)->child, req);
    req->width += 4;
    req->height += 4;
  } else {
    req->width = req->height = 4;
  }
}



/* _on_size_allocate:
 */
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc )
{
  CL_DEBUG("[TODO] dialog size allocate: %d, %d, %d, %d",
           alloc->x, alloc->y, alloc->width, alloc->height);
  /* [FIXME] chain to parent_class method */
  wid->x = alloc->x;
  wid->y = alloc->y;
  wid->width = alloc->width;
  wid->height = alloc->height;
  /* ---- */
  if (ALTK_BIN(wid)->child) {
    AltkAllocation child_alloc;
    gint disp;
    if (wid->state == ALTK_STATE_ACTIVE)
      disp = 1;
    else
      disp = 0;
    child_alloc.x = alloc->x + 2 + disp;
    child_alloc.y = alloc->y + 2 + disp;
    child_alloc.width = alloc->width - (4 + disp);
    child_alloc.height = alloc->height - (4 + disp);
    altk_widget_size_allocate(ALTK_BIN(wid)->child, &child_alloc);
  }
}



/* _on_mouse_enter_event:
 */
static void _on_mouse_enter_event ( AltkWidget *wid,
                                    AltkEvent *event )
{
  CL_DEBUG("button enter...");
  altk_widget_set_state(wid, ALTK_STATE_PRELIGHT);
}



/* _on_mouse_leave_event:
 */
static void _on_mouse_leave_event ( AltkWidget *wid,
                                    AltkEvent *event )
{
  CL_DEBUG("button leave...");
  altk_widget_set_state(wid, ALTK_STATE_NORMAL);
}



/* _on_mouse_button_down_event:
 */
static void _on_mouse_button_down_event ( AltkWidget *wid,
                                          AltkEvent *event )
{
  CL_DEBUG("button down...");
  altk_widget_set_state(wid, ALTK_STATE_ACTIVE);
  altk_widget_queue_resize(wid);
}



/* _on_mouse_button_up_event:
 */
static void _on_mouse_button_up_event ( AltkWidget *wid,
                                        AltkEvent *event )
{
  CL_DEBUG("button up...");
  altk_widget_set_state(wid, ALTK_STATE_PRELIGHT);
  altk_widget_queue_resize(wid);
}



/* _on_expose_event:
 */
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event )
{
  CL_DEBUG("[TODO] Button.expose_event()");
  /* gint r; */
  /* AltkRegionBox *box; */
  /* gint cx, cy, cw, ch; */
  /* /\* [FIXME] use drawable methods *\/ */
  /* ALLEGRO_COLOR col; */
  /* ALLEGRO_STATE state; */
  /* if (wid->state == ALTK_STATE_NORMAL) */
  /*   col = al_map_rgb(0, 255, 255); */
  /* else */
  /*   col = al_map_rgb(255, 0, 0); */
  /* al_store_state(&state, ALLEGRO_STATE_DISPLAY | ALLEGRO_STATE_TARGET_BITMAP); */
  /* al_set_target_bitmap(ALTK_BITMAP(event->expose.window)->al_bitmap); */
  /* al_get_clipping_rectangle(&cx, &cy, &cw, &ch); */
  /* for (r = 0, box = event->expose.area->rects; */
  /*      r < event->expose.area->n_rects; */
  /*      r++, box++) */
  /*   { */
  /*     al_set_clipping_rectangle(box->x1, */
  /*                               box->y1, */
  /*                               box->x2 - box->x1, */
  /*                               box->y2 - box->y1); */
  /*     al_clear_to_color(col); */
  /*   } */
  /* al_set_clipping_rectangle(cx, cy, cw, ch); */
  /* al_restore_state(&state); */
}
