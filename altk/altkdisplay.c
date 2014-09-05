/* altkdisplay.c -
 */

#include "altk/private.h"
#include "altk/altkdisplay.h"
#include "altk/altkmain.h"
#include "altk/altkevent.h"
#include "altk/altkbitmap.h"
#include "altk/altkwindow.h"
#include "altk/altkwm.h"
/* [REMOVEME] */
#include "altk/altkwidget.h"
#include "altk/altkdialog.h"

#include "altk/altkdisplay.inl"



/* altk_display_new:
 */
AltkDisplay *altk_display_new ( void )
{
  AltkDisplay *display;
  display = ALTK_DISPLAY(l_object_new(ALTK_CLASS_DISPLAY, NULL));
  return display;
}



/* _create_al_display:
 */
static void _create_al_display ( AltkDisplay *display )
{
  ALLEGRO_STATE state;
  al_store_state(&state, ALLEGRO_STATE_DISPLAY | ALLEGRO_STATE_NEW_DISPLAY_PARAMETERS | ALLEGRO_STATE_TARGET_BITMAP);
  al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_REQUIRE);
  /* al_set_new_display_option(ALLEGRO_SWAP_METHOD, 1, ALLEGRO_REQUIRE); */
  if (!(display->al_display = al_create_display(640, 480)))
    CL_ERROR("could not create allegro display");
  al_restore_state(&state);
  CL_DEBUG("allegro display created:");
  CL_DEBUG(" - size         : %dx%d",
           al_get_display_width(display->al_display),
           al_get_display_height(display->al_display));
  CL_DEBUG(" - color_size   : %d", al_get_display_option(display->al_display, ALLEGRO_COLOR_SIZE));
  CL_DEBUG(" - render_method: %d", al_get_display_option(display->al_display, ALLEGRO_RENDER_METHOD));
  CL_DEBUG(" - single_buffer: %d", al_get_display_option(display->al_display, ALLEGRO_SINGLE_BUFFER));
  CL_DEBUG(" - swap_method  : %d", al_get_display_option(display->al_display, ALLEGRO_SWAP_METHOD));
  if (!al_get_display_option(display->al_display, ALLEGRO_SINGLE_BUFFER) == 1)
    CL_ERROR("[FIXME] display is not single-buffered!");
}



/* altk_display_open:
 */
void altk_display_open ( AltkDisplay *display )
{
  GList *l;
  ASSERT(!display->al_display);
  /* create the ALLEGRO_DISPLAY */
  _create_al_display(display);
  /* create our own backbuffer */
  display->backbuf = altk_bitmap_new(display,
                                     al_get_display_width(display->al_display),
                                     al_get_display_height(display->al_display));
  /* [REMOVEME] (and create some bitmap in the debug_update func) */
#ifdef DEBUG_UPDATES
  display->debugbuf = altk_bitmap_new(display,
                                     al_get_display_width(display->al_display),
                                     al_get_display_height(display->al_display));
#endif
  /* register the display */
  altk_wm_register_display(display);
  /* register the display event source */
  altk_wm_register_al_source(al_get_display_event_source(display->al_display));
  /* create the root window */
  display->root_window = altk_window_new_root(display);
  /* [FIXME] just a trick waiting for proper signal implementation */
  for (l = display->top_widgets; l; l = l->next)
    _altk_dialog_handle_open_display(ALTK_DIALOG(l->data));
}



/* altk_display_is_open:
 */
gboolean altk_display_is_open ( AltkDisplay *display )
{
  return display->al_display ? TRUE : FALSE;
}



/* altk_display_attach_widget:
 *
 * [REMOVEME] when we have signals
 */
void altk_display_attach_widget ( AltkDisplay *display,
                                  struct _AltkWidget *widget )
{
  display->top_widgets = g_list_append(display->top_widgets,
                                       l_object_ref(widget));
}



/* altk_display_get_root_window:
 */
struct _AltkWindow *altk_display_get_root_window ( AltkDisplay *display )
{
  return display->root_window;
}
