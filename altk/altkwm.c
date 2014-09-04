/* altkwm.c - window management
 */

#include "altk/private.h"
#include "altk/altkwm.h"
#include "altk/altkdisplay.h"



/* AlSource:
 */
typedef struct _AlSource
{
  GSource g_source;
  ALLEGRO_EVENT_QUEUE *queue;
}
  AlSource;



/* AltkWM:
 */
typedef struct _AltkWM
{
  /* a GSource for the allegro events */
  AlSource *al_source;
  /* map <ALLEGRO_DISPLAY *, AltkDisplay *> */
  GHashTable *display_map;
}
  AltkWM;

/* the wm singleton */
static AltkWM *wm = NULL;



static AltkDisplay *altk_wm_lookup_display ( ALLEGRO_DISPLAY *al_display );

static gboolean _al_source_prepare ( GSource *src,
                                      gint *timeout );
static gboolean _al_source_check ( GSource *src );
static gboolean _al_source_dispatch ( GSource *src,
                                      GSourceFunc callback,
                                      gpointer data );
static void _al_source_finalize ( GSource *src );



/* _altk_wm_init:
 *
 * Called by altk_init() - do not call directly.
 */
void _altk_wm_init ( void )
{
  static GSourceFuncs al_funcs = {
    _al_source_prepare,
    _al_source_check,
    _al_source_dispatch,
    _al_source_finalize,
  };
  /* create the wm */
  wm = g_new0(AltkWM, 1);
  wm->display_map = g_hash_table_new(NULL, NULL);
  /* init the allegro source */
  wm->al_source = (AlSource *) g_source_new(&al_funcs, sizeof(AlSource));
  wm->al_source->queue = al_create_event_queue();
  g_source_attach((GSource *) wm->al_source, NULL);
}



/* altk_wm_register_display:
 */
void altk_wm_register_display ( struct _AltkDisplay *display )
{
  ASSERT(display->al_display);
  g_hash_table_insert(wm->display_map, display->al_display, display);
}



/* altk_wm_lookup_display:
 */
static AltkDisplay *altk_wm_lookup_display ( ALLEGRO_DISPLAY *al_display )
{
  gpointer d = g_hash_table_lookup(wm->display_map, al_display);
  return d ? ALTK_DISPLAY(d) : NULL;
}



/* altk_wm_register_al_source:
 */
void altk_wm_register_al_source ( ALLEGRO_EVENT_SOURCE *source )
{
  al_register_event_source(wm->al_source->queue, source);
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



/* _filter_event:
 */
static void _filter_event ( ALLEGRO_EVENT *al_event )
{
  CL_DEBUG("[TODO] filter_event(%d)", al_event->type);
}



/* _al_source_dispatch:
 */
static gboolean _al_source_dispatch ( GSource *src,
                                      GSourceFunc callback,
                                      gpointer data )
{
  ALLEGRO_EVENT al_event;
  if (al_get_next_event(((AlSource *) src)->queue, &al_event))
    {
      /* CL_DEBUG("EVENT: %d", al_event.type); */
      if (al_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        AltkDisplay *display = altk_wm_lookup_display(al_event.display.source);
        CL_DEBUG("display %p closed, bye", display);
        exit(0);
      }
      _filter_event(&al_event);
    }
  return G_SOURCE_CONTINUE;
}



/* _al_source_finalize:
 */
static void _al_source_finalize ( GSource *src )
{
}