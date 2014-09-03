/* altkwm.h - window management
 */

#ifndef _ALTKWM_H_
#define _ALTKWM_H_

#include "altk/altkbase.h"



struct _AltkDisplay;

void altk_wm_register_display ( struct _AltkDisplay *display );
void altk_wm_register_al_source ( ALLEGRO_EVENT_SOURCE *al_source );



#endif
