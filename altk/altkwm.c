/* altkwm.c - window management
 */

#include "altk/private.h"
#include "altk/altkwm.h"
#include "altk/altkdisplay.h"
#include "altk/altkevent.h"
#include "altk/altkwindow.h"



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
  /* currently 'entered' window */
  AltkDisplay *entered_display;
  AltkWindow *flywin;
  AltkWindow *entered_window;
  gint mouse_x;
  gint mouse_y;
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



/* _mouse_motion:
 */
static void _mouse_motion ( ALLEGRO_DISPLAY *al_display,
                            gint mx,
                            gint my )
{
  AltkDisplay *display;
  AltkWindow *entered;
  AltkEvent event;
  display = altk_wm_lookup_display(al_display);
  ASSERT(display);
  wm->entered_display = display;
  wm->mouse_x = mx;
  wm->mouse_y = my;
  /* find the deepest overflew window */
  wm->flywin = altk_window_get_child_at(altk_display_get_root_window(display),
                                        mx,
                                        my);
  /* generate the entered/leave events */
  for (entered = wm->flywin; entered; entered = entered->parent) {
    if (entered->event_mask & ALTK_EVENT_MASK_MOUSE_CROSSING)
      break;
  }
  if (entered != wm->entered_window)
    {
      if (wm->entered_window)
        {
          event.type = ALTK_EVENT_MOUSE_LEAVE;
          event.crossing.window = wm->entered_window;
          event.crossing.mx = mx - wm->entered_window->root_x;
          event.crossing.my = my - wm->entered_window->root_y;
          altk_event_process(&event);
        }
      if ((wm->entered_window = entered))
        {
          event.type = ALTK_EVENT_MOUSE_ENTER;
          event.crossing.window = wm->entered_window;
          event.crossing.mx = mx - wm->entered_window->root_x;
          event.crossing.my = my - wm->entered_window->root_y;
          altk_event_process(&event);
        }
    }
}



/* _filter_event:
 */
static void _filter_event ( ALLEGRO_EVENT *al_event )
{
  switch (al_event->type)
    {
    case ALLEGRO_EVENT_MOUSE_AXES:
      _mouse_motion(al_event->mouse.display, al_event->mouse.x, al_event->mouse.y);
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      {
        AltkWindow *win;
        _mouse_motion(al_event->mouse.display, al_event->mouse.x, al_event->mouse.y);
        for (win = wm->flywin;
             win && !(win->event_mask & ALTK_EVENT_MASK_MOUSE_BUTTON); 
             win = win->parent)
          ;
        if (win)
          {
            AltkEvent event;
            event.type = ALTK_EVENT_MOUSE_BUTTON_DOWN;
            event.button.window = win;
            event.button.mx = wm->mouse_x - win->root_x;
            event.button.my = wm->mouse_y - win->root_y;
            event.button.button = al_event->mouse.button;
            altk_event_process(&event);
          }
      }
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
      {
        AltkWindow *win;
        _mouse_motion(al_event->mouse.display, al_event->mouse.x, al_event->mouse.y);
        for (win = wm->flywin;
             win && !(win->event_mask & ALTK_EVENT_MASK_MOUSE_BUTTON); 
             win = win->parent)
          ;
        if (win)
          {
            AltkEvent event;
            event.type = ALTK_EVENT_MOUSE_BUTTON_UP;
            event.button.window = win;
            event.button.mx = wm->mouse_x - win->root_x;
            event.button.my = wm->mouse_y - win->root_y;
            event.button.button = al_event->mouse.button;
            altk_event_process(&event);
          }
      }
      break;
    default:
      CL_DEBUG("[TODO] filter_event(%d)", al_event->type);
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
