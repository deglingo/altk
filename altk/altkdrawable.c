/* altkdrawable.c -
 */

#include "altk/private.h"
#include "altk/altkdrawable.h"
#include "altk/altkdrawable.inl"



/* altk_drawable_set_offset:
 */
void altk_drawable_set_offset ( AltkDrawable *drawable,
                                gint ofs_x,
                                gint ofs_y )
{
  drawable->offset_x = ofs_x;
  drawable->offset_y = ofs_y;
}



/* altk_drawable_get_target:
 */
gpointer altk_drawable_get_target ( AltkDrawable *drawable )
{
  ASSERT(ALTK_DRAWABLE_GET_CLASS(drawable)->get_target);
  return ALTK_DRAWABLE_GET_CLASS(drawable)->get_target(drawable);
}



/* altk_drawable_get_size:
 */
void altk_drawable_get_size ( AltkDrawable *drawable,
                              gint *width,
                              gint *height )
{
  ASSERT(ALTK_DRAWABLE_GET_CLASS(drawable)->get_size);
  return ALTK_DRAWABLE_GET_CLASS(drawable)->get_size(drawable, width, height);
}



/* altk_drawable_draw_rectangle:
 */
void altk_drawable_draw_rectangle ( AltkDrawable *drawable,
                                    struct _AltkGC *gc,
                                    gboolean filled,
                                    gint x,
                                    gint y,
                                    gint width,
                                    gint height )
{
  ASSERT(ALTK_DRAWABLE_GET_CLASS(drawable)->draw_rectangle);
  ALTK_DRAWABLE_GET_CLASS(drawable)->draw_rectangle(drawable,
                                                    gc,
                                                    filled,
                                                    x,
                                                    y,
                                                    width,
                                                    height);
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
                                        struct _AltkRegion *region,
                                        gint dest_x,
                                        gint dest_y )
{
  ASSERT(ALTK_DRAWABLE_GET_CLASS(drawable)->draw_bitmap_region);
  ALTK_DRAWABLE_GET_CLASS(drawable)->draw_bitmap_region
    (drawable, bitmap, region, dest_x, dest_y);
}
