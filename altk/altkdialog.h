/* altkdialog.h -
 */

#ifndef _ALTKDIALOG_H_
#define _ALTKDIALOG_H_

#include "altk/altkbin.h"
#include "altk/altkdialog-def.h"


struct _AltkDisplay;



/* AltkDialog:
 */
struct _AltkDialog
{
  ALTK_DIALOG_INSTANCE_HEADER;
};



/* AltkDialogClass:
 */
struct _AltkDialogClass
{
  ALTK_DIALOG_CLASS_HEADER;
};



AltkWidget *altk_dialog_new ( struct _AltkDisplay *display );



#endif
