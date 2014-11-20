/* altkdialog.c -
 */

#include "altk/private.h"
#include "altk/altkdialog.h"
#include "altk/altkdisplay.h"
#include "altk/altkbitmap.h" /* [REMOVEME] */
#include "altk/altkwindow.h"
#include "altk/altkstyle.h"
#include "altk/altkwm.h"

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
  altk_widget_set_event_mask(ALTK_WIDGET(obj), ALTK_EVENT_MASK_EXPOSE);
  altk_wm_register_top_widget(ALTK_WIDGET(obj));
}



/* altk_dialog_new:
 */
AltkWidget *altk_dialog_new ( AltkDisplay *display )
{
  AltkWidget *dlg;
  dlg = ALTK_WIDGET(l_object_new(ALTK_CLASS_DIALOG, NULL));
  if (display)
    altk_dialog_set_display(ALTK_DIALOG(dlg), display);
  return dlg;
}



/* _on_display_open:
 */
void _on_display_open ( AltkDisplay *display,
                        AltkDialog *dlg )
{
  ASSERT(display == dlg->display);
  altk_widget_map(ALTK_WIDGET(dlg));
  if (ALTK_WIDGET_VISIBLE(dlg))
    {
      /* [FIXME] process resize immediately ? */
      altk_widget_queue_resize(ALTK_WIDGET(dlg));
      altk_widget_realize(ALTK_WIDGET(dlg));
      altk_widget_queue_draw(ALTK_WIDGET(dlg));
    }
}



/* altk_dialog_set_display:
 */
void altk_dialog_set_display ( AltkDialog *dlg,
                               struct _AltkDisplay *display )
{
  ASSERT(!dlg->display); /* [TODO] */
  /* atach dialog <> display */
  ALTK_DIALOG(dlg)->display = display;
  l_signal_connect(L_OBJECT(display), "open", (LSignalHandler) _on_display_open, dlg, NULL);
  /* process immediately if the display is already open */
  if (altk_display_is_open(display))
    _on_display_open(display, dlg);
}



/* _on_get_display:
 */
static AltkDisplay *_on_get_display ( AltkWidget *widget )
{
  return ALTK_DIALOG(widget)->display;
}



/* altk_dialog_set_size_hints:
 */
void altk_dialog_set_size_hints ( AltkDialog *dlg,
                                  AltkSizeHints hints )
{
  altk_wm_set_top_widget_size_hints(ALTK_WIDGET(dlg), hints);
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
  /* CL_DEBUG("[TODO] dialog size allocate: %d, %d, %d, %d", */
  /*          alloc->x, alloc->y, alloc->width, alloc->height); */
  ALTK_WIDGET_CLASS(parent_class)->size_allocate(wid, alloc);
  /* ---- */
  if (ALTK_BIN_CHILD_VISIBLE(wid)) {
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
  AltkStyleContext *ctxt = altk_widget_get_style_context(wid);
  /* CL_DEBUG("Dialog.expose_event()"); */
  altk_style_context_draw_box(ctxt,
                              event->expose.gc,
                              0,
                              0,
                              wid->width,
                              wid->height);
}
