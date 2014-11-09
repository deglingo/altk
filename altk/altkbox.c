/* altkbox.c -
 */

#include "altk/private.h"
#include "altk/altkbox.h"
#include "altk/altkwindow.h"
#include "altk/altkbox.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_ORIENTATION,
    _PROP_COUNT,
  };

static LParamSpec *pspecs[_PROP_COUNT] = { NULL, };



/* Child:
 */
typedef struct _Child
{
  AltkWidget *widget;
  AltkRequisition size_request;
}
  Child;



/* Private:
 */
typedef struct _Private
{
  AltkOrientation orientation;
  GList *children;
  gint border_size;
  gint padding;
  /* layout */
  gint nchildren;
  gint children_req;
}
  Private;

#define PRIVATE(box) ((Private *)(ALTK_BOX(box)->private))



static LObject *_get_property ( LObject *object,
                                LParamSpec *pspec );
static void _set_property ( LObject *object,
                            LParamSpec *pspec,
                            LObject *value );
static void _size_request ( AltkWidget *wid,
                            AltkRequisition *req );
static void _size_allocate ( AltkWidget *wid,
                             AltkAllocation *alloc );
static void _foreach ( AltkWidget *widget,
                       AltkForeachFunc func,
                       gpointer data );
static void _add ( AltkContainer *cont,
                   AltkWidget *child );



/* altk_box_init:
 */
static void altk_box_init ( LObject *obj )
{
  ALTK_BOX(obj)->private = g_new0(Private, 1);
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
  /* [fixme] prop default */
  PRIVATE(obj)->orientation = ALTK_VERTICAL;
  PRIVATE(obj)->border_size = 4;
  PRIVATE(obj)->padding = 4;
}



/* altk_box_class_init:
 */
static void altk_box_class_init ( LObjectClass *cls )
{
  cls->get_property = _get_property;
  cls->set_property = _set_property;
  ALTK_WIDGET_CLASS(cls)->size_request = _size_request;
  ALTK_WIDGET_CLASS(cls)->size_allocate = _size_allocate;
  ALTK_WIDGET_CLASS(cls)->foreach = _foreach;
  ALTK_CONTAINER_CLASS(cls)->add = _add;

  pspecs[PROP_ORIENTATION] =
    l_param_spec_int("orientation",
                     0);

  l_object_class_install_properties(cls, _PROP_COUNT, pspecs);
}



/* altk_box_new:
 */
AltkWidget *altk_box_new ( AltkOrientation orientation )
{
  AltkBox *box = ALTK_BOX(l_object_new(ALTK_CLASS_BOX, NULL));
  PRIVATE(box)->orientation = orientation;
  return ALTK_WIDGET(box);
}



/* _get_property:
 */
static LObject *_get_property ( LObject *object,
                                LParamSpec *pspec )
{
  switch (pspec->param_id)
    {
    case PROP_ORIENTATION:
      return L_OBJECT(l_int_new(PRIVATE(object)->orientation));
    default:
      ASSERT(0);
      return NULL;
    }
}



/* _set_property:
 */
static void _set_property ( LObject *object,
                            LParamSpec *pspec,
                            LObject *value )
{
  switch (pspec->param_id)
    {
    case PROP_ORIENTATION:
      ASSERT(L_IS_INT(value)); /* [removeme] */
      altk_box_set_orientation(ALTK_BOX(object),
                               L_INT_VALUE(value));
      break;
    default:
      ASSERT(0);
    }
}



/* altk_box_set_orientation:
 */
void altk_box_set_orientation ( AltkBox *box,
                                AltkOrientation orientation )
{
  PRIVATE(box)->orientation = orientation;
  altk_widget_queue_resize(ALTK_WIDGET(box));
}



