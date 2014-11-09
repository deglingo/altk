/* altkbitmap.c -
 */

#include "altk/private.h"
#include "altk/altkbitmap.h"
#include "altk/altkdisplay.h"
#include "altk/altkregion.h"
#include "altk/altkgc.h"
#include "altk/altkfont.h"
#include "altk/altkbitmap.inl"



static gpointer get_target ( AltkDrawable *drawable );
static void get_size ( AltkDrawable *drawable,
                       gint *width,
                       gint *height );



/* altk_bitmap_class_init:
 */
static void altk_bitmap_class_init ( LObjectClass *cls )
{
  ALTK_DRAWABLE_CLASS(cls)->get_target = get_target;
  ALTK_DRAWABLE_CLASS(cls)->get_size = get_size;
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



/* get_target:
 */
static gpointer get_target ( AltkDrawable *drawable )
{
  return ALTK_BITMAP(drawable)->al_bitmap;
}



/* get_size:
 */
static void get_size ( AltkDrawable *drawable,
                       gint *width,
                       gint *height )
{
  *width = al_get_bitmap_width(ALTK_BITMAP(drawable)->al_bitmap);
  *height = al_get_bitmap_height(ALTK_BITMAP(drawable)->al_bitmap);
}
