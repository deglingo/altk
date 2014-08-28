/* altkdisplay.c -
 */

#include "altk/altkdisplay.h"

#include "altk/altkdisplay.inl"



static GSource *_display_source_new ( void );
static gboolean _display_source_prepare ( GSource *src,
                                          gint *timeout );
static gboolean _display_source_check ( GSource *src );
static gboolean _display_source_dispatch ( GSource *src,
                                           GSourceFunc callback,
                                           gpointer data );
static void _display_source_finalize ( GSource *src );



/* DisplaySource:
 */
typedef struct _DisplaySource
{
  GSource g_source;
  ALLEGRO_EVENT_QUEUE *queue;
}
  DisplaySource;



/* _display_source_new:
 */
static GSource *_display_source_new ( void )
{
  static GSourceFuncs funcs =
    {
      _display_source_prepare,
      _display_source_check,
      _display_source_dispatch,
      _display_source_finalize,
    };
  DisplaySource *src = (DisplaySource *) g_source_new(&funcs, sizeof(DisplaySource));
  src->queue = al_create_event_queue();
  return (GSource *) src;
}



/* _display_source_prepare:
 */
static gboolean _display_source_prepare ( GSource *src,
                                          gint *timeout )
{
  DisplaySource *dsrc = (DisplaySource *) src;
  *timeout = 0;
  return !al_is_event_queue_empty(dsrc->queue);
}



/* _display_source_check:
 */
static gboolean _display_source_check ( GSource *src )
{
  DisplaySource *dsrc = (DisplaySource *) src;
  return !al_is_event_queue_empty(dsrc->queue);
}



/* _display_source_dispatch:
 */
static gboolean _display_source_dispatch ( GSource *src,
                                           GSourceFunc callback,
                                           gpointer data )
{
  DisplaySource *dsrc = (DisplaySource *) src;
  ALLEGRO_EVENT event;
  if (al_get_next_event(dsrc->queue, &event))
    {
      switch (event.type) {
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        /* [fixme] */
        fprintf(stderr, "display closed, bye\n");
        exit(0);
        break;
      default:
        fprintf(stderr, "[todo] display event: %d\n", event.type);
      }
    }
  return TRUE;
}



/* _display_source_finalize:
 */
static void _display_source_finalize ( GSource *src )
{
}



/* altk_display_new:
 */
AltkDisplay *altk_display_new ( void )
{
  AltkDisplay *display;
  display = ALTK_DISPLAY(l_object_new(ALTK_CLASS_DISPLAY, NULL));
  display->g_source = _display_source_new();
  return display;
}



/* altk_display_open:
 */
void altk_display_open ( AltkDisplay *display )
{
  display->al_display = al_create_display(640, 480);
  al_register_event_source(((DisplaySource *) display->g_source)->queue,
                           al_get_display_event_source(display->al_display));
  display->g_source_uid = g_source_attach(display->g_source, NULL);
}



/* altk_display_attach_widget:
 */
void altk_display_attach_widget ( AltkDisplay *display,
                                  struct _AltkWidget *widget )
{
}
