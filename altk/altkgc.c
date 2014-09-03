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
