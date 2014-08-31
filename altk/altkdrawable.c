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
