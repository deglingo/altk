/* altkevent.h -
 */

#ifndef _ALTKEVENT_H_
#define _ALTKEVENT_H_

#include "altk/altkbase.h"
#include "altk/altkregion.h" /* [fixme] */



struct _AltkWidget;

typedef enum _AltkEventType AltkEventType;
typedef union _AltkEvent AltkEvent;
typedef struct _AltkEventAny AltkEventAny;
typedef struct _AltkEventExpose AltkEventExpose;

#define _ALTK_EVENT_HEADER \
  AltkEventType type; \
  struct _AltkWidget *widget



/* AltkEventType:
 */
enum _AltkEventType
  {
    ALTK_EVENT_EXPOSE = 1 << 0,
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



/* AltkEvent:
 */
union _AltkEvent
{
  AltkEventType type;
  AltkEventAny any;
  AltkEventExpose expose;
};



void altk_event_process ( AltkEvent *event );



#endif
