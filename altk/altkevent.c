/* altkevent.c -
 */

#include "altk/private.h"
#include "altk/altkevent.h"
#include "altk/altkwidget.h"



/* altk_event_process:
 */
void altk_event_process ( AltkEvent *event )
{
  altk_widget_event(event->any.widget, event);
}
