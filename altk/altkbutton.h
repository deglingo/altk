/* altkbutton.h -
 */

#ifndef _ALTKBUTTON_H_
#define _ALTKBUTTON_H_

#include "altk/altkbin.h"
#include "altk/altkbutton-def.h"



/* AltkButton:
 */
struct _AltkButton
{
  ALTK_BUTTON_INSTANCE_HEADER;
};



/* AltkButtonClass:
 */
struct _AltkButtonClass
{
  ALTK_BUTTON_CLASS_HEADER;
};



AltkWidget *altk_button_new_with_label ( const gchar *label );



#endif
