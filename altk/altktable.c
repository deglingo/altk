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
  gboolean expand;
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
static gint layout_n_hexpands ( Layout *layout );
static gint layout_n_vexpands ( Layout *layout );



/* Child:
 */
typedef struct _Child
{
  AltkWidget *widget;
  gint top;
  gint left;
  gint bottom;
  gint right;
  AltkPackFlags pack_flags;
  AltkRequisition request;
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
  gboolean xprows=FALSE, xpcols=FALSE;
  /* rows */
  if (top < layout->top) {
    layout->top = top;
    xprows = TRUE;
  }
  if (bottom > layout->bottom) {
    layout->bottom = bottom;
    xprows = TRUE;
  }
  if (xprows)
    layout->rows = g_realloc(layout->rows, sizeof(LineLayout) * (layout->bottom - layout->top));
  /* cols */
  if (left < layout->left) {
    layout->left = left;
    xpcols = TRUE;
  }
  if (right > layout->right) {
    layout->right = right;
    xpcols = TRUE;
  }
  if (xpcols)
    layout->cols = g_realloc(layout->cols, sizeof(LineLayout) * (layout->right - layout->left));
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
    return &layout->cols[col - layout->left];
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
    return &layout->rows[row - layout->top];
  else
    return NULL;
}



/* layout_n_hexpands:
 */
static gint layout_n_hexpands ( Layout *layout )
{
  gint n = 0;
  gint l;
  for (l = layout->left; l < layout->right; l++) {
    if (layout_col(layout, l)->expand)
      n++;
  }
  return n;
}



/* layout_n_vexpands:
 */
static gint layout_n_vexpands ( Layout *layout )
{
  gint n = 0;
  gint l;
  for (l = layout->top; l < layout->bottom; l++) {
    if (layout_row(layout, l)->expand)
      n++;
  }
  return n;
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
      AltkSizeDistrib distrib;
      if (!ALTK_WIDGET_VISIBLE(child->widget))
        continue;
      /* CL_DEBUG("child request: %s", l_object_to_string(L_OBJECT(child->widget))); */
      altk_widget_size_request(child->widget, &child->request);
      /* columns layout */
      ncols = child->right - child->left;
      dsize = child->request.width - priv->hpad * (ncols - 1);
      /* CL_DEBUG(" -> ncols=%d, dsize=%d", ncols, dsize); */
      if (dsize > 0)
        {
          altk_size_distrib_init(&distrib, dsize, ncols);
          for (i = child->left; i < child->right; i++)
            {
              LineLayout *ll = layout_col(layout, i);
              gint dist_size = altk_size_distrib_next(&distrib);
              ll->size_req = MAX(ll->size_req, dist_size);
              if (child->pack_flags & ALTK_PACK_HEXPAND)
                ll->expand = TRUE;
              /* CL_DEBUG(" -> col[%d].size_req = %d", i, ll->size_req); */
            }
        }
      /* rows layout */
      nrows = child->bottom - child->top;
      dsize = child->request.height - priv->vpad * (nrows - 1);
      /* CL_DEBUG(" -> nrows=%d, dsize=%d", nrows, dsize); */
      if (dsize > 0)
        {
          altk_size_distrib_init(&distrib, dsize, nrows);
          for (i = child->top; i < child->bottom; i++)
            {
              LineLayout *ll = layout_row(layout, i);
              gint dist_size = altk_size_distrib_next(&distrib);
              ll->size_req = MAX(ll->size_req, dist_size);
              if (child->pack_flags & ALTK_PACK_VEXPAND)
                ll->expand = TRUE;
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



static void _alloc_child ( AltkWidget *wid,
                           Child *child,
                           gint x,
                           gint y,
                           gint width,
                           gint height )
{
  AltkAllocation alloc;
  if (width > child->request.width && !(child->pack_flags & ALTK_PACK_HFILL))
    {
      if (child->pack_flags & ALTK_PACK_ANCHOR_LEFT)
        alloc.x = x;
      else if (child->pack_flags & ALTK_PACK_ANCHOR_RIGHT)
        alloc.x = x + (width - child->request.width);
      else
        alloc.x = x + (width - child->request.width) / 2;
      alloc.width = child->request.width;
    }
  else
    {
      alloc.x = x;
      alloc.width = width;
    }
  if (height > child->request.height && !(child->pack_flags & ALTK_PACK_VFILL))
    {
      if (child->pack_flags & ALTK_PACK_ANCHOR_TOP)
        alloc.y = y;
      else if (child->pack_flags & ALTK_PACK_ANCHOR_BOTTOM)
        alloc.y = y + (height - child->request.height);
      else
        alloc.y = y + (height - child->request.height) / 2;
      alloc.height = child->request.height;
    }
  else
    {
      alloc.y = y;
      alloc.height = height;
    }
  altk_widget_size_allocate(child->widget, &alloc);
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
  AltkSizeDistrib hdistrib, vdistrib;
  gboolean hgrow=FALSE, vgrow=FALSE, hshrink=FALSE, vshrink=FALSE;
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(wid, alloc);
  /* hgrow/shrink */
  if (alloc->width > wid->size_request.width)
    {
      hgrow = TRUE;
      altk_size_distrib_init(&hdistrib, alloc->width - wid->size_request.width, layout_n_hexpands(layout));
    }
  else if (alloc->width < wid->size_request.width)
    {
      hshrink = TRUE;
      altk_size_distrib_init(&hdistrib, wid->size_request.width - alloc->width, layout->right - layout->left);
    }
  /* vgrow/shrink */
  if (alloc->height > wid->size_request.height)
    {
      vgrow = TRUE;
      altk_size_distrib_init(&vdistrib, alloc->height - wid->size_request.height, layout_n_vexpands(layout));
    }
  else if (alloc->height < wid->size_request.height)
    {
      vshrink = TRUE;
      altk_size_distrib_init(&vdistrib, wid->size_request.height - alloc->height, layout->bottom - layout->top);
    }
  /* prepare cols/rows */
  ofs = 0;
  for (i = layout->left; i < layout->right; i++)
    {
      LineLayout *ll = layout_col(layout, i);
      ll->pos1 = ofs;
      ll->pos2 = ofs + ll->size_req;
      if (hgrow && ll->expand)
        ll->pos2 += altk_size_distrib_next(&hdistrib);
      else if (hshrink)
        ll->pos2 -= altk_size_distrib_next(&hdistrib);
      ofs = ll->pos2 + priv->hpad;
    }
  ofs = 0;
  for (i = layout->top; i < layout->bottom; i++)
    {
      LineLayout *ll = layout_row(layout, i);
      ll->pos1 = ofs;
      ll->pos2 = ofs + ll->size_req;
      if (vgrow && ll->expand)
        ll->pos2 += altk_size_distrib_next(&vdistrib);
      else if (vshrink)
        ll->pos2 -= altk_size_distrib_next(&vdistrib);
      ofs = ll->pos2 + priv->vpad;
    }
  /* alloc children */
  for (l = priv->children; l; l = l->next)
    {
      Child *child = l->data;
      gint x, y, width, height;
      if (!ALTK_WIDGET_VISIBLE(child->widget))
        continue;
      x = layout_col(layout, child->left)->pos1;
      y = layout_row(layout, child->top)->pos1;
      width = layout_col(layout, child->right - 1)->pos2 - x;
      height = layout_row(layout, child->bottom - 1)->pos2 - y;
      x += alloc->x;
      y += alloc->y;
      _alloc_child(wid, child, x, y, width, height);
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
  tchild->pack_flags = flags;
  layout_extend(PRIVATE(table)->layout, top, left, bottom, right);
}
