/* altkgc.c -
 */

#include "altk/private.h"
#include "altk/altkgc.h"
#include "altk/altkfont.h"
#include "altk/altkgc.inl"



/* Private:
 */
typedef struct _Private
{
  ALLEGRO_COLOR fg;
  ALLEGRO_COLOR bg;
  struct _AltkFont *font;
}
  Private;

#define PRIVATE(gc) ((Private *)(ALTK_GC(gc)->private))



/* altk_gc_init:
 */
static void altk_gc_init ( LObject *obj )
{
  ALTK_GC(obj)->private = g_new0(Private, 1);
}



/* altk_gc_new:
 */
AltkGC *altk_gc_new ( void )
{
  AltkGC *gc = ALTK_GC_NEW(NULL);
  Private *priv = gc->private;
  priv->font = altk_font_new_builtin();
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
  PRIVATE(gc)->fg = _map_h32(color);
}



/* altk_gc_set_bg_h32:
 */
void altk_gc_set_bg_h32 ( AltkGC *gc,
                          guint32 color )
{
  PRIVATE(gc)->bg = _map_h32(color);
}



/* altk_gc_get_fg:
 */
ALLEGRO_COLOR altk_gc_get_fg ( AltkGC *gc )
{
  return PRIVATE(gc)->fg;
}



/* altk_gc_get_font:
 */
AltkFont *altk_gc_get_font ( AltkGC *gc )
{
  return PRIVATE(gc)->font;
}
