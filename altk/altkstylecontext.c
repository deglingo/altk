/* altkstylecontext.c -
 */

#include "altk/private.h"
#include "altk/altkstylecontext.h"
#include "altk/altkstylecontext.inl"



/* altk_style_context_new:
 */
AltkStyleContext *altk_style_context_new ( AltkStyle *style )
{
  AltkStyleContext *ctxt;
  ctxt = ALTK_STYLE_CONTEXT(l_object_new(ALTK_CLASS_STYLE_CONTEXT, NULL));
  ctxt->style = l_object_ref(style); /* [FIXME] ref ? */
  ctxt->state = ALTK_STATE_NORMAL;
  return ctxt;
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
                               NULL /* [fixme] gc */,
                               TRUE,
                               x,
                               y,
                               width,
                               height);

}
