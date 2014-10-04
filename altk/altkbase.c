/* altkbase.c - basic stuff
 */

#include "altk/private.h"
#include "altk/altkbase.h"
#include "altk/altkmain.h"
#include "altk/altkevent.h"
#include "altk/altkwidget.h"
#include "altk/altkwm.h"



void _altk_main_init ( void );
void _altk_wm_init ( void );



/* altk_init_once:
 */
static gpointer altk_init_once ( gpointer dummy )
{
  los_init();
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
  altk_wm_register_al_source(al_get_mouse_event_source());
  altk_event_set_handler(altk_widget_event_handler, NULL);
  return NULL;
}



/* altk_init:
 */
int altk_init ( void )
{
  GOnce once = G_ONCE_INIT;
  g_once(&once, altk_init_once, NULL);
  return 0;
}



/* altk_size_distrib_init:
 */
void altk_size_distrib_init ( AltkSizeDistrib *distrib,
                              guint size,
                              guint n_items )
{
  distrib->size = size;
  distrib->n_items = n_items;
  distrib->current = 0;
  distrib->remainder = 0;
}



/* altk_size_distrib_next:
 */
guint altk_size_distrib_next ( AltkSizeDistrib *distrib )
{
  guint s;
  guint r;
  if (distrib->current >= distrib->n_items)
    return 0;
  s = distrib->size + distrib->remainder;
  r = s / distrib->n_items;
  distrib->remainder = s - (distrib->n_items * r);
  distrib->current++;
  return r;
}
