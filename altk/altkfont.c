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
