/* altkframe.h -
 */

#ifndef _ALTKFRAME_H_
#define _ALTKFRAME_H_

#include "altk/altkbin.h"
#include "altk/altkframe-def.h"


/* AltkFrame:
 */
struct _AltkFrame
{
  ALTK_FRAME_INSTANCE_HEADER;
};



/* AltkFrameClass:
 */
struct _AltkFrameClass
{
  ALTK_FRAME_CLASS_HEADER;
};



AltkWidget *altk_frame_new ( const gchar *title );



#endif
