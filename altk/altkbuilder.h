/* altkbuilder.h -
 */

#ifndef _ALTKBUILDER_H_
#define _ALTKBUILDER_H_

#include "altk/altkbase.h"
#include "altk/altkbuilder-def.h"



/* AltkBuilder:
 */
struct _AltkBuilder
{
  ALTK_BUILDER_INSTANCE_HEADER;
};



/* AltkBuilderClass:
 */
struct _AltkBuilderClass
{
  ALTK_BUILDER_CLASS_HEADER;
};



AltkBuilder *altk_builder_new ( void );



#endif
