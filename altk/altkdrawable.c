/* altkdrawable.c -
 */

#include "altk/private.h"
#include "altk/altkdrawable.h"
#include "altk/altkdrawable.inl"



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
