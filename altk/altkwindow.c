/* altkwindow.c -
 */

#include "altk/private.h"
#include "altk/altkwindow.h"
#include "altk/altkwindow.inl"



/* altk_window_class_init:
 */
static void altk_window_class_init ( LObjectClass *cls )
{
}



/* altk_window_new_root:
 */
AltkWindow *altk_window_new_root ( struct _AltkDisplay *display )
{
  AltkWindow *win;
  win = ALTK_WINDOW(l_object_new(ALTK_CLASS_WINDOW, NULL));
  return win;
}
