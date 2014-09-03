/* altkwidget.h -
 */

#ifndef _ALTKWIDGET_H_
#define _ALTKWIDGET_H_

#include "altk/altkbase.h"
#include "altk/altkregion.h"
#include "altk/altkevent.h"
#include "altk/altkwidget-def.h"



struct _AltkDisplay;
struct _AltkStyle;
struct _AltkGC;

typedef struct _AltkRequisition AltkRequisition;
typedef struct _AltkAllocation AltkAllocation;

typedef gboolean (* AltkForeachFunc) ( AltkWidget *widget,
                                       gpointer data );

#define ALTK_FOREACH_CONT (TRUE)
#define ALTK_FOREACH_STOP (FALSE)



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
  }
  AltkWidgetFlags;

#define ALTK_WIDGET_VISIBLE(w) (ALTK_WIDGET(w)->flags & ALTK_WIDGET_FLAG_VISIBLE)



/* AltkWidget:
 */
struct _AltkWidget
{
  ALTK_WIDGET_INSTANCE_HEADER;
  /* the parent widget */
  AltkWidget *parent;
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
  /* graphic contexts */
  struct _AltkGC *gc[ALTK_STATE_COUNT];
  /* events mask */
  guint event_mask;
  /* current state of the widget */
  AltkState state;
  /* [FIXME] the display on which this widget is attached */
  struct _AltkDisplay *display;
};



/* AltkWidgetClass:
 */
struct _AltkWidgetClass
{
  ALTK_WIDGET_CLASS_HEADER;

  void (* size_request) ( AltkWidget *widget,
                          AltkRequisition *req );

  void (* size_allocate ) ( AltkWidget *widget,
                            AltkAllocation *alloc );

  /* [FIXME] should be in AltkContainer only ? */
  void (* forall) ( AltkWidget *widget,
                    AltkForeachFunc func,
                    gpointer data );

  /* events */
  void (* expose_event) ( AltkWidget *widget,
                          AltkEvent *event );

  void (* expose_background_event) ( AltkWidget *widget,
                                     AltkEvent *event );

  void (* mouse_enter_event) ( AltkWidget *widget,
                               AltkEvent *event );

  void (* mouse_leave_event) ( AltkWidget *widget,
                               AltkEvent *event );

  void (* mouse_button_down_event) ( AltkWidget *widget,
                                     AltkEvent *event );

  void (* mouse_button_up_event) ( AltkWidget *widget,
                                   AltkEvent *event );
};



void _altk_widget_set_parent ( AltkWidget *widget,
                               AltkWidget *parent );
void altk_widget_map ( AltkWidget *widget,
                       struct _AltkDisplay *display );
void altk_widget_event ( AltkWidget *widget,
                         AltkEvent *event );
void altk_widget_size_request ( AltkWidget *widget,
                                AltkRequisition *req );
void altk_widget_size_allocate ( AltkWidget *widget,
                                 AltkAllocation *alloc );
AltkRegion *altk_widget_get_shape ( AltkWidget *widget );
void altk_widget_forall ( AltkWidget *widget,
                          AltkForeachFunc func,
                          gpointer data );
void altk_widget_set_event_mask ( AltkWidget *widget,
                                  AltkEventType mask );
AltkEventType altk_widget_get_event_mask ( AltkWidget *widget );
void altk_widget_set_state ( AltkWidget *widget,
                             AltkState state );
void altk_widget_queue_draw ( AltkWidget *widget,
                              gboolean children );
void altk_widget_queue_resize ( AltkWidget *widget );
struct _AltkDisplay *altk_widget_get_display ( AltkWidget *widget );
void altk_widget_get_root_coords ( AltkWidget *widget,
                                   gint *root_x,
                                   gint *root_y );



#endif
