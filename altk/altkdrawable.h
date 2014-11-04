/* altkdrawable.h -
 */

#ifndef _ALTKDRAWABLE_H_
#define _ALTKDRAWABLE_H_

#include "altk/altkbase.h"
#include "altk/altkdrawable-def.h"



struct _AltkRegion;
struct _AltkGC;
struct _AltkBitmap;



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

  gpointer (* get_target) ( AltkDrawable *drawable );

  void (* get_size) ( AltkDrawable *drawabale,
                      gint *width,
                      gint *height );

  /* drawing methods */
  void (* draw_rectangle) ( AltkDrawable *drawable,
                            struct _AltkGC *gc,
                            gboolean filled,
                            gint x,
                            gint y,
                            gint width,
                            gint height );

  void (* draw_text) ( AltkDrawable *drawable,
                       struct _AltkGC *gc,
                       gint x,
                       gint y,
                       const gchar *text );

  void (* draw_bitmap_region) ( AltkDrawable *drawable,
                                struct _AltkBitmap *bitmap,
                                struct _AltkRegion *region,
                                gint dest_x,
                                gint dest_y);
};



gpointer altk_drawable_get_target ( AltkDrawable *drawable );
void altk_drawable_get_size ( AltkDrawable *drawable,
                              gint *width,
                              gint *height );

void altk_drawable_set_offset ( AltkDrawable *drawable,
                                gint ofs_x,
                                gint ofs_y );

void altk_drawable_draw_rectangle ( AltkDrawable *drawable,
                                    struct _AltkGC *gc,
                                    gboolean filled,
                                    gint x,
                                    gint y,
                                    gint width,
                                    gint height );
void altk_drawable_draw_text ( AltkDrawable *drawable,
                               struct _AltkGC *gc,
                               gint x,
                               gint y,
                               const gchar *text );
void altk_drawable_draw_bitmap_region ( AltkDrawable *drawable,
                                        struct _AltkBitmap *bitmap,
                                        struct _AltkRegion *region,
                                        gint dest_x,
                                        gint dest_y );



#endif
