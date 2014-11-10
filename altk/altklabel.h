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
  gint min_chars;
  AltkPackFlags alignment;
};



/* AltkLabelClass:
 */
struct _AltkLabelClass
{
  ALTK_LABEL_CLASS_HEADER;
};



AltkWidget *altk_label_new ( const gchar *text );
void altk_label_set_text ( AltkLabel *label,
                           const gchar *text );
void altk_label_set_min_chars ( AltkLabel *label,
                                gint min_chars );
void altk_label_set_alignment ( AltkLabel *label,
                                AltkPackFlags alignment );



#endif
