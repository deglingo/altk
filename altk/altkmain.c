/* altkmain.c -
 */

#include "altk/private.h"
#include "altk/altkmain.h"
#include "altk/altkevent.h"
#include "altk/altkwidget.h"
/* [fixme] ?? */
#include "altk/altkdisplay.h"



/* [fixme] WM globals */
static AltkWidget *_entered_widget = NULL;



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



/* _filter_event:
 */
static void _filter_event ( ALLEGRO_EVENT *al_event )
{
  AltkEvent event;
  AltkDisplay *display;
  AltkWidget *fly, *entered;
  gint root_x, root_y;
  switch (al_event->type)
    {
    case ALLEGRO_EVENT_MOUSE_AXES:
      /* CL_DEBUG("mouse move: %d, %d", al_event->mouse.x, al_event->mouse.y); */
      display = altk_display_from_al_display(al_event->mouse.display);
      fly = altk_display_get_widget_at(display, al_event->mouse.x, al_event->mouse.y);
      entered = fly;
      while (entered && !(entered->event_mask & ALTK_EVENT_MOUSE_ENTER))
        entered = entered->parent;
      if (entered != _entered_widget) {
        if (_entered_widget) {
          altk_widget_get_root_coords(_entered_widget, &root_x, &root_y);
          event.type = ALTK_EVENT_MOUSE_LEAVE;
          event.crossing.widget = _entered_widget;
          event.crossing.mx = al_event->mouse.x - root_x;
          event.crossing.my = al_event->mouse.y - root_y;
          altk_event_process(&event);
        }
        if (entered) {
          altk_widget_get_root_coords(entered, &root_x, &root_y);
          event.type = ALTK_EVENT_MOUSE_ENTER;
          event.crossing.widget = entered;
          event.crossing.mx = al_event->mouse.x - root_x;
          event.crossing.my = al_event->mouse.y - root_y;
          altk_event_process(&event);
        }
        _entered_widget = entered;
      }
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      display = altk_display_from_al_display(al_event->mouse.display);
      fly = altk_display_get_widget_at(display, al_event->mouse.x, al_event->mouse.y);
      while (fly && !(fly->event_mask & ALTK_EVENT_MOUSE_BUTTON_DOWN))
        fly = fly->parent;
      if (fly) {
        event.type = ALTK_EVENT_MOUSE_BUTTON_DOWN;
        event.button.widget = fly;
        event.button.mx = al_event->mouse.x;
        event.button.my = al_event->mouse.y;
        event.button.button = al_event->mouse.button;
        altk_event_process(&event);
      }
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
      display = altk_display_from_al_display(al_event->mouse.display);
      fly = altk_display_get_widget_at(display, al_event->mouse.x, al_event->mouse.y);
      while (fly && !(fly->event_mask & ALTK_EVENT_MOUSE_BUTTON_UP))
        fly = fly->parent;
      if (fly) {
        event.type = ALTK_EVENT_MOUSE_BUTTON_UP;
        event.button.widget = fly;
        event.button.mx = al_event->mouse.x;
        event.button.my = al_event->mouse.y;
        event.button.button = al_event->mouse.button;
        altk_event_process(&event);
      }
      break;
    default:
      CL_DEBUG("[TODO] allegro event %d", al_event->type);
    }
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
        AltkDisplay *display = altk_display_from_al_display(al_event.display.source);
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
