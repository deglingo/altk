/* altkstyle.h -
 */

#ifndef _ALTKSTYLE_H_
#define _ALTKSTYLE_H_

#include "altk/altkbase.h"
#include "altk/altkstyle-def.h"



/* AltkStyle:
 */
struct _AltkStyle
{
  ALTK_STYLE_INSTANCE_HEADER;
};



/* AltkStyleClass:
 */
struct _AltkStyleClass
{
  ALTK_STYLE_CLASS_HEADER;
};



AltkStyle *altk_style_new ( void );



#endif
