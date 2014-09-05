/* altkwindow.c -
 */

#include "altk/private.h"
#include "altk/altkwindow.h"
#include "altk/altkwindow.inl"



/* altk_window_new_root:
 */
AltkWindow *altk_window_new_root ( struct _AltkDisplay *display )
{
  AltkWindow *win;
  win = ALTK_WINDOW(l_object_new(ALTK_CLASS_WINDOW, NULL));
  return win;
}



/* altk_window_new:
 */
AltkWindow *altk_window_new ( AltkWindow *parent,
                              gint x,
                              gint y,
                              gint width,
                              gint height )
{
  AltkWindow *win;
  win = ALTK_WINDOW(l_object_new(ALTK_CLASS_WINDOW, NULL));
  return win;
}



/* altk_window_set_bounds:
 */
void altk_window_set_bounds ( AltkWindow *window,
                              gint x,
                              gint y,
                              gint width,
                              gint height )
{
}
