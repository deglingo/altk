/* altkfont.c -
 */

#include "altk/private.h"
#include "altk/altkfont.h"
#include "altk/altkfont.inl"



/* altk_font_class_init:
 */
static void altk_font_class_init ( LObjectClass *cls )
{
}



/* altk_font_new_builtin:
 */
AltkFont *altk_font_new_builtin ( void )
{
  AltkFont *font = ALTK_FONT(l_object_new(ALTK_CLASS_FONT, NULL));
  font->al_font = al_create_builtin_font();
  return font;
}



/* altk_font_get_text_size:
 */
void altk_font_get_text_size ( AltkFont *font,
                               const gchar *text,
                               gint *bx,
                               gint *by,
                               gint *bw,
                               gint *bh )
{
  al_get_text_dimensions(font->al_font, text, bx, by, bw, bh);
}
