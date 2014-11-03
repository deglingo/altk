/* altkstylecontext.c -
 */

#include "altk/private.h"
#include "altk/altkstylecontext.h"
#include "altk/altkgc.h"
#include "altk/altkstylecontext.inl"



struct gc_info {
    guint32 fg;
    guint32 bg;
};



/* _create_gc:
 */
static AltkGC *_create_gc ( struct gc_info *info )
{
  AltkGC *gc = altk_gc_new();
  altk_gc_set_fg_h32(gc, info->fg);
  altk_gc_set_bg_h32(gc, info->bg);
  return gc;
}



/* altk_style_context_new:
 */
AltkStyleContext *altk_style_context_new ( AltkStyle *style )
{
  struct  gc_info gc_info[] = {
    { 0xff7f7f7f, 0x000000 }, /* normal */
    { 0xffc0c0c0, 0x000000 }, /* active */
  };
  AltkStyleContext *ctxt;
  ctxt = ALTK_STYLE_CONTEXT(l_object_new(ALTK_CLASS_STYLE_CONTEXT, NULL));
  ctxt->style = l_object_ref(style); /* [FIXME] ref ? */
  ctxt->state = ALTK_STATE_NORMAL;
  ctxt->gc_normal = _create_gc(&gc_info[0]);
  ctxt->gc_active = _create_gc(&gc_info[1]);
  return ctxt;
}



/* _select_gc:
 */
static AltkGC *_select_gc ( AltkStyleContext *context )
{
  if (context->state == ALTK_STATE_NORMAL)
    return context->gc_normal;
  else
    return context->gc_active;
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
  return altk_gc_get_font(_select_gc(context));
}



/* altk_style_context_draw_box:
 */
void altk_style_context_draw_box ( AltkStyleContext *context,
                                   AltkDrawable *drawable,
                                   gint x,
                                   gint y,
                                   gint width,
                                   gint height )
{
  altk_drawable_draw_rectangle(drawable,
                               _select_gc(context),
                               TRUE,
                               x,
                               y,
                               width,
                               height);

}



/* altk_style_context_draw_frame:
 */
void altk_style_context_draw_frame ( AltkStyleContext *context,
                                     AltkDrawable *drawable,
                                     gint x,
                                     gint y,
                                     gint width,
                                     gint height )
{
  altk_drawable_draw_rectangle(drawable,
                               _select_gc(context),
                               FALSE,
                               x,
                               y,
                               width,
                               height);
}



/* altk_style_context_draw_text:
 */
void altk_style_context_draw_text ( AltkStyleContext *context,
                                    AltkDrawable *drawable,
                                    gint x,
                                    gint y,
                                    const gchar *text )
{
  altk_drawable_draw_text(drawable,
                          _select_gc(context),
                          x,
                          y,
                          text);
}
