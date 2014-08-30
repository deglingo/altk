/* altkregion.h - stolen from GDK2
 */

/* GDK - The GIMP Drawing Kit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#ifndef _ALTKREGION_H_
#define _ALTKREGION_H_

#include "altk/altkbase.h"

G_BEGIN_DECLS

typedef struct _AltkRegion AltkRegion;

/* [FIXME] these should be elsewhere */
typedef struct _AltkPoint AltkPoint;
typedef struct _AltkRectangle AltkRectangle;
typedef struct _AltkSegment AltkSegment;
typedef struct _AltkSpan AltkSpan;

struct _AltkPoint
{
  gint x;
  gint y;
};

struct _AltkRectangle
{
  gint x;
  gint y;
  gint width;
  gint height;
};

struct _AltkSegment
{
  gint x1;
  gint y1;
  gint x2;
  gint y2;
};

struct _AltkSpan
{
  gint x;
  gint y;
  gint width;
};

/* GC fill rule for polygons
 *  EvenOddRule
 *  WindingRule
 */
typedef enum
{
  ALTK_EVEN_ODD_RULE,
  ALTK_WINDING_RULE
} AltkFillRule;

/* Types of overlapping between a rectangle and a region
 * ALTK_OVERLAP_RECTANGLE_IN: rectangle is in region
 * ALTK_OVERLAP_RECTANGLE_OUT: rectangle in not in region
 * ALTK_OVERLAP_RECTANGLE_PART: rectangle in partially in region
 */
typedef enum
{
  ALTK_OVERLAP_RECTANGLE_IN,
  ALTK_OVERLAP_RECTANGLE_OUT,
  ALTK_OVERLAP_RECTANGLE_PART
} AltkOverlapType;

typedef void (*AltkSpanFunc) (AltkSpan *span,
			     gpointer data);

AltkRegion *altk_region_new       (void);
AltkRegion *altk_region_polygon   (AltkPoint     *points,
				 gint          npoints,
				 AltkFillRule   fill_rule);
AltkRegion *altk_region_copy      (AltkRegion    *region);
AltkRegion *altk_region_rectangle (AltkRectangle *rectangle);
void       altk_region_destroy   (AltkRegion    *region);

void	       altk_region_get_clipbox    (AltkRegion     *region,
                                          AltkRectangle  *rectangle);
void           altk_region_get_rectangles (AltkRegion     *region,
                                          AltkRectangle **rectangles,
                                          gint          *n_rectangles);

gboolean       altk_region_empty    (AltkRegion    *region);
gboolean       altk_region_equal    (AltkRegion    *region1,
				    AltkRegion    *region2);
gboolean       altk_region_point_in (AltkRegion    *region,
				    int           x,
				    int           y);
AltkOverlapType altk_region_rect_in  (AltkRegion    *region,
				    AltkRectangle *rect);

void altk_region_offset          (AltkRegion    *region,
				 gint          dx,
				 gint          dy);
void altk_region_shrink          (AltkRegion    *region,
				 gint          dx,
				 gint          dy);
void altk_region_union_with_rect (AltkRegion    *region,
				 AltkRectangle *rect);
void altk_region_intersect       (AltkRegion    *source1,
				 AltkRegion    *source2);
void altk_region_union           (AltkRegion    *source1,
			 	 AltkRegion    *source2);
void altk_region_subtract        (AltkRegion    *source1,
				 AltkRegion    *source2);
void altk_region_xor             (AltkRegion    *source1,
				 AltkRegion    *source2);

void altk_region_spans_intersect_foreach (AltkRegion   *region,
					 AltkSpan     *spans,
					 int          n_spans,
					 gboolean     sorted,
					 AltkSpanFunc  function,
					 gpointer     data);

G_END_DECLS

#endif

