/* altkgc.h -
 */

#ifndef _ALTKGC_H_
#define _ALTKGC_H_

#include "altk/altkbase.h"
#include "altk/altkregion.h"
#include "altk/altkdrawable.h"
#include "altk/altkfont.h"
#include "altk/altkgc-def.h"



struct _AltkFont;



/* AltkGC:
 */
struct _AltkGC
{
  ALTK_GC_INSTANCE_HEADER;
  gpointer private;
};



/* AltkGCClass:
 */
struct _AltkGCClass
{
  ALTK_GC_CLASS_HEADER;
};



AltkGC *altk_gc_new ( AltkDrawable *surface );

void altk_gc_set_surface ( AltkGC *gc,
                           AltkDrawable *surface );
void altk_gc_set_offset ( AltkGC *gc,
                          gint offset_x,
                          gint offset_y );
void altk_gc_set_color_hrgb ( AltkGC *gc,
                              guint32 color );
void altk_gc_set_color_hargb ( AltkGC *gc,
                               guint32 color );

void altk_gc_clear_region ( AltkGC *gc,
                            AltkRegion *region );
void altk_gc_draw_rectangle ( AltkGC *gc,
                              gboolean filled,
                              gint x,
                              gint y,
                              gint width,
                              gint height );
void altk_gc_draw_text ( AltkGC *gc,
                         gint x,
                         gint y,
                         const gchar *text );

/* [removeme] */
void altk_gc_set_fg_h32 ( AltkGC *gc,
                          guint32 color );
void altk_gc_set_bg_h32 ( AltkGC *gc,
                          guint32 color );
ALLEGRO_COLOR altk_gc_get_fg ( AltkGC *gc );
AltkFont *altk_gc_get_font ( AltkGC *gc );



#endif
