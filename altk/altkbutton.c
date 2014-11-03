/* altkbutton.h -
 */

#include "altk/private.h"
#include "altk/altkbutton.h"
#include "altk/altklabel.h"
#include "altk/altkbitmap.h" /* [REMOVEME] */
#include "altk/altkwindow.h"
#include "altk/altkstylecontext.h"
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
  altk_widget_set_event_mask(ALTK_WIDGET(obj),
                             ALTK_EVENT_MASK_EXPOSE);
}



/* altk_button_new_with_label:
 */
AltkWidget *altk_button_new_with_label ( const gchar *text )
{
  AltkWidget *but, *lbl;
  but = ALTK_WIDGET(l_object_new(ALTK_CLASS_BUTTON, NULL));
  altk_widget_set_event_mask(but,
                             ALTK_EVENT_MASK_EXPOSE |
                             ALTK_EVENT_MASK_MOUSE_MOTION |
                             ALTK_EVENT_MASK_MOUSE_BUTTON);
  lbl = altk_label_new(text);
  altk_container_add(ALTK_CONTAINER(but), lbl);
  /* [FIXME] l_object_unref(lbl); */
  return but;
}



/* _on_realize:
 */
static void _on_realize ( AltkWidget *wid )
{
  ALTK_WIDGET_CLASS(parent_class)->realize(wid);
  ALTK_BUTTON(wid)->event_window = altk_window_new(wid->parent->window,
                                                   wid->x,
                                                   wid->y,
                                                   wid->width,
                                                   wid->height,
                                                   ALTK_WINDOW_FLAG_INPUT_ONLY);
  altk_window_set_event_mask(ALTK_BUTTON(wid)->event_window,
                             ALTK_EVENT_MASK_MOUSE_CROSSING |
                             ALTK_EVENT_MASK_MOUSE_BUTTON);
  ALTK_BUTTON(wid)->event_window->user_data = wid;
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
  /* CL_DEBUG("[TODO] button size allocate: %d, %d, %d, %d", */
  /*          alloc->x, alloc->y, alloc->width, alloc->height); */
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(wid, alloc);
  if (ALTK_WIDGET_REALIZED(wid))
    {
      altk_window_set_bounds(ALTK_BUTTON(wid)->event_window,
                             alloc->x,
                             alloc->y,
                             alloc->width,
                             alloc->height);
    }
  /* ---- */
  if (ALTK_BIN_CHILD_VISIBLE(wid)) {
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
  /* CL_TRACE("%p", wid); */
  altk_widget_set_state(wid, ALTK_STATE_PRELIGHT);
}



/* _on_mouse_leave_event:
 */
static void _on_mouse_leave_event ( AltkWidget *wid,
                                    AltkEvent *event )
{
  /* CL_TRACE("%p", wid); */
  altk_widget_set_state(wid, ALTK_STATE_NORMAL);
}



/* _on_mouse_button_down_event:
 */
static void _on_mouse_button_down_event ( AltkWidget *wid,
                                          AltkEvent *event )
{
  /* CL_TRACE("%p", wid); */
  altk_widget_set_state(wid, ALTK_STATE_ACTIVE);
}



/* _on_mouse_button_up_event:
 */
static void _on_mouse_button_up_event ( AltkWidget *wid,
                                        AltkEvent *event )
{
  /* CL_TRACE("%p", wid); */
  altk_widget_set_state(wid, ALTK_STATE_PRELIGHT);
}



/* _on_expose_event:
 */
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event )
{
  /* CL_TRACE("%s", L_OBJECT_REPR(wid)); */
  AltkStyleContext *ctxt = altk_widget_get_style_context(wid);
  /* CL_DEBUG("Button.expose_event()"); */
  altk_style_context_draw_box(ctxt,
                              ALTK_DRAWABLE(event->expose.window),
                              wid->x,
                              wid->y,
                              wid->width,
                              wid->height);
  /* altk_style_draw_box(wid->style, */
  /*                     ALTK_DRAWABLE(event->expose.window), */
  /*                     wid->gc[wid->state], */
  /*                     0 /\* [fixme] box type *\/, */
  /*                     0, */
  /*                     0, */
  /*                     wid->width, */
  /*                     wid->height); */
}
