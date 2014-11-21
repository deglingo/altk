/* altkbin.c -
 */

#include "altk/private.h"
#include "altk/altkbin.h"

#include "altk/altkbin.inl"



static void _on_add ( AltkContainer *cont,
                      AltkWidget *child );



/* altk_bin_class_init:
 */
static void altk_bin_class_init ( LObjectClass *cls )
{
  ((AltkContainerClass *) cls)->add = _on_add;
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
