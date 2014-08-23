/* altkdialog.c -
 */

#include "altk/altkdialog.h"
#include "altk/altkdisplay.h"



/* altk_dialog_new:
 */
AltkWidget *altk_dialog_new ( AltkDisplay *display )
{
  AltkWidget *dlg;
  dlg = ALTK_WIDGET(l_object_new(ALTK_CLASS_DIALOG, NULL));
  altk_display_attach_widget(display, dlg);
  return dlg;
}
