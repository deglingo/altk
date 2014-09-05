/* altkwindow.h -
 */

#ifndef _ALTKWINDOW_H_
#define _ALTKWINDOW_H_

#include "altk/altkdrawable.h"
#include "altk/altkwindow-def.h"



struct _AltkDisplay;



/* AltkWindow:
 */
struct _AltkWindow
{
  ALTK_WINDOW_INSTANCE_HEADER;
  /* some data for the user, totally ignored by the window itself */
  gpointer user_data;
};



/* AltkWindowClass:
 */
struct _AltkWindowClass
{
  ALTK_WINDOW_CLASS_HEADER;
};



AltkWindow *altk_window_new_root ( struct _AltkDisplay *display );
AltkWindow *altk_window_new ( AltkWindow *parent,
                              gint x,
                              gint y,
                              gint width,
                              gint height );
void altk_window_set_bounds ( AltkWindow *window,
                              gint x,
                              gint y,
                              gint width,
                              gint height );



#endif
