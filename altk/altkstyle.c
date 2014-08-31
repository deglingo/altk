/* altkstyle.c -
 */

#include "altk/private.h"
#include "altk/altkstyle.h"
#include "altk/altkdrawable.h"
#include "altk/altkgc.h"

#include "altk/altkstyle.inl"



/* altk_style_class_init:
 */
static void altk_style_class_init ( LObjectClass *cls )
{
}



/* altk_style_new:
 */
AltkStyle *altk_style_new ( void )
{
  /* [FIXME] */
  static AltkStyle *style = NULL;
  if (!style)
    style = ALTK_STYLE(l_object_new(ALTK_CLASS_STYLE, NULL));
  return l_object_ref(style);
}



/* altk_style_draw_text:
 */
void altk_style_draw_text ( AltkStyle *style,
                            struct _AltkDrawable *dest,
                            struct _AltkGC *gc,
                            gint x,
                            gint y,
                            const gchar *text )
{
  altk_drawable_draw_text(dest, gc, x, y, text);
}
