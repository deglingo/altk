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
  gpointer private;
};



/* AltkBoxClass:
 */
struct _AltkBoxClass
{
  ALTK_BOX_CLASS_HEADER;
};



#define ALTK_BOX_ADD(box, child, flags) altk_box_pack_start(ALTK_BOX(box), (child), (flags))

AltkWidget *altk_box_new ( AltkOrientation orientation );
void altk_box_set_orientation ( AltkBox *box,
                                AltkOrientation orientation );
void altk_box_pack_start ( AltkBox *box,
                           AltkWidget *child,
                           AltkPackFlags flags );



#endif
