/* altkfont.h -
 */

#ifndef _ALTKFONT_H_
#define _ALTKFONT_H_

#include "altk/altkbase.h"
#include "altk/altkfont-def.h"



/* AltkFont:
 */
struct _AltkFont
{
  ALTK_FONT_INSTANCE_HEADER;
  /* the underlying allegro font */
  ALLEGRO_FONT *al_font;
  gint char_width;
};



/* AltkFontClass:
 */
struct _AltkFontClass
{
  ALTK_FONT_CLASS_HEADER;
};



AltkFont *altk_font_new_builtin ( void );
void altk_font_get_text_size ( AltkFont *font,
                               const gchar *text,
                               gint *bx,
                               gint *by,
                               gint *bw,
                               gint *bh );



#endif
