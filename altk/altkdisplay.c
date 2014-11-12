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



/* Signals:
 */
enum
  {
    SIG_OPEN,
    SIG_COUNT,
  };

static LSignalID signals[SIG_COUNT] = { 0, };



/* altk_display_class_init:
 */
static void altk_display_class_init ( LObjectClass *cls )
{
  signals[SIG_OPEN] = l_signal_new(cls,
                                   "open");
}



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
  /* create our own backbuf */
  display->backbuf = al_create_bitmap(al_get_display_width(display->al_display),
                                      al_get_display_height(display->al_display));
}



/* altk_display_open:
 */
void altk_display_open ( AltkDisplay *display )
{
  if (altk_display_is_open(display))
    return;
  ASSERT(!display->al_display);
  /* create the ALLEGRO_DISPLAY */
  _create_al_display(display);
  display->update_area = altk_region_new();
  /* register the display */
  altk_wm_register_display(display);
  /* register the display event source */
  altk_wm_register_al_source(al_get_display_event_source(display->al_display));
  /* create the root window */
  display->root_window = altk_window_new_root(display);
  /* signal */
  l_signal_emit(L_OBJECT(display), signals[SIG_OPEN], 0);
}



/* altk_display_is_open:
 */
gboolean altk_display_is_open ( AltkDisplay *display )
{
  return display->al_display ? TRUE : FALSE;
}



/* altk_display_get_size:
 */
void altk_display_get_size ( AltkDisplay *display,
                             gint *width,
                             gint *height )
{
  ASSERT(display->al_display);
  *width = al_get_display_width(display->al_display);
  *height = al_get_display_height(display->al_display);
}



/* altk_display_get_root_window:
 */
struct _AltkWindow *altk_display_get_root_window ( AltkDisplay *display )
{
  return display->root_window;
}



/* altk_display_flip:
 */
void altk_display_flip ( AltkDisplay *display )
{
  gint r;
  AltkRegionBox *box;
  AltkRegion *area;
  area = display->update_area;
  display->update_area = altk_region_new();
  ALLEGRO_STATE state;
  ALTK_WINDOW_DRAW_UPDATE(display->root_window, area, 0x00ff00);
  al_store_state(&state, ALLEGRO_STATE_DISPLAY | ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_backbuffer(display->al_display);
  for (r = 0, box = area->rects; r < area->n_rects; r++, box++)
    {
      al_draw_bitmap_region(display->backbuf,
                            box->x1,
                            box->y1,
                            box->x2 - box->x1,
                            box->y2 - box->y1,
                            box->x1,
                            box->y1, 
                            0);
    }
  al_flip_display();
  al_restore_state(&state);
  altk_region_destroy(area);
}



/* altk_display_get_backbuf:
 */
ALLEGRO_BITMAP *altk_display_get_backbuf ( AltkDisplay *display )
{
  return display->backbuf;
}



/* altk_display_invalidate_region:
 */
void altk_display_invalidate_region ( AltkDisplay *display,
                                      AltkRegion *area )
{
  AltkRectangle rect;
  AltkRegion *clip;
  altk_region_union(display->update_area, area);
  rect.x = rect.y = 0;
  rect.width = al_get_display_width(display->al_display);
  rect.height = al_get_display_height(display->al_display);
  /* [FIXME] use a static region */
  clip = altk_region_rectangle(&rect);
  altk_region_intersect(display->update_area, clip);
  altk_region_destroy(clip);
}



/* altk_display_invalidate_area:
 */
void altk_display_invalidate_area ( AltkDisplay *display,
                                    gint x,
                                    gint y,
                                    gint width,
                                    gint height )
{
  /* [FIXME] use a static region */
  AltkRegion *region;
  AltkRectangle rect;
  rect.x = x;
  rect.y = y;
  rect.width = width;
  rect.height = height;
  region = altk_region_rectangle(&rect);
  altk_display_invalidate_region(display, region);
  altk_region_destroy(region);
}



/* altk_display_draw_bitmap_region:
 */
void altk_display_draw_bitmap_region ( AltkDisplay *display,
                                       AltkBitmap *bitmap,
                                       AltkRegion *region,
                                       gint src_x,
                                       gint src_y,
                                       gint dest_x,
                                       gint dest_y )
{
  gint r;
  AltkRegionBox *box;
  ALLEGRO_STATE state;
  ALLEGRO_BITMAP *source;
  source = altk_drawable_get_target(ALTK_DRAWABLE(bitmap));
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(display->backbuf);
  for (r = 0, box = region->rects; r < region->n_rects; r++, box++)
    {
      gint dx = box->x1 + dest_x;
      gint dy = box->y1 + dest_y;
      gint width = box->x2 - box->x1;
      gint height = box->y2 - box->y1;
      al_draw_bitmap_region(source,
                            box->x1 + src_x,
                            box->y1 + src_y,
                            width,
                            height,
                            dx,
                            dy,
                            0);
      altk_display_invalidate_area(display,
                                   dx,
                                   dy,
                                   width,
                                   height);
    }
  al_restore_state(&state);
}
