/* altkcontainer.c -
 */

#include "altk/private.h"
#include "altk/altkcontainer.h"

#include "altk/altkcontainer.inl"



void _on_foreach ( AltkWidget *widget,
                   AltkForeachFunc func,
                   gpointer data );



/* altk_container_class_init:
 */
static void altk_container_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->foreach = _on_foreach;
}



/* altk_container_add:
 */
void altk_container_add ( AltkContainer *cont,
                          AltkWidget *child )
{
  ALTK_CONTAINER_GET_CLASS(cont)->add(cont, child);
}



/* _on_foreach:
 */
void _on_foreach ( AltkWidget *widget,
                   AltkForeachFunc func,
                   gpointer data )
{
  CL_ERROR("AltkContainer.foreach() not implemented");
}
