/* altkstylecontext.h -
 */

#ifndef _ALTKSTYLECONTEXT_H_
#define _ALTKSTYLECONTEXT_H_

#include "altk/altkbase.h"
#include "altk/altkstyle.h"
#include "altk/altkdrawable.h"
#include "altk/altkgc.h"
#include "altk/altkfont.h"
#include "altk/altkstylecontext-def.h"



/* AltkStyleContext:
 */
struct _AltkStyleContext
{
  ALTK_STYLE_CONTEXT_INSTANCE_HEADER;

  AltkStyle *style;
  AltkState state;
  AltkFont *font; /* [fixme] */
};



/* AltkStyleContextClass:
 */
struct _AltkStyleContextClass
{
  ALTK_STYLE_CONTEXT_CLASS_HEADER;
};



AltkStyleContext *altk_style_context_new ( AltkStyle *style );
void altk_style_context_set_state ( AltkStyleContext *context,
                                    AltkState state );
void altk_style_context_draw_box ( AltkStyleContext *context,
                                   AltkGC *gc,
                                   gint x,
                                   gint y,
                                   gint width,
                                   gint height );
void altk_style_context_draw_frame ( AltkStyleContext *context,
                                     AltkGC *gc,
                                     gint x,
                                     gint y,
                                     gint width,
                                     gint height );
void altk_style_context_draw_text ( AltkStyleContext *context,
                                    AltkGC *gc,
                                    gint x,
                                    gint y,
                                    const gchar *text );
AltkFont *altk_style_context_get_font ( AltkStyleContext *context );



#endif
