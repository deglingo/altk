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
    ALTK_PACK_HEXPAND       = 1 << 0,
    ALTK_PACK_VEXPAND       = 1 << 1,
    ALTK_PACK_HFILL         = 1 << 2,
    ALTK_PACK_VFILL         = 1 << 3,
    ALTK_PACK_ANCHOR_TOP    = 1 << 4,
    ALTK_PACK_ANCHOR_BOTTOM = 1 << 5,
    ALTK_PACK_ANCHOR_LEFT   = 1 << 6,
    ALTK_PACK_ANCHOR_RIGHT  = 1 << 7,
  }
  AltkPackFlags;

#define ALTK_PACK_EXPAND              (ALTK_PACK_HEXPAND | ALTK_PACK_VEXPAND)
#define ALTK_PACK_FILL                (ALTK_PACK_HFILL | ALTK_PACK_VFILL)
#define ALTK_PACK_EXPAND_FILL         (ALTK_PACK_EXPAND | ALTK_PACK_FILL)
#define ALTK_PACK_VEXPAND_FILL        (ALTK_PACK_VEXPAND | ALTK_PACK_VFILL)
#define ALTK_PACK_HEXPAND_FILL        (ALTK_PACK_HEXPAND | ALTK_PACK_HFILL)
#define ALTK_PACK_ANCHOR_MIDDLE       (0)
#define ALTK_PACK_ANCHOR_TOP_LEFT     (ALTK_PACK_ANCHOR_TOP | ALTK_PACK_ANCHOR_LEFT)
#define ALTK_PACK_ANCHOR_TOP_RIGHT    (ALTK_PACK_ANCHOR_TOP | ALTK_PACK_ANCHOR_RIGHT)
#define ALTK_PACK_ANCHOR_BOTTOM_LEFT  (ALTK_PACK_ANCHOR_BOTTOM | ALTK_PACK_ANCHOR_LEFT)
#define ALTK_PACK_ANCHOR_BOTTOM_RIGHT (ALTK_PACK_ANCHOR_BOTTOM | ALTK_PACK_ANCHOR_RIGHT)



/* AltkSizeHints:
 */
typedef enum _AltkSizeHints
  {
    ALTK_SIZE_HINT_MAXIMIZED = 1 << 0,
  }
  AltkSizeHints;



/* AltkSizeDistrib:
 */
typedef struct _AltkSizeDistrib
{
  gint size;
  gint n_items;
  gint remainder;
  gint current;
}
  AltkSizeDistrib;



int altk_init ( void );

void altk_size_distrib_init ( AltkSizeDistrib *distrib,
                              gint size,
                              gint n_items );
guint altk_size_distrib_next ( AltkSizeDistrib *distrib );



#endif
