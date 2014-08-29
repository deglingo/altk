/* altkmain.c -
 */

#include "altk/private.h"
#include "altk/altkmain.h"
/* [fixme] ?? */
#include "altk/altkdisplay.h"



/* AlSource:
 */
typedef struct _AlSource
{
  GSource g_source;
  ALLEGRO_EVENT_QUEUE *queue;
}
  AlSource;



static gboolean _al_source_prepare ( GSource *src,
                                      gint *timeout );
static gboolean _al_source_check ( GSource *src );
static gboolean _al_source_dispatch ( GSource *src,
                                      GSourceFunc callback,
                                      gpointer data );
static void _al_source_finalize ( GSource *src );

static AlSource *al_source = NULL;

static gboolean _idle_timer ( gpointer data );



/* _idle_timer:
 */
static gboolean _idle_timer ( gpointer data )
{
  g_usleep(1000);
  return G_SOURCE_CONTINUE;
}



/* _al_source_prepare:
 */
static gboolean _al_source_prepare ( GSource *src,
                                     gint *timeout )
{
  *timeout = 0;
  return !al_is_event_queue_empty(((AlSource *) src)->queue);
}



/* _al_source_check:
 */
static gboolean _al_source_check ( GSource *src )
{
  return !al_is_event_queue_empty(((AlSource *) src)->queue);
}



/* _al_source_dispatch:
 */
static gboolean _al_source_dispatch ( GSource *src,
                                      GSourceFunc callback,
                                      gpointer data )
{
  ALLEGRO_EVENT event;
  if (al_get_next_event(((AlSource *) src)->queue, &event))
    {
      CL_DEBUG("EVENT: %d", event.type);
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        AltkDisplay *display = altk_display_from_al_display(event.display.source);
        CL_DEBUG("display %p closed, bye", display);
        exit(0);
      }
    }
  return G_SOURCE_CONTINUE;
}



/* _al_source_finalize:
 */
static void _al_source_finalize ( GSource *src )
{
}



/* _altk_main_init:
 *
 * Called by altk_init(), don't use this directly.
 */
void _altk_main_init ( void )
{
  static GSourceFuncs al_funcs = {
    _al_source_prepare,
    _al_source_check,
    _al_source_dispatch,
    _al_source_finalize,
  };
  al_source = (AlSource *) g_source_new(&al_funcs, sizeof(AlSource));
  al_source->queue = al_create_event_queue();
  g_source_attach((GSource *) al_source, NULL);
  /* idle timer */
  g_idle_add_full(G_MAXINT32, _idle_timer, NULL, NULL);
}



/* altk_main_register_al_source:
 */
void altk_main_register_al_source ( ALLEGRO_EVENT_SOURCE *src )
{
  al_register_event_source(al_source->queue, src);
}



/* altk_main:
 */
void altk_main ( void )
{
  GMainLoop *loop;
  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
}
