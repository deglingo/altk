/* altktable.c -
 */

#include "altk/private.h"
#include "altk/altktable.h"
#include "altk/altkwindow.h"
#include "altk/altktable.inl"



/* LineLayout:
 */
typedef struct _LineLayout
{
  gint size_req;
  gint pos1;
  gint pos2;
}
  LineLayout;



/* Layout:
 */
typedef struct _Layout
{
  /* extents */
  gint top;
  gint left;
  gint bottom;
  gint right;
  /* rows */
  LineLayout *rows;
  /* cols */
  LineLayout *cols;
}
  Layout;



/* Private:
 */
typedef struct _Private
{
  guint hpad;
  guint vpad;
  GList *children;
  Layout *layout;
}
  Private;

#define PRIVATE(table) ((Private *) (ALTK_TABLE(table)->private))



static Layout *layout_new ( void );
static void layout_extend ( Layout *layout,
                            gint top,
                            gint left,
                            gint bottom,
                            gint right );
static void layout_reset ( Layout *layout );
static LineLayout *layout_col ( Layout *layout,
                                gint col );
static LineLayout *layout_row ( Layout *layout,
                                gint row );



/* Child:
 */
typedef struct _Child
{
  AltkWidget *widget;
  gint top;
  gint left;
  gint bottom;
  gint right;
  AltkPackFlags flags;
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



/* layout_new:
 */
static Layout *layout_new ( void )
{
  Layout *layout = g_new0(Layout, 1);
  /* [FIXME] */
  layout->top = 0;
  layout->left = 0;
  layout->bottom = 0;
  layout->right = 0;
  layout->cols = g_new0(LineLayout, 16);
  layout->rows = g_new0(LineLayout, 16);
  return layout;
}



/* layout_extend:
 */
static void layout_extend ( Layout *layout,
                            gint top,
                            gint left,
                            gint bottom,
                            gint right )
{
  /* [TODO] */
  ASSERT(top >= 0);
  ASSERT(left >= 0);
  ASSERT(right < 16);
  ASSERT(bottom < 16);
  layout->top = MIN(layout->top, top);
  layout->left = MIN(layout->left, left);
  layout->bottom = MAX(layout->bottom, bottom);
  layout->right = MAX(layout->right, right);
}



/* layout_reset:
 */
static void layout_reset ( Layout *layout )
{
  gint i;
  for (i = layout->left; i < layout->right; i++)
    layout_col(layout, i)->size_req = 0;
  for (i = layout->top; i < layout->bottom; i++)
    layout_row(layout, i)->size_req = 0;
}



/* layout_col:
 */
static LineLayout *layout_col ( Layout *layout,
                                gint col )
{
  /* [FIXME] */
  if (col >= layout->left && col < layout->right)
    return &layout->cols[col];
  else
    return NULL;
}



/* layout_row:
 */
static LineLayout *layout_row ( Layout *layout,
                                gint row )
{
  /* [FIXME] */
  if (row >= layout->top && row < layout->bottom)
    return &layout->rows[row];
  else
    return NULL;
}



/* altk_table_init:
 */
static void altk_table_init ( LObject *obj )
{
  Private *priv;
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
  priv = ALTK_TABLE(obj)->private = g_new0(Private, 1);
  priv->hpad = 2;
  priv->vpad = 2;
  priv->layout = layout_new();
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
  AltkTable *table = ALTK_TABLE(wid);
  Private *priv = PRIVATE(table);
  Layout *layout = priv->layout;
  GList *l;
  gint i;
  layout_reset(layout);
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      gint ncols, nrows;
      guint dsize;
      AltkRequisition child_req;
      AltkSizeDistrib distrib;
      if (!ALTK_WIDGET_VISIBLE(child->widget))
        continue;
      /* CL_DEBUG("child request: %s", l_object_to_string(L_OBJECT(child->widget))); */
      altk_widget_size_request(child->widget, &child_req);
      /* columns layout */
      ncols = child->right - child->left;
      dsize = child_req.width - priv->hpad * (ncols - 1);
      /* CL_DEBUG(" -> ncols=%d, dsize=%d", ncols, dsize); */
      if (dsize > 0)
        {
          altk_size_distrib_init(&distrib, dsize, ncols);
          for (i = child->left; i < child->right; i++)
            {
              LineLayout *ll = layout_col(layout, i);
              gint dist_size = altk_size_distrib_next(&distrib);
              ll->size_req = MAX(ll->size_req, dist_size);
              /* CL_DEBUG(" -> col[%d].size_req = %d", i, ll->size_req); */
            }
        }
      /* rows layout */
      nrows = child->bottom - child->top;
      dsize = child_req.height - priv->vpad * (nrows - 1);
      /* CL_DEBUG(" -> nrows=%d, dsize=%d", nrows, dsize); */
      if (dsize > 0)
        {
          altk_size_distrib_init(&distrib, dsize, nrows);
          for (i = child->top; i < child->bottom; i++)
            {
              LineLayout *ll = layout_row(layout, i);
              gint dist_size = altk_size_distrib_next(&distrib);
              ll->size_req = MAX(ll->size_req, dist_size);
              /* CL_DEBUG(" -> row[%d].size_req = %d", i, ll->size_req); */
            }
        }
    }
  /* sum */
  req->width = priv->hpad * (layout->right - layout->left - 1);
  req->height = priv->vpad * (layout->bottom - layout->top - 1);
  for (i = layout->left; i < layout->right; i++)
    {
      LineLayout *ll = layout_col(layout, i);
      req->width += ll->size_req;
    }
  for (i = layout->top; i < layout->bottom; i++)
    {
      LineLayout *ll = layout_row(layout, i);
      req->height += ll->size_req;
    }
}



