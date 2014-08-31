/* altkstyle.h -
 */

#ifndef _ALTKSTYLE_H_
#define _ALTKSTYLE_H_

#include "altk/altkbase.h"
#include "altk/altkstyle-def.h"



struct _AltkDrawable;
struct _AltkGC;



/* AltkStyle:
 */
struct _AltkStyle
{
  ALTK_STYLE_INSTANCE_HEADER;
};



/* AltkStyleClass:
 */
struct _AltkStyleClass
{
  ALTK_STYLE_CLASS_HEADER;
};



AltkStyle *altk_style_new ( void );

void altk_style_draw_text ( AltkStyle *style,
                            struct _AltkDrawable *dest,
                            struct _AltkGC *gc,
                            gint x,
                            gint y,
                            const gchar *text );



#endif
