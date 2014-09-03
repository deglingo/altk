/* altkbase.c - basic stuff
 */

#include "altk/private.h"
#include "altk/altkbase.h"
#include "altk/altkmain.h"



void _altk_main_init ( void );
void _altk_wm_init ( void );



/* altk_init:
 */
int altk_init ( void )
{
  if (!al_init())
    CL_ERROR("could not initialize allegro");
  if (!al_init_primitives_addon())
    CL_ERROR("coud not initialize the primitives addon");
  if (!al_init_image_addon())
    CL_ERROR("could not initialize the image addon");
  if (!al_init_font_addon())
    CL_ERROR("could not initialize the font addon");
  if (!al_install_mouse())
    CL_ERROR("could not initialize the mouse driver");
  _altk_main_init();
  _altk_wm_init();
  return 0;
}
