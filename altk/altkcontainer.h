/* altkcontainer.h -
 */

#ifndef _ALTKCONTAINER_H_
#define _ALTKCONTAINER_H_

#include "altk/altkwidget.h"
#include "altk/altkcontainer-def.h"



/* AltkContainer:
 */
struct _AltkContainer
{
  ALTK_CONTAINER_INSTANCE_HEADER;
};



/* AltkContainerClass:
 */
struct _AltkContainerClass
{
  ALTK_CONTAINER_CLASS_HEADER;

  void (* add) ( AltkContainer *cont,
                 AltkWidget *child );

  void (* remove) ( AltkContainer *cont,
                    AltkWidget *child );
};



#define ALTK_CONTAINER_ADD(cont, child) altk_container_add(ALTK_CONTAINER(cont), (child))
#define ALTK_CONTAINER_REMOVE(cont, child) altk_container_remove(ALTK_CONTAINER(cont), (child))

void altk_container_add ( AltkContainer *cont,
                          AltkWidget *child );
void altk_container_remove ( AltkContainer *cont,
                             AltkWidget *child );



#endif
