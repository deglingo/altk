/* altkbitmap.c -
 */

#include "altk/private.h"
#include "altk/altkbitmap.h"
#include "altk/altkdisplay.h"
#include "altk/altkregion.h"
#include "altk/altkgc.h"
#include "altk/altkfont.h"
#include "altk/altkbitmap.inl"



static void _on_draw_text ( AltkDrawable *drawable,
                            AltkGC *gc,
                            gint x,
                            gint y,
                            const gchar *text );
static void _on_draw_bitmap_region ( AltkDrawable *drawable,
                                     AltkBitmap *bitmap,
                                     AltkRegion *region,
                                     gint dest_x,
                                     gint dest_y );



/* altk_bitmap_class_init:
 */
static void altk_bitmap_class_init ( LObjectClass *cls )
{
  ((AltkDrawableClass *) cls)->draw_text = _on_draw_text;
  ((AltkDrawableClass *) cls)->draw_bitmap_region = _on_draw_bitmap_region;
}



/* altk_bitmap_new:
 */
AltkDrawable *altk_bitmap_new ( AltkDisplay *display,
                                gint width,
                                gint height )
{
  AltkDrawable *d;
  ALLEGRO_STATE state;
  d = ALTK_DRAWABLE(l_object_new(ALTK_CLASS_BITMAP, NULL));
  /* [FIXME] */
  ASSERT(display->al_display);
  al_store_state(&state, 
                 ALLEGRO_STATE_DISPLAY |
                 ALLEGRO_STATE_TARGET_BITMAP |
                 ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
  al_set_target_backbuffer(display->al_display);
  al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY);
  al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
  ALTK_BITMAP(d)->al_bitmap = al_create_bitmap(width, height);
  ALTK_BITMAP(d)->destroy = TRUE;
  al_restore_state(&state);
  return d;
}



/* altk_bitmap_new_from_al_bitmap:
 */
AltkDrawable *altk_bitmap_new_from_al_bitmap ( ALLEGRO_BITMAP *bmp,
                                               gboolean destroy )
{
  AltkDrawable *d;
  d = ALTK_DRAWABLE(l_object_new(ALTK_CLASS_BITMAP, NULL));
  ALTK_BITMAP(d)->al_bitmap = bmp;
  ALTK_BITMAP(d)->destroy = destroy;
  return d;
}



/* _on_draw_text:
 */
static void _on_draw_text ( AltkDrawable *drawable,
                            AltkGC *gc,
                            gint x,
                            gint y,
                            const gchar *text )
{
  ALLEGRO_COLOR color = al_map_rgb(255, 255, 0);
  ALLEGRO_STATE state;
  CL_DEBUG("draw_text(%d, %d, \"%s\")", x, y, text);
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(ALTK_BITMAP(drawable)->al_bitmap);
  al_draw_text(gc->font->al_font, color, x, y, ALLEGRO_ALIGN_LEFT, text);
  al_restore_state(&state);
}



/* _on_draw_bitmap_region:
 */
static void _on_draw_bitmap_region ( AltkDrawable *drawable,
                                     AltkBitmap *bitmap,
                                     AltkRegion *region,
                                     gint dest_x,
                                     gint dest_y )
{
  gint r;
  AltkRegionBox *box;
  ALLEGRO_STATE state;
  CL_DEBUG("draw_bitmap_region(%p, %p, %p, %d, %d)",
           drawable, bitmap, region, dest_x, dest_y);
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(ALTK_BITMAP(drawable)->al_bitmap);
  for (r = 0, box = region->rects; r < region->n_rects; r++, box++)
    {
      al_draw_bitmap_region(bitmap->al_bitmap,
                            box->x1,
                            box->y1,
                            box->x2 - box->x1,
                            box->y2 - box->y1,
                            box->x1 + dest_x,
                            box->y1 + dest_y,
                            0);
    }
  al_restore_state(&state);
}
