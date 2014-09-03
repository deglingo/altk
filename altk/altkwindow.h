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
};



/* AltkWindowClass:
 */
struct _AltkWindowClass
{
  ALTK_WINDOW_CLASS_HEADER;
};



AltkWindow *altk_window_new_root ( struct _AltkDisplay *display );



#endif
