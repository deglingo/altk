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
  AltkDrawable *surface;
  ALLEGRO_BITMAP *target;
  ALLEGRO_COLOR color;
  gint offset_x;
  gint offset_y;
  /* [removeme] */
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
AltkGC *altk_gc_new ( AltkDrawable *surface )
{
  AltkGC *gc = ALTK_GC_NEW(NULL);
  Private *priv = gc->private;
  if (surface)
    altk_gc_set_surface(gc, surface);
  priv->font = altk_font_new_builtin();
  return gc;
}



/* altk_gc_set_surface:
 */
void altk_gc_set_surface ( AltkGC *gc,
                           AltkDrawable *surface )
{
  if (surface != PRIVATE(gc)->surface)
    {
      if (PRIVATE(gc)->surface)
        L_OBJECT_CLEAR(PRIVATE(gc)->surface);
      if (surface)
        {
          PRIVATE(gc)->surface = l_object_ref(surface);
          PRIVATE(gc)->target = altk_drawable_get_target(surface);
        }
    }
}



/* altk_gc_set_offset:
 */
void altk_gc_set_offset ( AltkGC *gc,
                          gint offset_x,
                          gint offset_y )
{
  CL_ERROR("[FIXME]");
  PRIVATE(gc)->offset_x = offset_x;
  PRIVATE(gc)->offset_y = offset_y;
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



/* altk_gc_set_color_hrgb:
 */
void altk_gc_set_color_hrgb ( AltkGC *gc,
                              guint32 color )
{
  altk_gc_set_color_hargb(gc, 0xff000000 | color);
}



/* altk_gc_set_color_hargb:
 */
void altk_gc_set_color_hargb ( AltkGC *gc,
                               guint32 color )
{
  PRIVATE(gc)->color = al_map_rgba((color >> 16) & 0xff,
                                   (color >> 8)  & 0xff,
                                   (color)       & 0xff,
                                   (color >> 24) & 0xff);
}



/* altk_gc_clear_region:
 */
void altk_gc_clear_region ( AltkGC *gc,
                            AltkRegion *region )
{
  Private *priv = PRIVATE(gc);
  ALLEGRO_STATE state;
  gint cx, cy, cw, ch;
  gint r;
  AltkRegionBox *box;
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(priv->target);
  al_get_clipping_rectangle(&cx, &cy, &cw, &ch);
  for (r = 0, box = region->rects; r < region->n_rects; r++, box++)
    {
      al_set_clipping_rectangle(box->x1,
                                box->y1,
                                box->x2 - box->x1,
                                box->y2 - box->y1);
      al_clear_to_color(priv->color);
    }
  al_set_clipping_rectangle(cx, cy, cw, ch);
  al_restore_state(&state);
}



/* altk_gc_draw_rectangle:
 */
void altk_gc_draw_rectangle ( AltkGC *gc,
                              gboolean filled,
                              gint x,
                              gint y,
                              gint width,
                              gint height )
{
  Private *priv = PRIVATE(gc);
  ALLEGRO_STATE state;
  /* [fixme] pixel exact coordinates */
  float x1 = (float) (x + priv->offset_x);
  float y1 = (float) (y + priv->offset_y);
  float x2 = (float) (x + width + priv->offset_x);
  float y2 = (float) (y + height + priv->offset_y);
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(priv->target);
  if (filled)
    al_draw_filled_rectangle(x1, y1, x2, y2, priv->color);
  else
    al_draw_rectangle(x1, y1, x2, y2, priv->color, 1.0);
  al_restore_state(&state);
}



/* altk_gc_draw_text:
 */
void altk_gc_draw_text ( AltkGC *gc,
                         gint x,
                         gint y,
                         const gchar *text )
{
  Private *priv = PRIVATE(gc);
  ALLEGRO_STATE state;
  al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
  al_set_target_bitmap(priv->target);
  al_draw_text(priv->font->al_font,
               priv->color,
               x,
               y,
               ALLEGRO_ALIGN_LEFT,
               text);
  al_restore_state(&state);
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
