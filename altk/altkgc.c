/* altkgc.c -
 */

#include "altk/private.h"
#include "altk/altkgc.h"
#include "altk/altkfont.h"
#include "altk/altkgc.inl"



/* altk_gc_new:
 */
AltkGC *altk_gc_new ( void )
{
  AltkGC *gc = ALTK_GC(l_object_new(ALTK_CLASS_GC, NULL));
  gc->font = altk_font_new_builtin();
  return gc;
}



/* _map_h32:
 */
static ALLEGRO_COLOR _map_h32 ( guint32 c )
{
  return al_map_rgba((c >> 16) & 0xff,
                     (c >> 8)  & 0xff,
                     (c)       & 0xff,
                     (c >> 24) & 0xff);
}



/* altk_gc_set_fg_h32:
 */
void altk_gc_set_fg_h32 ( AltkGC *gc,
                          guint32 color )
{
  gc->fg = _map_h32(color);
}



/* altk_gc_set_bg_h32:
 */
void altk_gc_set_bg_h32 ( AltkGC *gc,
                          guint32 color )
{
  gc->bg = _map_h32(color);
}
