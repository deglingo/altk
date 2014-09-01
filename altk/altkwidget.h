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

typedef void (* AltkForeachFunc) ( AltkWidget *widget,
                                   gpointer data );



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



/* AltkWidget:
 */
struct _AltkWidget
{
  ALTK_WIDGET_INSTANCE_HEADER;
  /* the parent widget */
  AltkWidget *parent;
  AltkWidget *children;
  AltkWidget *children_tail;
  AltkWidget *next;
  AltkWidget *prev;
  /* on-screen location */
  gint root_x;
  gint root_y;
  gint x;
  gint y;
  gint width;
  gint height;
  /* a style to draw the various part of the widget */
  struct _AltkStyle *style;
  /* graphic contexts */
  struct _AltkGC *gc[ALTK_STATE_COUNT];
  /* events mask */
  guint event_mask;
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

  void (* mouse_enter_event) ( AltkWidget *widget,
                               AltkEvent *event );

  void (* mouse_leave_event) ( AltkWidget *widget,
                               AltkEvent *event );
};



void _altk_widget_attach_child ( AltkWidget *widget,
                                 AltkWidget *child );
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



#endif
