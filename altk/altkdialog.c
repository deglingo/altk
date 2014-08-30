/* altkdialog.c -
 */

#include "altk/private.h"
#include "altk/altkdialog.h"
#include "altk/altkdisplay.h"

#include "altk/altkdialog.inl"



static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req );
/* static void _on_size_allocate ( AltkWidget *wid, */
/*                                 AltkAllocation *alloc ); */



/* altk_dialog_class_init:
 */
static void altk_dialog_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->size_request = _on_size_request;
  /* ((AltkWidgetClass *) cls)->size_allocate = _on_size_allocate; */
}



/* altk_dialog_new:
 */
AltkWidget *altk_dialog_new ( AltkDisplay *display )
{
  AltkWidget *dlg;
  dlg = ALTK_WIDGET(l_object_new(ALTK_CLASS_DIALOG, NULL));
  altk_display_attach_widget(display, dlg);
  return dlg;
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



/* /\* _on_size_allocate: */
/*  *\/ */
/* static void _on_size_allocate ( AltkWidget *wid, */
/*                                 AltkAllocation *alloc ) */
/* { */
/*   CL_DEBUG("[TODO] dialog size allocate: %d, %d, %d, %d", */
/*            alloc->x, alloc->y, alloc->width, alloc->height); */
/* } */
