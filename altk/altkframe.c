/* altkframe.c -
 */

#include "altk/private.h"
#include "altk/altkframe.h"

#include "altk/altkframe.inl"



static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req );
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc );
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event );



/* altk_frame_class_init:
 */
static void altk_frame_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->size_request = _on_size_request;
  ((AltkWidgetClass *) cls)->size_allocate = _on_size_allocate;
  ((AltkWidgetClass *) cls)->expose_event = _on_expose_event;
}



/* altk_frame_init:
 */
static void altk_frame_init ( LObject *obj )
{
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
  altk_widget_set_event_mask(ALTK_WIDGET(obj), ALTK_EVENT_MASK_EXPOSE);
}



/* altk_frame_new:
 */
AltkWidget *altk_frame_new ( const gchar *title )
{
  AltkWidget *frame;
  frame = ALTK_WIDGET(l_object_new(ALTK_CLASS_FRAME, NULL));
  /* [todo] title */
  return frame;
}



/* _on_size_request:
 */
static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req )
{
  if (ALTK_BIN(wid)->child) {
    altk_widget_size_request(ALTK_BIN(wid)->child, req);
    req->width += 8;
    req->height += 8;
  } else {
    req->width = req->height = 8;
  }
}



/* _on_size_allocate:
 */
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc )
{
  /* CL_DEBUG("[TODO] frame size allocate: %d, %d, %d, %d", */
  /*          alloc->x, alloc->y, alloc->width, alloc->height); */
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(wid, alloc);
  if (ALTK_BIN_CHILD_VISIBLE(wid)) {
    AltkAllocation child_alloc;
    child_alloc.x = alloc->x + 4;
    child_alloc.y = alloc->y + 4;
    child_alloc.width = alloc->width - 8;
    child_alloc.height = alloc->height - 8;
    altk_widget_size_allocate(ALTK_BIN(wid)->child, &child_alloc);
  }
}



/* _on_expose_event:
 */
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event )
{
  AltkStyleContext *ctxt = altk_widget_get_style_context(wid);
  /* CL_DEBUG("Frame.expose_event()"); */
  altk_style_context_draw_frame(ctxt,
                                event->expose.gc,
                                wid->x + 1,
                                wid->y + 1,
                                wid->width - 2,
                                wid->height - 2);
}
