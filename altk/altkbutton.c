/* altkbutton.h -
 */

#include "altk/private.h"
#include "altk/altkbutton.h"
#include "altk/altklabel.h"
#include "altk/altkbutton.inl"



static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req );
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc );



/* altk_button_class_init:
 */
static void altk_button_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->size_request = _on_size_request;
  ((AltkWidgetClass *) cls)->size_allocate = _on_size_allocate;
}



/* altk_button_new_with_label:
 */
AltkWidget *altk_button_new_with_label ( const gchar *text )
{
  AltkWidget *but, *lbl;
  but = ALTK_WIDGET(l_object_new(ALTK_CLASS_BUTTON, NULL));
  altk_widget_set_event_mask(but, ALTK_EVENT_EXPOSE | ALTK_EVENT_MOUSE_ENTER);
  lbl = altk_label_new(text);
  altk_container_add(ALTK_CONTAINER(but), lbl);
  /* [FIXME] l_object_unref(lbl); */
  return but;
}



/* _on_size_request:
 */
static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req )
{
  if (ALTK_BIN(wid)->child) {
    altk_widget_size_request(ALTK_BIN(wid)->child, req);
    req->width += 4;
    req->height += 4;
  } else {
    req->width = req->height = 4;
  }
}



/* _on_size_allocate:
 */
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc )
{
  CL_DEBUG("[TODO] dialog size allocate: %d, %d, %d, %d",
           alloc->x, alloc->y, alloc->width, alloc->height);
  /* [FIXME] chain to parent_class method */
  wid->x = alloc->x;
  wid->y = alloc->y;
  wid->width = alloc->width;
  wid->height = alloc->height;
  /* update root_x/y */
  if (wid->parent) {
    wid->root_x = wid->parent->root_x + alloc->x;
    wid->root_y = wid->parent->root_y + alloc->y;
  } else {
    wid->root_x = alloc->x;
    wid->root_y = alloc->y;
  }
  /* ---- */
  if (ALTK_BIN(wid)->child) {
    AltkAllocation child_alloc;
    child_alloc.x = 2;
    child_alloc.y = 2;
    child_alloc.width = alloc->width - 4;
    child_alloc.height = alloc->height - 4;
    altk_widget_size_allocate(ALTK_BIN(wid)->child, &child_alloc);
  }
}
