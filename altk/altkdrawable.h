/* altkdrawable.h -
 */

#ifndef _ALTKDRAWABLE_H_
#define _ALTKDRAWABLE_H_

#include "altk/altkbase.h"
#include "altk/altkdrawable-def.h"



struct _AltkGC;



/* AltkDrawable:
 */
struct _AltkDrawable
{
  ALTK_DRAWABLE_INSTANCE_HEADER;
  /* [FIXME] should be handled by the implementations instead ? */
  gint offset_x;
  gint offset_y;
};



/* AltkDrawableClass:
 */
struct _AltkDrawableClass
{
  ALTK_DRAWABLE_CLASS_HEADER;

  /* drawing methods */
  void (* draw_text) ( AltkDrawable *drawable,
                       struct _AltkGC *gc,
                       gint x,
                       gint y,
                       const gchar *text );
};



void altk_drawable_set_offset ( AltkDrawable *drawable,
                                gint ofs_x,
                                gint ofs_y );

void altk_drawable_draw_text ( AltkDrawable *drawable,
                               struct _AltkGC *gc,
                               gint x,
                               gint y,
                               const gchar *text );



#endif
