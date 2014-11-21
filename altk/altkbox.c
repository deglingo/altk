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
  AltkPackFlags pack_flags;
  /* */
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
  gint nexpands;
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
static void _add ( AltkContainer *cont,
                   AltkWidget *child );



static Child *get_child ( AltkBox *box,
                          AltkWidget *child )
{
  GList *l;
  for (l = PRIVATE(box)->children; l; l = l->next)
    {
      Child *c = l->data;
      if (c->widget == child)
        return c;
    }
  return NULL;
}



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
  priv->nexpands = 0;
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      if (!ALTK_WIDGET_VISIBLE(child->widget))
        continue;
      priv->nchildren++;
      if (child->pack_flags & ALTK_PACK_VEXPAND)
        priv->nexpands++;
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
  priv->nexpands = 0;
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      if (!ALTK_WIDGET_VISIBLE(child->widget))
        continue;
      priv->nchildren++;
      if (child->pack_flags & ALTK_PACK_HEXPAND)
        priv->nexpands++;
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



static void _alloc_child ( AltkWidget *box,
                           Child *child,
                           gint x,
                           gint y,
                           gint width,
                           gint height )
{
  AltkAllocation alloc;
  if (width > child->size_request.width && !(child->pack_flags & ALTK_PACK_HFILL))
    {
      alloc.width = child->size_request.width;
      if (child->pack_flags & ALTK_PACK_ANCHOR_LEFT)
        alloc.x = x;
      else if (child->pack_flags & ALTK_PACK_ANCHOR_RIGHT)
        alloc.x = x + (width - child->size_request.width);
      else
        alloc.x = x + (width - child->size_request.width) / 2;
    }
  else
    {
      alloc.x = x;
      alloc.width = width;
    }
  if (height > child->size_request.height && !(child->pack_flags & ALTK_PACK_VFILL))
    {
      alloc.height = child->size_request.height;
      if (child->pack_flags & ALTK_PACK_ANCHOR_TOP)
        alloc.y = y;
      else if (child->pack_flags & ALTK_PACK_ANCHOR_BOTTOM)
        alloc.y = y + (height - child->size_request.height);
      else
        alloc.y = y + (height - child->size_request.height) / 2;
    }
  else
    {
      alloc.y = y;
      alloc.height = height;
    }
  altk_widget_size_allocate(child->widget, &alloc);
}



static void _size_allocate_v ( AltkWidget *wid,
                               AltkAllocation *alloc )
{
  Private *priv = PRIVATE(wid);
  GList *l;
  gint offset = priv->border_size;
  gboolean grow=FALSE, shrink=FALSE;
  AltkSizeDistrib distrib;
  if (alloc->height > wid->size_request.height) {
    grow = TRUE;
    if (priv->nexpands > 0)
      altk_size_distrib_init(&distrib, alloc->height - wid->size_request.height, priv->nexpands);
  } else if (alloc->height < wid->size_request.height) {
    shrink = TRUE;
    altk_size_distrib_init(&distrib, wid->size_request.height - alloc->height, priv->nchildren);
  }
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      gint x, y, width, height;
      if (!ALTK_WIDGET_VISIBLE(child->widget))
        continue;
      x = alloc->x + priv->border_size;
      y = alloc->y + offset;
      width = alloc->width - 2 * priv->border_size;
      height = child->size_request.height;
      if (grow && (child->pack_flags & ALTK_PACK_VEXPAND)) {
        height += altk_size_distrib_next(&distrib);
      } else if (shrink) {
        height -= altk_size_distrib_next(&distrib);
      }
      if (width <= 0 || height <= 0)
        width = height = 0;
      offset += height + priv->padding;
      _alloc_child(wid, child, x, y, width, height);
    }
}



static void _size_allocate_h ( AltkWidget *wid,
                               AltkAllocation *alloc )
{
  Private *priv = PRIVATE(wid);
  GList *l;
  gint offset = priv->border_size;
  gboolean grow=FALSE, shrink=FALSE;
  AltkSizeDistrib distrib;
  if (alloc->width > wid->size_request.width) {
    grow = TRUE;
    if (priv->nexpands > 0)
      altk_size_distrib_init(&distrib, alloc->width - wid->size_request.width, priv->nexpands);
  } else if (alloc->width < wid->size_request.width) {
    shrink = TRUE;
    altk_size_distrib_init(&distrib, wid->size_request.width - alloc->width, priv->nchildren);
  }
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      gint x, y, width, height;
      if (!ALTK_WIDGET_VISIBLE(child->widget))
        continue;
      x = alloc->x + offset;
      y = alloc->y + priv->border_size;
      width = child->size_request.width;
      height = alloc->height - 2 * priv->border_size;
      if (grow && (child->pack_flags & ALTK_PACK_HEXPAND)) {
        width += altk_size_distrib_next(&distrib);
      } else if (shrink) {
        width -= altk_size_distrib_next(&distrib);
      }
      if (width <= 0 || height <= 0)
        width = height = 0;
      offset += width + priv->padding;
      _alloc_child(wid, child, x, y, width, height);
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
  Child *c;
  altk_container_add(ALTK_CONTAINER(box), child);
  c = get_child(box, child);
  ASSERT(c);
  c->pack_flags = flags;
}
