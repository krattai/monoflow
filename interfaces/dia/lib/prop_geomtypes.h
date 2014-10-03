/* Dia -- a diagram creation/manipulation program -*- c -*-
 * Copyright (C) 1998 Alexander Larsson
 *
 * Property system for dia objects/shapes.
 * Copyright (C) 2000 James Henstridge
 * Copyright (C) 2001 Cyrille Chepelov
 * Major restructuration done in August 2001 by C. Chepelov
 *
 * Property types for "geometric" types (real, points, rectangles, etc.)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifndef PROP_GEOMTYPES_H
#define PROP_GEOMTYPES_H

#include "properties.h"
#include "geometry.h"

typedef struct {
  Property common;
  real real_data;
} RealProperty;

typedef struct {
  Property common;
  real length_data;
} LengthProperty;

typedef struct {
  Property common;
  real fontsize_data;
} FontsizeProperty;

typedef struct {
  Property common;
  Point point_data;
} PointProperty;

typedef struct {
  Property common;
  GArray *pointarray_data;
} PointarrayProperty;

typedef struct {
  Property common;
  BezPoint bezpoint_data;
} BezPointProperty;

typedef struct {
  Property common;
  GArray *bezpointarray_data;
} BezPointarrayProperty;

typedef struct {
  Property common;
  Rectangle rect_data;
} RectProperty;

typedef struct {
  Property common;
  Point endpoints_data[2];
} EndpointsProperty;

typedef struct {
  Property common;
  gint connpoint_line_data;
} Connpoint_LineProperty;

void prop_geomtypes_register(void);

#endif
