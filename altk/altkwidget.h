/* altkwidget.h -
 */

#ifndef _ALTKWIDGET_H_
#define _ALTKWIDGET_H_

#include "altk/altkbase.h"
#include "altk/altkregion.h"
#include "altk/altkevent.h"
#include "altk/altkstylecontext.h"
#include "altk/altkwidget-def.h"



struct _AltkDisplay;
struct _AltkStyle;
struct _AltkWindow;

typedef struct _AltkRequisition AltkRequisition;
typedef struct _AltkAllocation AltkAllocation;



/* AltkRequisition:
 */
struct _AltkRequisition
{
  gint width;
  gint height;
};



/* AtlkAllocation:
 */
struct _AltkAllocation
{
  gint x;
  gint y;
  gint width;
  gint height;
};



/* AltkWidgetFlags:
 */
typedef enum
  {
    ALTK_WIDGET_FLAG_NEEDS_RESIZE = 1 << 0,
    ALTK_WIDGET_FLAG_VISIBLE = 1 << 1,
    ALTK_WIDGET_FLAG_TOP_WIDGET = 1 << 2,
    ALTK_WIDGET_FLAG_REALIZED = 1 << 3,
    ALTK_WIDGET_FLAG_MAPPED = 1 << 4,
    ALTK_WIDGET_FLAG_NOWINDOW = 1 << 5,
    ALTK_WIDGET_FLAG_ENABLE_SHOW_ALL = 1 << 6,
    ALTK_WIDGET_FLAG_DESTROYED = 1 << 7,
  }
  AltkWidgetFlags;

#define ALTK_WIDGET_VISIBLE(w)    (ALTK_WIDGET(w)->flags & ALTK_WIDGET_FLAG_VISIBLE)
#define ALTK_WIDGET_TOP_WIDGET(w) (ALTK_WIDGET(w)->flags & ALTK_WIDGET_FLAG_TOP_WIDGET)
#define ALTK_WIDGET_REALIZED(w)   (ALTK_WIDGET(w)->flags & ALTK_WIDGET_FLAG_REALIZED)
#define ALTK_WIDGET_MAPPED(w)     (ALTK_WIDGET(w)->flags & ALTK_WIDGET_FLAG_MAPPED)
#define ALTK_WIDGET_NOWINDOW(w)   (ALTK_WIDGET(w)->flags & ALTK_WIDGET_FLAG_NOWINDOW)
#define ALTK_WIDGET_ENABLE_SHOW_ALL(w) (ALTK_WIDGET(w)->flags & ALTK_WIDGET_FLAG_ENABLE_SHOW_ALL)
#define ALTK_WIDGET_DESTROYED(w)  (ALTK_WIDGET(w)->flags & ALTK_WIDGET_FLAG_DESTROYED)



/* AltkWidget:
 */
struct _AltkWidget
{
  ALTK_WIDGET_INSTANCE_HEADER;
  /* the parent widget */
  AltkWidget *parent;
  /* children list */
  AltkWidget *children;
  AltkWidget *next;
  AltkWidget *prev;
  /* user name */
  gchar *name;
  /* flags */
  AltkWidgetFlags flags;
  /* on-screen location (relative to parent widget) */
  gint x;
  gint y;
  gint width;
  gint height;
  /* current size_request */
  AltkRequisition size_request;
  /* a style to draw the various part of the widget */
  struct _AltkStyle *style;
  AltkStyleContext *style_context;
  /* events mask */
  AltkEventMask event_mask;
  /* current state of the widget */
  AltkState state;
  /* window */
  struct _AltkWindow *window;
};



/* AltkWidgetClass:
 */
struct _AltkWidgetClass
{
  ALTK_WIDGET_CLASS_HEADER;

  struct _AltkDisplay * (*get_display) ( AltkWidget *widget );

  void (* map) ( AltkWidget *widget );

  void (* realize) ( AltkWidget *widget );

  void (* unrealize) ( AltkWidget *widget );

  void (* size_request) ( AltkWidget *widget,
                          AltkRequisition *req );

  void (* size_allocate ) ( AltkWidget *widget,
                            AltkAllocation *alloc );

  /* events */
  void (* expose_event) ( AltkWidget *widget,
                          AltkEvent *event );

  void (* expose_background_event) ( AltkWidget *widget,
                                     AltkEvent *event );

  void (* mouse_enter_event) ( AltkWidget *widget,
                               AltkEvent *event );

  void (* mouse_leave_event) ( AltkWidget *widget,
                               AltkEvent *event );

  void (* mouse_motion_event) ( AltkWidget *widget,
                                AltkEvent *event );

  void (* mouse_button_down_event) ( AltkWidget *widget,
                                     AltkEvent *event );

  void (* mouse_button_up_event) ( AltkWidget *widget,
                                   AltkEvent *event );
};


void altk_widget_event_handler ( AltkEvent *event,
                                 gpointer data );

void _altk_widget_set_parent ( AltkWidget *widget,
                               AltkWidget *parent );
void _altk_widget_unset_parent ( AltkWidget *widget );
void altk_widget_destroy ( AltkWidget *widget );
void altk_widget_map ( AltkWidget *widget );
void altk_widget_unmap ( AltkWidget *widget );
void altk_widget_realize ( AltkWidget *widget );
void altk_widget_unrealize ( AltkWidget *widget );
void altk_widget_event ( AltkWidget *widget,
                         AltkEvent *event );
void altk_widget_size_request ( AltkWidget *widget,
                                AltkRequisition *req );
void altk_widget_size_allocate ( AltkWidget *widget,
                                 AltkAllocation *alloc );
void altk_widget_set_name ( AltkWidget *widget,
                            const gchar *name );
AltkWidget *altk_widget_find ( AltkWidget *widget,
                               const gchar *name );
void altk_widget_show ( AltkWidget *widget );
void altk_widget_hide ( AltkWidget *widget );
void altk_widget_show_all ( AltkWidget *widget );
void altk_widget_set_enable_show_all ( AltkWidget *widget,
                                       gboolean enable );
AltkRegion *altk_widget_get_shape ( AltkWidget *widget );
void altk_widget_set_event_mask ( AltkWidget *widget,
                                  AltkEventMask mask );
AltkEventMask altk_widget_get_event_mask ( AltkWidget *widget );
void altk_widget_set_state ( AltkWidget *widget,
                             AltkState state );
void altk_widget_queue_draw ( AltkWidget *widget );
void altk_widget_queue_resize ( AltkWidget *widget );
struct _AltkDisplay *altk_widget_get_display ( AltkWidget *widget );
void altk_widget_get_root_coords ( AltkWidget *widget,
                                   gint *root_x,
                                   gint *root_y );
void altk_widget_intersect_visible_area ( AltkWidget *widget,
                                          AltkRegion *area );
AltkStyleContext *altk_widget_get_style_context ( AltkWidget *widget );



#endif
