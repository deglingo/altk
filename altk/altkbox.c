/* altkbox.c -
 */

#include "altk/private.h"
#include "altk/altkbox.h"
#include "altk/altkbox.inl"



static void _size_request ( AltkWidget *wid,
                            AltkRequisition *req );
static void _foreach ( AltkWidget *widget,
                       AltkForeachFunc func,
                       gpointer data );



/* altk_box_class_init:
 */
static void altk_box_class_init ( LObjectClass *cls )
{
  ALTK_WIDGET_CLASS(cls)->size_request = _size_request;
  ALTK_WIDGET_CLASS(cls)->foreach = _foreach;
}



/* altk_box_new:
 */
AltkWidget *altk_box_new ( void )
{
  return ALTK_WIDGET(l_object_new(ALTK_CLASS_BOX, NULL));
}



/* _size_request:
 */
static void _size_request ( AltkWidget *wid,
                            AltkRequisition *req )
{
  CL_DEBUG("[TODO] AltkBox.size_request()");
}



/* _foreach:
 */
static void _foreach ( AltkWidget *widget,
                       AltkForeachFunc func,
                       gpointer data )
{
  CL_DEBUG("[TODO] AltkBox.foreach()");
}



/* altk_box_pack_start:
 */
void altk_box_pack_start ( AltkBox *box,
                           AltkWidget *child,
                           AltkPackFlags flags )
{
}
