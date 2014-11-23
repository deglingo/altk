/* altkbin.c -
 */

#include "altk/private.h"
#include "altk/altkbin.h"

#include "altk/altkbin.inl"



static void _on_add ( AltkContainer *cont,
                      AltkWidget *child );
static void _on_remove ( AltkContainer *cont,
                         AltkWidget *child );



/* altk_bin_class_init:
 */
static void altk_bin_class_init ( LObjectClass *cls )
{
  ALTK_CONTAINER_CLASS(cls)->add = _on_add;
  ALTK_CONTAINER_CLASS(cls)->remove = _on_remove;
}



/* _on_add:
 */
static void _on_add ( AltkContainer *cont,
                      AltkWidget *child )
{
  ASSERT(!ALTK_BIN(cont)->child);
  ALTK_BIN(cont)->child = l_object_ref(child);
  _altk_widget_set_parent(child, ALTK_WIDGET(cont));
}



/* _on_remove:
 */
static void _on_remove ( AltkContainer *cont,
                         AltkWidget *child )
{
  ASSERT(child == ALTK_BIN_CHILD(cont));
  _altk_widget_unset_parent(child);
  L_OBJECT_CLEAR(ALTK_BIN(cont)->child);
}
