/* altkroot.c -
 */

#include "altk/private.h"
#include "altk/altkroot.h"
#include "altk/altkroot.inl"



static void _on_add ( AltkContainer *root,
                      AltkWidget *child );



/* altk_root_class_init:
 */
static void altk_root_class_init ( LObjectClass *cls )
{
  ((AltkContainerClass *) cls)->add = _on_add;
}



/* _on_add:
 */
static void _on_add ( AltkContainer *root,
                      AltkWidget *child )
{
  _altk_widget_attach_child(ALTK_WIDGET(root), child);
}
