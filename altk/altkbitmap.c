/* altkbitmap.c -
 */

#include "altk/private.h"
#include "altk/altkbitmap.h"
#include "altk/altkdisplay.h"
#include "altk/altkbitmap.inl"



/* altk_bitmap_class_init:
 */
static void altk_bitmap_class_init ( LObjectClass *cls )
{
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
  al_restore_state(&state);
  return d;
}
