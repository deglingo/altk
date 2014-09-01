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
  struct _AltkFont *font;
};



/* AltkGCClass:
 */
struct _AltkGCClass
{
  ALTK_GC_CLASS_HEADER;
};



AltkGC *altk_gc_new ( void );



#endif
