/* altkbase.h - basic stuff
 */

#ifndef _ALTKBASE_H_
#define _ALTKBASE_H_

#include <glib.h> /* [removeme] */
#include <los.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>



/* priorities for the various event sources */
#define ALTK_PRIORITY_RESIZE 0
#define ALTK_PRIORITY_EXPOSE 100



/* AltkState:
 */
typedef enum _AltkState
  {
    ALTK_STATE_NORMAL = 0,
    ALTK_STATE_PRELIGHT,
    ALTK_STATE_ACTIVE,
    /* number of states */
    ALTK_STATE_COUNT,
  }
  AltkState;



/* AltkOrientation:
 */
typedef enum _AltkOrientation
  {
    ALTK_VERTICAL,
    ALTK_HORIZONTAL,
  }
  AltkOrientation;



/* AltkPackFlags:
 */
typedef enum _AltkPackFlags
  {
    ALTK_PACK_DUMMY, /* [todo] */
  }
  AltkPackFlags;



int altk_init ( void );



#endif
