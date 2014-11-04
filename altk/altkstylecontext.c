/* altkstylecontext.c -
 */

#include "altk/private.h"
#include "altk/altkstylecontext.h"
#include "altk/altkgc.h"
#include "altk/altkstylecontext.inl"



/* altk_style_context_new:
 */
AltkStyleContext *altk_style_context_new ( AltkStyle *style )
{
  AltkStyleContext *ctxt;
  ctxt = ALTK_STYLE_CONTEXT(l_object_new(ALTK_CLASS_STYLE_CONTEXT, NULL));
  ctxt->style = l_object_ref(style); /* [FIXME] ref ? */
  ctxt->state = ALTK_STATE_NORMAL;
  ctxt->font = altk_font_new_builtin();
  return ctxt;
}



/* altk_style_context_set_state:
 */
void altk_style_context_set_state ( AltkStyleContext *context,
                                    AltkState state )
{
  context->state = state;
}



/* altk_style_context_get_font:
 */
AltkFont *altk_style_context_get_font ( AltkStyleContext *context )
{
  return context->font;
}



/* altk_style_context_draw_box:
 */
void altk_style_context_draw_box ( AltkStyleContext *context,
                                   AltkGC *gc,
                                   gint x,
                                   gint y,
                                   gint width,
                                   gint height )
{
  altk_gc_set_color_hrgb(gc, 0xc0c0c0); /* [fixme] */
  altk_gc_draw_rectangle(gc, TRUE, x, y, width, height);
}



/* altk_style_context_draw_frame:
 */
void altk_style_context_draw_frame ( AltkStyleContext *context,
                                     AltkGC *gc,
                                     gint x,
                                     gint y,
                                     gint width,
                                     gint height )
{
  altk_gc_set_color_hrgb(gc, 0x0);
  altk_gc_draw_rectangle(gc,
                         FALSE,
                         x,
                         y,
                         width,
                         height);
}



/* altk_style_context_draw_text:
 */
void altk_style_context_draw_text ( AltkStyleContext *context,
                                    AltkGC *gc,
                                    gint x,
                                    gint y,
                                    const gchar *text )
{
  altk_gc_set_color_hrgb(gc, 0x0);
  altk_gc_draw_text(gc,
                    x,
                    y,
                    text);
}
