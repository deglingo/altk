/* altkbox.h -
 */

#ifndef _ALTKBOX_H_
#define _ALTKBOX_H_

#include "altk/altkcontainer.h"
#include "altk/altkbox-def.h"



/* AltkBox:
 */
struct _AltkBox
{
  ALTK_BOX_INSTANCE_HEADER;

  AltkOrientation orientation;
  GList *children;
};



/* AltkBoxClass:
 */
struct _AltkBoxClass
{
  ALTK_BOX_CLASS_HEADER;
};



AltkWidget *altk_box_new ( AltkOrientation orientation );
void altk_box_pack_start ( AltkBox *box,
                           AltkWidget *child,
                           AltkPackFlags flags );



#endif
