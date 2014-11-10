/* altkwm.h - window management
 */

#ifndef _ALTKWM_H_
#define _ALTKWM_H_

#include "altk/altkbase.h"
#include "altk/altkwidget.h"



struct _AltkDisplay;

void altk_wm_register_display ( struct _AltkDisplay *display );
void altk_wm_register_al_source ( ALLEGRO_EVENT_SOURCE *al_source );

void altk_wm_register_top_widget ( AltkWidget *widget );
void altk_wm_set_top_widget_size_hints ( AltkWidget *widget,
                                         AltkSizeHints hints );
AltkSizeHints altk_wm_get_top_widget_size_hints ( AltkWidget *widget );



#endif
