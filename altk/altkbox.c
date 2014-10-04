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
}
  Child;



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
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
  /* [fixme] prop default */
  ALTK_BOX(obj)->orientation = ALTK_VERTICAL;
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
  box->orientation = orientation;
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
      return L_OBJECT(l_int_new(ALTK_BOX(object)->orientation));
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
  box->orientation = orientation;
  altk_widget_queue_resize(ALTK_WIDGET(box));
}



#define SIZE_REQUEST(X, Y, WIDTH, HEIGHT, wid, req) do {        \
    GList *l;                                                   \
    guint n_visible = 0;                                        \
    req->WIDTH = 4;                                             \
    req->HEIGHT = 4;                                            \
    for (l = ALTK_BOX(wid)->children; l; l = l->next)           \
      {                                                         \
        Child *child = l->data;                                 \
        AltkRequisition child_req;                              \
        if (!ALTK_WIDGET_VISIBLE(child->widget))                \
          continue;                                             \
        n_visible++;                                            \
        altk_widget_size_request(child->widget, &child_req);    \
        req->WIDTH = MAX(req->WIDTH, child_req.WIDTH + 4);      \
        req->HEIGHT += child_req.HEIGHT;                        \
      }                                                         \
    if (n_visible > 0)                                          \
      req->HEIGHT += (2 * (n_visible - 1));                     \
  } while (0)



#define SIZE_ALLOCATE(X, Y, WIDTH, HEIGHT, wid, alloc) do {         \
    GList *l;                                                       \
    guint ofs = 2;                                                  \
    for (l = ALTK_BOX(wid)->children; l; l = l->next)               \
      {                                                             \
        Child *child = l->data;                                     \
        AltkAllocation child_alloc;                                 \
        if (!ALTK_WIDGET_VISIBLE(child->widget))                    \
          continue;                                                 \
        child_alloc.X = alloc->X + 2;                               \
        child_alloc.Y = alloc->Y + ofs;                             \
        child_alloc.WIDTH = alloc->WIDTH - 4;                       \
        child_alloc.HEIGHT = child->widget->size_request.HEIGHT;    \
        ofs += (2 + child->widget->size_request.HEIGHT);            \
        altk_widget_size_allocate(child->widget, &child_alloc);     \
      }                                                             \
  } while (0)



/* _size_request:
 */
static void _size_request ( AltkWidget *wid,
                            AltkRequisition *req )
{
  switch (ALTK_BOX(wid)->orientation)
    {
    case ALTK_VERTICAL:
      SIZE_REQUEST(x, y, width, height, wid, req);
      break;
    case ALTK_HORIZONTAL:
      SIZE_REQUEST(y, x, height, width, wid, req);
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
  CL_DEBUG("[TODO] box size allocate: %d, %d, %d, %d",
           alloc->x, alloc->y, alloc->width, alloc->height);
  /* [FIXME] chain to parent_class method */
  wid->x = alloc->x;
  wid->y = alloc->y;
  wid->width = alloc->width;
  wid->height = alloc->height;
  /* children */
  switch (ALTK_BOX(wid)->orientation)
    {
    case ALTK_VERTICAL:
      SIZE_ALLOCATE(x, y, width, height, wid, alloc);
      break;
    case ALTK_HORIZONTAL:
      SIZE_ALLOCATE(y, x, height, width, wid, alloc);
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
  for (l = ALTK_BOX(widget)->children; l; l = l->next)
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
  ALTK_BOX(cont)->children = g_list_append(ALTK_BOX(cont)->children, bchild);
}



/* altk_box_pack_start:
 */
void altk_box_pack_start ( AltkBox *box,
                           AltkWidget *child,
                           AltkPackFlags flags )
{
  altk_container_add(ALTK_CONTAINER(box), child);
}
