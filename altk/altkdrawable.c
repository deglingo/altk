/* altkdrawable.c -
 */

#include "altk/private.h"
#include "altk/altkdrawable.h"
#include "altk/altkdrawable.inl"



/* altk_drawable_class_init:
 */
static void altk_drawable_class_init ( LObjectClass *cls )
{
}



/* altk_drawable_set_offset:
 */
void altk_drawable_set_offset ( AltkDrawable *drawable,
                                gint ofs_x,
                                gint ofs_y )
{
  drawable->offset_x = ofs_x;
  drawable->offset_y = ofs_y;
}



/* altk_drawable_draw_text:
 */
void altk_drawable_draw_text ( AltkDrawable *drawable,
                               struct _AltkGC *gc,
                               gint x,
                               gint y,
                               const gchar *text )
{
  ASSERT(ALTK_DRAWABLE_GET_CLASS(drawable)->draw_text);
  ALTK_DRAWABLE_GET_CLASS(drawable)->draw_text(drawable, gc, x, y, text);
}



/* altk_drawable_draw_bitmap_region:
 */
void altk_drawable_draw_bitmap_region ( AltkDrawable *drawable,
                                        struct _AltkBitmap *bitmap,
                                        struct _AltkRegion *region )
{
  ASSERT(ALTK_DRAWABLE_GET_CLASS(drawable)->draw_bitmap_region);
  ALTK_DRAWABLE_GET_CLASS(drawable)->draw_bitmap_region(drawable, bitmap, region);
}
