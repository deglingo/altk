/* altkbin.h -
 */

#ifndef _ALTKBIN_H_
#define _ALTKBIN_H_

#include "altk/altkcontainer.h"
#include "altk/altkbin-def.h"



/* AltkBin:
 */
struct _AltkBin
{
  ALTK_BIN_INSTANCE_HEADER;

  AltkWidget *child;
};



/* AltkBinClass:
 */
struct _AltkBinClass
{
  ALTK_BIN_CLASS_HEADER;
};



#define ALTK_BIN_CHILD_VISIBLE(bin) (ALTK_BIN(bin)->child && ALTK_WIDGET_VISIBLE(ALTK_BIN(bin)->child))



#endif
