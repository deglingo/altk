/* altkmain.c -
 */

#include "altk/altkmain.h"



/* altk_main:
 */
void altk_main ( void )
{
  GMainLoop *loop;
  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
}
