/* altkmain.c -
 */

#include "altk/private.h"
#include "altk/altkmain.h"



static gboolean _idle_timer ( gpointer data );



/* _altk_main_init:
 *
 * Called by altk_init(), don't use this directly.
 */
void _altk_main_init ( void )
{
  /* idle timer */
  g_idle_add_full(G_MAXINT32, _idle_timer, NULL, NULL);
}



/* _idle_timer:
 */
static gboolean _idle_timer ( gpointer data )
{
  g_usleep(1000);
  return G_SOURCE_CONTINUE;
}



/* altk_main:
 */
void altk_main ( void )
{
  GMainLoop *loop;
  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
}
