/* altktable.h -
 */

#ifndef _ALTKTABLE_H_
#define _ALTKTABLE_H_

#include "altk/altkcontainer.h"
#include "altk/altktable-def.h"



/* AltkTable:
 */
struct _AltkTable
{
  ALTK_TABLE_INSTANCE_HEADER;

  GList *children;
};



/* AltkTableClass:
 */
struct _AltkTableClass
{
  ALTK_TABLE_CLASS_HEADER;
};



AltkWidget *altk_table_new ( void );



#endif
