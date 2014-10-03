/* altkbuilder.c -
 */

#include "altk/altkbuilder.h"
#include "altk/altkbuilder.inl"



/* altk_builder_new:
 */
AltkBuilder *altk_builder_new ( void )
{
  AltkBuilder *b = ALTK_BUILDER(l_object_new(ALTK_CLASS_BUILDER, NULL));
  return b;
}
