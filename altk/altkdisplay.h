/* altkdisplay.h -
 */

#ifndef _ALTKDISPLAY_H_
#define _ALTKDISPLAY_H_



struct _AltkWidget;

typedef struct _AltkDisplay AltkDisplay;



AltkDisplay *altk_display_new ( void );
void altk_display_attach_widget ( AltkDisplay *display,
                                  struct _AltkWidget *widget );



#endif
