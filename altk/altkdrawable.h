/* altkdrawable.h -
 */

#ifndef _ALTKDRAWABLE_H_
#define _ALTKDRAWABLE_H_

#include "altk/altkbase.h"
#include "altk/altkdrawable-def.h"



/* AltkDrawable:
 */
struct _AltkDrawable
{
  ALTK_DRAWABLE_INSTANCE_HEADER;
};



/* AltkDrawableClass:
 */
struct _AltkDrawableClass
{
  ALTK_DRAWABLE_CLASS_HEADER;

  gpointer (* get_target) ( AltkDrawable *drawable );

  void (* get_size) ( AltkDrawable *drawabale,
                      gint *width,
                      gint *height );
};



gpointer altk_drawable_get_target ( AltkDrawable *drawable );
void altk_drawable_get_size ( AltkDrawable *drawable,
                              gint *width,
                              gint *height );



#endif