/* _size_allocate:
 */
static void _size_allocate ( AltkWidget *wid,
                             AltkAllocation *alloc )
{
  AltkTable *table = ALTK_TABLE(wid);
  Private *priv = PRIVATE(table);
  Layout *layout = priv->layout;
  gint i, ofs;
  GList *l;
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(wid, alloc);
  /* prepare cols/rows */
  ofs = 0;
  for (i = layout->left; i < layout->right; i++)
    {
      LineLayout *ll = layout_col(layout, i);
      ll->pos1 = ofs;
      ll->pos2 = ofs + ll->size_req;
      ofs = ll->pos2 + priv->hpad;
    }
  ofs = 0;
  for (i = layout->top; i < layout->bottom; i++)
    {
      LineLayout *ll = layout_row(layout, i);
      ll->pos1 = ofs;
      ll->pos2 = ofs + ll->size_req;
      ofs = ll->pos2 + priv->vpad;
    }
  /* alloc children */
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      AltkAllocation child_alloc;
      if (!ALTK_WIDGET_VISIBLE(child->widget))
        continue;
      child_alloc.x = layout_col(layout, child->left)->pos1;
      child_alloc.y = layout_row(layout, child->top)->pos1;
      child_alloc.width = layout_col(layout, child->right - 1)->pos2 - child_alloc.x;
      child_alloc.height = layout_row(layout, child->bottom - 1)->pos2 - child_alloc.y;
      child_alloc.x += alloc->x;
      child_alloc.y += alloc->y;
      altk_widget_size_allocate(child->widget, &child_alloc);
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



/* get_child:
 */
static Child *get_child ( AltkTable *table,
                          AltkWidget *widget )
{
  GList *l;
  for (l = PRIVATE(table)->children; l; l = l->next)
    {
      if (((Child *) (l->data))->widget == widget)
        return l->data;
    }
  return NULL;
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
  Child *tchild;
  /* [fixme] allow that ? */
  ASSERT(top < bottom && left < right);
  altk_container_add(ALTK_CONTAINER(table), child);
  tchild = get_child(table, child);
  tchild->top = top;
  tchild->left = left;
  tchild->bottom = bottom;
  tchild->right = right;
  tchild->flags = flags;
  layout_extend(PRIVATE(table)->layout, top, left, bottom, right);
}
