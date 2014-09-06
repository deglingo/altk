/* altkstylecontext.h -
 */

#ifndef _ALTKSTYLECONTEXT_H_
#define _ALTKSTYLECONTEXT_H_

#include "altk/altkbase.h"
#include "altk/altkstyle.h"
#include "altk/altkdrawable.h"
#include "altk/altkstylecontext-def.h"



/* AltkStyleContext:
 */
struct _AltkStyleContext
{
  ALTK_STYLE_CONTEXT_INSTANCE_HEADER;

  AltkStyle *style;
  AltkState state;
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
                                   AltkDrawable *drawable,
                                   gint x,
                                   gint y,
                                   gint width,
                                   gint height );



#endif
