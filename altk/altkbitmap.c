/* altkbitmap.c -
 */

#include "altk/private.h"
#include "altk/altkbitmap.h"
#include "altk/altkdisplay.h"
#include "altk/altkgc.h"
#include "altk/altkbitmap.inl"



static void _on_draw_text ( AltkDrawable *drawable,
                            AltkGC *gc,
                            gint x,
                            gint y,
                            const gchar *text );



/* altk_bitmap_class_init:
 */
static void altk_bitmap_class_init ( LObjectClass *cls )
{
  ((AltkDrawableClass *) cls)->draw_text = _on_draw_text;
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
  CL_DEBUG("[TODO] draw_text(%d, %d, \"%s\")", x, y, text);
}
