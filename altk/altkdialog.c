/* altkdialog.c -
 */

#include "altk/private.h"
#include "altk/altkdialog.h"
#include "altk/altkdisplay.h"
#include "altk/altkbitmap.h" /* [REMOVEME] */
#include "altk/altkwindow.h"

#include "altk/altkdialog.inl"



static AltkDisplay *_on_get_display ( AltkWidget *widget );
static void _on_size_request ( AltkWidget *wid,
                               AltkRequisition *req );
static void _on_size_allocate ( AltkWidget *wid,
                                AltkAllocation *alloc );
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event );



/* altk_dialog_class_init:
 */
static void altk_dialog_class_init ( LObjectClass *cls )
{
  ((AltkWidgetClass *) cls)->get_display = _on_get_display;
  ((AltkWidgetClass *) cls)->size_request = _on_size_request;
  ((AltkWidgetClass *) cls)->size_allocate = _on_size_allocate;
  ((AltkWidgetClass *) cls)->expose_event = _on_expose_event;
}



/* altk_dialog_init:
 */
static void altk_dialog_init ( LObject *obj )
{
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_TOP_WIDGET;
}



/* altk_dialog_new:
 */
AltkWidget *altk_dialog_new ( AltkDisplay *display )
{
  AltkWidget *dlg;
  dlg = ALTK_WIDGET(l_object_new(ALTK_CLASS_DIALOG, NULL));
  altk_widget_set_event_mask(dlg, ALTK_EVENT_EXPOSE);
  /* [fixme] atach dialog <> display */
  altk_display_attach_widget(display, dlg);
  ALTK_DIALOG(dlg)->display = display;
  return dlg;
}



/* _altk_dialog_handle_open_display:
 *
 * [REMOVEME] waiting for LSignal implementation
 */
void _altk_dialog_handle_open_display ( AltkDialog *dlg )
{
  if (!ALTK_WIDGET_VISIBLE(dlg))
    return;
  altk_widget_map(ALTK_WIDGET(dlg));
  /* [FIXME] process resize immediately ? */
  altk_widget_queue_resize(ALTK_WIDGET(dlg));
  altk_widget_realize(ALTK_WIDGET(dlg));
}



/* _on_get_display:
 */
static AltkDisplay *_on_get_display ( AltkWidget *widget )
{
  return ALTK_DIALOG(widget)->display;
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
  /* resize the window */
  if (ALTK_WIDGET_REALIZED(wid))
    {
      altk_window_set_bounds(wid->window, alloc->x, alloc->y, alloc->width, alloc->height);
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



/* _on_expose_event:
 */
static void _on_expose_event ( AltkWidget *wid,
                               AltkEvent *event )
{
  gint r;
  AltkRegionBox *box;
  gint cx, cy, cw, ch;
  /* [FIXME] use drawable methods */
  ALLEGRO_COLOR col = al_map_rgb(0, 255, 255);
  ALLEGRO_STATE state;
  al_store_state(&state, ALLEGRO_STATE_DISPLAY | ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(ALTK_BITMAP(event->expose.window)->al_bitmap);
  al_get_clipping_rectangle(&cx, &cy, &cw, &ch);
  for (r = 0, box = event->expose.area->rects;
       r < event->expose.area->n_rects;
       r++, box++)
    {
      al_set_clipping_rectangle(box->x1,
                                box->y1,
                                box->x2 - box->x1,
                                box->y2 - box->y1);
      al_clear_to_color(col);
    }
  al_set_clipping_rectangle(cx, cy, cw, ch);
  al_restore_state(&state);
}
