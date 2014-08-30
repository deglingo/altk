/* altkbitmap.h -
 */

#ifndef _ALTKBITMAP_H_
#define _ALTKBITMAP_H_

#include "altk/altkdrawable.h"
#include "altk/altkbitmap-def.h"



struct _AltkDisplay;



/* AltkBitmap:
 */
struct _AltkBitmap
{
  ALTK_BITMAP_INSTANCE_HEADER;
  /* underlying allegro bitmap */
  ALLEGRO_BITMAP *al_bitmap;
};



/* AltkBitmapClass:
 */
struct _AltkBitmapClass
{
  ALTK_BITMAP_CLASS_HEADER;
};



AltkDrawable *altk_bitmap_new ( struct _AltkDisplay *display,
                                gint width,
                                gint height );



#endif
