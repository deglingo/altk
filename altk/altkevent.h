/* altkevent.h -
 */

#ifndef _ALTKEVENT_H_
#define _ALTKEVENT_H_

#include "altk/altkbase.h"
#include "altk/altkregion.h" /* [fixme] */



struct _AltkWindow;

typedef enum _AltkEventType AltkEventType;
typedef union _AltkEvent AltkEvent;
typedef struct _AltkEventAny AltkEventAny;
typedef struct _AltkEventExpose AltkEventExpose;
typedef struct _AltkEventCrossing AltkEventCrossing;
typedef struct _AltkEventButton AltkEventButton;

typedef void (* AltkEventHandler) ( AltkEvent *event,
                                    gpointer data );

#define _ALTK_EVENT_HEADER \
  AltkEventType type; \
  struct _AltkWindow *window



/* AltkEventType:
 */
enum _AltkEventType
  {
    ALTK_EVENT_EXPOSE = 1 << 0,
    ALTK_EVENT_MOUSE_ENTER = 1 << 1,
    ALTK_EVENT_MOUSE_LEAVE = 1 << 2,
    ALTK_EVENT_EXPOSE_BACKGROUND = 1 << 3,
    ALTK_EVENT_MOUSE_BUTTON_DOWN = 1 << 4,
    ALTK_EVENT_MOUSE_BUTTON_UP = 1 << 5,
  };



/* AltkEventAny:
 */
struct _AltkEventAny
{
  _ALTK_EVENT_HEADER;
};



/* AltkEventExpose:
 */
struct _AltkEventExpose
{
  _ALTK_EVENT_HEADER;
  AltkRegion *area;
};



/* AltkEventCrossing:
 */
struct _AltkEventCrossing
{
  _ALTK_EVENT_HEADER;
  gint mx;
  gint my;
};



/* AltkEventButton:
 */
struct _AltkEventButton
{
  _ALTK_EVENT_HEADER;
  gint mx;
  gint my;
  guint button;
};



/* AltkEvent:
 */
union _AltkEvent
{
  AltkEventType type;
  AltkEventAny any;
  AltkEventExpose expose;
  AltkEventCrossing crossing;
  AltkEventButton button;
};



void altk_event_set_handler ( AltkEventHandler handler,
                              gpointer data );
void altk_event_process ( AltkEvent *event );



#endif
