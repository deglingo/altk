/* altkfont.c -
 */

#include "altk/private.h"
#include "altk/altkfont.h"
#include "altk/altkfont.inl"



static void _get_char_width ( AltkFont *font )
{
  gint c;
  gchar text[2] = { 0, 0 };
  gint bx, by, bw, bh;
  font->char_width = 0;
  /* [FIXME] */
  for (c = 32; c < 128; c++)
    {
      text[0] = c;
      altk_font_get_text_size(font, text, &bx, &by, &bw, &bh);
      font->char_width = MAX(font->char_width, bw);
    }
}



/* altk_font_new_builtin:
 */
AltkFont *altk_font_new_builtin ( void )
{
  static AltkFont *font = NULL;
  if (!font)
    {
      font = ALTK_FONT_NEW(NULL);
      font->al_font = al_create_builtin_font();
      _get_char_width(font);
    }
  return l_object_ref(font);
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
