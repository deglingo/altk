/* altkwidget.h -
 */

#ifndef _ALTKWIDGET_H_
#define _ALTKWIDGET_H_

#include "altk/altkbase.h"
#include "altk/altkregion.h"
#include "altk/altkevent.h"
#include "altk/altkwidget-def.h"



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



/* AltkWidget:
 */
struct _AltkWidget
{
  ALTK_WIDGET_INSTANCE_HEADER;
  /* on-screen location */
  gint root_x;
  gint root_y;
  gint x;
  gint y;
  gint width;
  gint height;
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

  /* events */
  void (* expose_event) ( AltkWidget *widget,
                          AltkEvent *event );
};



void altk_widget_event ( AltkWidget *widget,
                         AltkEvent *event );
void altk_widget_size_request ( AltkWidget *widget,
                                AltkRequisition *req );
void altk_widget_size_allocate ( AltkWidget *widget,
                                 AltkAllocation *alloc );
AltkRegion *altk_widget_get_shape ( AltkWidget *widget );



#endif
