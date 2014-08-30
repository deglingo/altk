/* altkwidget.c -
 */

#include "altk/private.h"
#include "altk/altkwidget.h"
#include "altk/altkwidget.inl"



static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc );



/* altk_widget_class_init:
 */
static void altk_widget_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->size_allocate = _on_size_allocate;
}



/* altk_widget_size_request:
 */
void altk_widget_size_request ( AltkWidget *widget,
                                AltkRequisition *req )
{
  ALTK_WIDGET_GET_CLASS(widget)->size_request(widget, req);
  CL_DEBUG("size_request(%p) -> %d, %d",
           widget, req->width, req->height);
}



/* altk_widget_size_allocate:
 */
void altk_widget_size_allocate ( AltkWidget *widget,
                                 AltkAllocation *alloc )
{
  CL_DEBUG("size_allocate(%p, %d, %d, %d, %d)",
           widget, alloc->x, alloc->y, alloc->width, alloc->height);
  ALTK_WIDGET_GET_CLASS(widget)->size_allocate(widget, alloc);
}



/* _on_size_allocate:
 */
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc )
{
  wid->x = alloc->x;
  wid->y = alloc->y;
  wid->width = alloc->width;
  wid->height = alloc->height;
  /* [FIXME] relative to parent */
  wid->root_x = alloc->x;
  wid->root_y = alloc->y;
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