static void _size_request_v ( AltkWidget *wid,
                              AltkRequisition *req )
{
  Private *priv = PRIVATE(wid);
  GList *l;
  req->width = 0;
  req->height = 0;
  priv->nchildren = 0;
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      if (!ALTK_WIDGET_VISIBLE(child->widget))
        continue;
      priv->nchildren++;
      altk_widget_size_request(child->widget, &child->size_request);
      req->width = MAX(req->width, child->size_request.width);
      req->height += child->size_request.height;
    }
  priv->children_req = req->height;
  req->width += 2 * priv->border_size;
  req->height += 2 * priv->border_size;
  if (priv->nchildren)
    req->height += priv->padding * (priv->nchildren - 1);
}



static void _size_request_h ( AltkWidget *wid,
                              AltkRequisition *req )
{
  Private *priv = PRIVATE(wid);
  GList *l;
  req->width = 0;
  req->height = 0;
  priv->nchildren = 0;
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      if (!ALTK_WIDGET_VISIBLE(child->widget))
        continue;
      priv->nchildren++;
      altk_widget_size_request(child->widget, &child->size_request);
      req->width += child->size_request.width;
      req->height = MAX(req->height, child->size_request.height);
    }
  priv->children_req = req->width;
  req->width += 2 * priv->border_size;
  req->height += 2 * priv->border_size;
  if (priv->nchildren)
    req->width += priv->padding * (priv->nchildren - 1);
}



static void _size_allocate_v ( AltkWidget *wid,
                               AltkAllocation *alloc )
{
  Private *priv = PRIVATE(wid);
  GList *l;
  gint offset = priv->border_size;
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      AltkAllocation child_alloc;
      child_alloc.x = alloc->x + priv->border_size;
      child_alloc.y = alloc->y + offset;
      child_alloc.width = alloc->width - 2 * priv->border_size;
      child_alloc.height = child->size_request.height;
      offset += child->size_request.height + priv->padding;
      altk_widget_size_allocate(child->widget, &child_alloc);
    }
}



static void _size_allocate_h ( AltkWidget *wid,
                               AltkAllocation *alloc )
{
  Private *priv = PRIVATE(wid);
  GList *l;
  gint offset = priv->border_size;
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      AltkAllocation child_alloc;
      child_alloc.x = alloc->x + offset;
      child_alloc.y = alloc->y + priv->border_size;
      child_alloc.width = child->size_request.width;
      child_alloc.height = alloc->height - 2 * priv->border_size;
      offset += child->size_request.width + priv->padding;
      altk_widget_size_allocate(child->widget, &child_alloc);
    }
}



/* _size_request:
 */
static void _size_request ( AltkWidget *wid,
                            AltkRequisition *req )
{
  Private *priv = PRIVATE(wid);
  switch (priv->orientation)
    {
    case ALTK_VERTICAL:
      _size_request_v(wid, req);
      break;
    case ALTK_HORIZONTAL:
      _size_request_h(wid, req);
      break;
    default:
      ASSERT(0);
    }
}



/* _size_allocate:
 */
static void _size_allocate ( AltkWidget *wid,
                             AltkAllocation *alloc )
{
  /* CL_DEBUG("[TODO] box size allocate: %d, %d, %d, %d", */
  /*          alloc->x, alloc->y, alloc->width, alloc->height); */
  Private *priv = PRIVATE(wid);
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(wid, alloc);
  /* children */
  switch (priv->orientation)
    {
    case ALTK_VERTICAL:
      _size_allocate_v(wid, alloc);
      break;
    case ALTK_HORIZONTAL:
      _size_allocate_h(wid, alloc);
      break;
    default:
      ASSERT(0);
    }
}



/* _foreach:
 */
static void _foreach ( AltkWidget *widget,
                       AltkForeachFunc func,
                       gpointer data )
{
  GList *l;
  for (l = PRIVATE(widget)->children; l; l = l->next)
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
  PRIVATE(cont)->children = g_list_append(PRIVATE(cont)->children, bchild);
}



/* altk_box_pack_start:
 */
void altk_box_pack_start ( AltkBox *box,
                           AltkWidget *child,
                           AltkPackFlags flags )
{
  altk_container_add(ALTK_CONTAINER(box), child);
}
