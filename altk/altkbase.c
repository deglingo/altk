/* altkbase.c - basic stuff
 */

#include "altk/private.h"
#include "altk/altkbase.h"



void _altk_main_init ( void );

GQuark ALTK_QUARK_AL_OWNER = 0;



/* altk_init:
 */
int altk_init ( void )
{
  if (!al_init())
    CL_ERROR("could not initialize allegro");
  if (!al_init_primitives_addon())
    CL_ERROR("coud not initialize the primitives addon");
  ALTK_QUARK_AL_OWNER = g_quark_from_string("altk-quark-al-owner");
  _altk_main_init();
  return 0;
}
