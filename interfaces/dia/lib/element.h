/* Dia -- an diagram creation/manipulation program
 * Copyright (C) 1998 Alexander Larsson
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

/* ** \file element.h -- Definition of a diagram - usual rectangular - object with eight handles  */
#ifndef ELEMENT_H
#define ELEMENT_H

#include "diatypes.h"
#include "object.h"
#include "handle.h"
#include "connectionpoint.h"
#include "boundingbox.h"

/*!
 * \class _Element
 * \brief Beside OrthCon one of the most use object classes
 *
 * This is a subclass of DiaObject used to help implementing objects
 * of a type with 8 handles. 
 */
struct _Element {
  DiaObject object; /*!< inheritance */
  
  Handle resize_handles[8]; /*!< not only for resizing but may also be used for connections */

  Point corner;
  real width;
  real height;

  ElementBBExtras extra_spacing;
};

void element_update_handles(Element *elem);
void element_update_connections_rectangle(Element *elem,
					  ConnectionPoint* cps);
void element_update_boundingbox(Element *elem);
void element_init(Element *elem, int num_handles, int num_connections);
void element_destroy(Element *elem);
void element_copy(Element *from, Element *to);
ObjectChange* element_move_handle(Element *elem, HandleId id,
				  Point *to, ConnectionPoint *cp,
				  HandleMoveReason reason, 
				  ModifierKeys modifiers);
void element_move_handle_aspect(Element *elem, HandleId id,
				Point *to, real aspect_ratio);

void element_save(Element *elem, ObjectNode obj_node);
void element_load(Element *elem, ObjectNode obj_node);

/* base property stuff ... */
static PropNumData width_range = { -G_MAXFLOAT, G_MAXFLOAT, 0.1};

#define ELEMENT_COMMON_PROPERTIES \
  OBJECT_COMMON_PROPERTIES, \
  { "elem_corner", PROP_TYPE_POINT, PROP_FLAG_NO_DEFAULTS, \
    "Element corner", "The corner of the element"}, \
  { "elem_width", PROP_TYPE_REAL, PROP_FLAG_DONT_MERGE | PROP_FLAG_NO_DEFAULTS | PROP_FLAG_OPTIONAL, \
    "Element width", "The width of the element", &width_range}, \
  { "elem_height", PROP_TYPE_REAL, PROP_FLAG_DONT_MERGE  | PROP_FLAG_NO_DEFAULTS | PROP_FLAG_OPTIONAL, \
    "Element height", "The height of the element", &width_range}

   /* Would like to have the frame, but need to figure out why
      custom_object ext_attributes lose their updates when they're on
      (see http://mail.gnome.org/archives/dia-list/2005-August/msg00053.html)
   */
      /*
#define ELEMENT_COMMON_PROPERTIES \
  OBJECT_COMMON_PROPERTIES, \
  PROP_FRAME_BEGIN("size",0,N_("Object dimensions")), \
  { "elem_corner", PROP_TYPE_POINT, 0, \
    "Element corner", "The corner of the element"}, \
  { "elem_width", PROP_TYPE_REAL, PROP_FLAG_VISIBLE, \
    "Element width", "The width of the element", &width_range}, \
  { "elem_height", PROP_TYPE_REAL, PROP_FLAG_VISIBLE, \
    "Element height", "The height of the element", &width_range}, \
  PROP_FRAME_END("size", 0)
      */
#define ELEMENT_COMMON_PROPERTIES_OFFSETS \
  OBJECT_COMMON_PROPERTIES_OFFSETS, \
  { "elem_corner", PROP_TYPE_POINT, offsetof(Element, corner) }, \
  { "elem_width", PROP_TYPE_REAL, offsetof(Element, width) }, \
  { "elem_height", PROP_TYPE_REAL, offsetof(Element, height) }

#endif /* ELEMENT_H */
