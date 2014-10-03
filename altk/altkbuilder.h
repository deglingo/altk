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
gboolean altk_builder_parse_text ( AltkBuilder *builder,
                                   const gchar *text,
                                   GError **error );
LObject *altk_builder_get_object ( AltkBuilder *builder,
                                   const gchar *name );



#endif
