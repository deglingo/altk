/* altkgc.h -
 */

#ifndef _ALTKGC_H_
#define _ALTKGC_H_

#include "altk/altkbase.h"
#include "altk/altkgc-def.h"



struct _AltkFont;



/* AltkGC:
 */
struct _AltkGC
{
  ALTK_GC_INSTANCE_HEADER;
  ALLEGRO_COLOR fg;
  ALLEGRO_COLOR bg;
  struct _AltkFont *font;
};



/* AltkGCClass:
 */
struct _AltkGCClass
{
  ALTK_GC_CLASS_HEADER;
};



AltkGC *altk_gc_new ( void );
void altk_gc_set_fg_h32 ( AltkGC *gc,
                          guint32 color );
void altk_gc_set_bg_h32 ( AltkGC *gc,
                          guint32 color );



#endif
