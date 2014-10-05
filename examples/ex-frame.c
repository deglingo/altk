/* ex_frame.c -
 */

#include <clog.h>
#include "altk/altk.h"



/* main:
 */
gint main ()
{
  AltkDisplay *display;
  AltkWidget *dlg, *frame, *lbl;
  CL_DEBUG("HELLO!");
  altk_init();
  display = altk_display_new();
  dlg = altk_dialog_new(display);
  frame = altk_frame_new("Frame title");
  ALTK_CONTAINER_ADD(dlg, frame);
  lbl = altk_label_new("A label in a frame!");
  ALTK_CONTAINER_ADD(frame, lbl);
  altk_widget_show_all(dlg);
  altk_display_open(display);
  altk_main();
  return 0;
}
