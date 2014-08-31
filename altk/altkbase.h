/* altkbase.h - basic stuff
 */

#ifndef _ALTKBASE_H_
#define _ALTKBASE_H_

#include <glib.h> /* [removeme] */
#include <los.h>
#include <allegro5/allegro.h>



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



extern GQuark ALTK_QUARK_AL_OWNER;

int altk_init ( void );



#endif
