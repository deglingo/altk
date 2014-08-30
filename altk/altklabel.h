/* altklabel.h -
 */

#ifndef _ALTKLABEL_H_
#define _ALTKLABEL_H_

#include "altk/altkwidget.h"
#include "altk/altklabel-def.h"



/* AltkLabel:
 */
struct _AltkLabel
{
  ALTK_LABEL_INSTANCE_HEADER;

  gchar *text;
};



/* AltkLabelClass:
 */
struct _AltkLabelClass
{
  ALTK_LABEL_CLASS_HEADER;
};



AltkWidget *altk_label_new ( const gchar *text );



#endif