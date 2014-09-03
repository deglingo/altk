/* altkcontainer.c -
 */

#include "altk/private.h"
#include "altk/altkcontainer.h"

#include "altk/altkcontainer.inl"



/* altk_container_add:
 */
void altk_container_add ( AltkContainer *cont,
                          AltkWidget *child )
{
  ALTK_CONTAINER_GET_CLASS(cont)->add(cont, child);
}
