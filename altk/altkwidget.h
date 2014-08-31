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



#endif
