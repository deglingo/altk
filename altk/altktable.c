/* altktable.c -
 */

#include "altk/private.h"
#include "altk/altktable.h"
#include "altk/altkwindow.h"
#include "altk/altktable.inl"



/* Child:
 */
typedef struct _Child
{
  AltkWidget *widget;
}
  Child;



static void _size_request ( AltkWidget *wid,
                            AltkRequisition *req );
static void _size_allocate ( AltkWidget *wid,
                             AltkAllocation *alloc );
static void _foreach ( AltkWidget *widget,
                       AltkForeachFunc func,
                       gpointer data );
static void _add ( AltkContainer *cont,
                   AltkWidget *child );



/* altk_table_init:
 */
static void altk_table_init ( LObject *obj )
{
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
}



/* altk_table_class_init:
 */
static void altk_table_class_init ( LObjectClass *cls )
{
  ALTK_WIDGET_CLASS(cls)->size_request = _size_request;
  ALTK_WIDGET_CLASS(cls)->size_allocate = _size_allocate;
  ALTK_WIDGET_CLASS(cls)->foreach = _foreach;
  ALTK_CONTAINER_CLASS(cls)->add = _add;
}



/* altk_table_new:
 */
AltkWidget *altk_table_new ( void )
{
  AltkTable *table = ALTK_TABLE(l_object_new(ALTK_CLASS_TABLE, NULL));
  return ALTK_WIDGET(table);
}



/* _size_request:
 */
static void _size_request ( AltkWidget *wid,
                            AltkRequisition *req )
{
  CL_DEBUG("[TODO] Table.size_request()");
}



/* _size_allocate:
 */
static void _size_allocate ( AltkWidget *wid,
                             AltkAllocation *alloc )
{
  CL_DEBUG("[TODO] Table.size_allocate()");
}



/* _foreach:
 */
static void _foreach ( AltkWidget *widget,
                       AltkForeachFunc func,
                       gpointer data )
{
  GList *l;
  for (l = ALTK_TABLE(widget)->children; l; l = l->next)
    {
      if (func(((Child *) (l->data))->widget, data) == ALTK_FOREACH_STOP)
        return;
    }
}



/* _add:
 */
static void _add ( AltkContainer *cont,
                   AltkWidget *child )
{
  Child *bchild = g_new0(Child, 1);
  _altk_widget_set_parent(child, ALTK_WIDGET(cont));
  bchild->widget = l_object_ref(child);
  ALTK_TABLE(cont)->children = g_list_append(ALTK_TABLE(cont)->children, bchild);
}



/* altk_table_attach:
 */
void altk_table_attach ( AltkTable *table,
                         AltkWidget *child,
                         gint top,
                         gint left,
                         gint bottom,
                         gint right,
                         AltkPackFlags flags )
{
  altk_container_add(ALTK_CONTAINER(table), child);
}
