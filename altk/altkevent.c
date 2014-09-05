/* altkevent.c -
 */

#include "altk/private.h"
#include "altk/altkevent.h"
#include "altk/altkwidget.h"



/* globals */
static AltkEventHandler event_handler = NULL;
static gpointer event_handler_data = NULL;



/* altk_event_set_handler:
 */
void altk_event_set_handler ( AltkEventHandler handler,
                              gpointer data )
{
  event_handler = handler;
  event_handler_data = data;
}



/* altk_event_process:
 */
void altk_event_process ( AltkEvent *event )
{
  ASSERT(event_handler);
  event_handler(event, event_handler_data);
}
