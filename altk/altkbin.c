/* altkbin.c -
 */

#include "altk/private.h"
#include "altk/altkbin.h"

#include "altk/altkbin.inl"



static void _on_forall ( AltkWidget *wid,
                         AltkForeachFunc func,
                         gpointer data );
static void _on_add ( AltkContainer *cont,
                      AltkWidget *child );



/* altk_bin_class_init:
 */
static void altk_bin_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->forall = _on_forall;
  ((AltkContainerClass *) cls)->add = _on_add;
}



/* _on_forall:
 */
static void _on_forall ( AltkWidget *wid,
                         AltkForeachFunc func,
                         gpointer data )
{
  if (ALTK_BIN(wid)->child)
    func(ALTK_BIN(wid)->child, data);
}



/* _on_add:
 */
static void _on_add ( AltkContainer *cont,
                      AltkWidget *child )
{
  AltkBin *bin = ALTK_BIN(cont);
  /* [REMOVEME] */
  ASSERT(!bin->child);
  bin->child = l_object_ref(child);
  _altk_widget_attach_child(ALTK_WIDGET(cont), child);
}
