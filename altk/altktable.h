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

  gpointer private;
};



/* AltkTableClass:
 */
struct _AltkTableClass
{
  ALTK_TABLE_CLASS_HEADER;
};



AltkWidget *altk_table_new ( void );
void altk_table_attach ( AltkTable *table,
                         AltkWidget *child,
                         gint top,
                         gint left,
                         gint bottom,
                         gint right,
                         AltkPackFlags flags );



#endif
