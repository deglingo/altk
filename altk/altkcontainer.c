/* altkcontainer.c -
 */

#include "altk/private.h"
#include "altk/altkcontainer.h"

#include "altk/altkcontainer.inl"



/* altk_container_class_init:
 */
static void altk_container_class_init ( LObjectClass *cls )
{
}



/* altk_container_add:
 */
void altk_container_add ( AltkContainer *cont,
                          AltkWidget *child )
{
  ASSERT(ALTK_CONTAINER_GET_CLASS(cont)->add);
  ALTK_CONTAINER_GET_CLASS(cont)->add(cont, child);
}



/* altk_container_remove:
 */
void altk_container_remove ( AltkContainer *cont,
                             AltkWidget *child )
{
  if (!ALTK_CONTAINER_GET_CLASS(cont)->remove)
    CL_ERROR("%s.remove() is not implemented",
             l_object_class_name(L_OBJECT_GET_CLASS(cont)));
  ALTK_CONTAINER_GET_CLASS(cont)->remove(cont, child);
}
