/* altkwidget.c -
 */

#include "altk/private.h"
#include "altk/altkwidget.h"
#include "altk/altkwidget.inl"



/* altk_widget_class_init:
 */
static void altk_widget_class_init ( LObjectClass *cls )
{
}



/* altk_widget_size_request:
 */
void altk_widget_size_request ( AltkWidget *widget,
                                AltkRequisition *req )
{
  CL_DEBUG("[TODO] size_request(%p)", widget);
  req->width = 32;
  req->height = 20;
}



/* altk_widget_size_allocate:
 */
void altk_widget_size_allocate ( AltkWidget *widget,
                                 AltkAllocation *alloc )
{
  CL_DEBUG("[TODO] size_allocate(%p, %d, %d, %d, %d)",
           widget, alloc->x, alloc->y, alloc->width, alloc->height);
}
