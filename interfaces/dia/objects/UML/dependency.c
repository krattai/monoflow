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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <math.h>
#include <string.h>

#include "intl.h"
#include "object.h"
#include "orth_conn.h"
#include "diarenderer.h"
#include "attributes.h"
#include "arrows.h"

#include "properties.h"

#include "stereotype.h"
#include "uml.h"

#include "pixmaps/dependency.xpm"

typedef struct _Dependency Dependency;

struct _Dependency {
  OrthConn orth;

  Point text_pos;
  Alignment text_align;
  real text_width;
  
  Color text_color;
  Color line_color;

  int draw_arrow;
  char *name;
  char *stereotype; /* excluding << and >> */
  char *st_stereotype; /* including << and >> */
};


#define DEPENDENCY_WIDTH 0.1
#define DEPENDENCY_ARROWLEN 0.8
#define DEPENDENCY_ARROWWIDTH 0.5
#define DEPENDENCY_DASHLEN 0.4
#define DEPENDENCY_FONTHEIGHT 0.8

static DiaFont *dep_font = NULL;

static real dependency_distance_from(Dependency *dep, Point *point);
static void dependency_select(Dependency *dep, Point *clicked_point,
			      DiaRenderer *interactive_renderer);
static ObjectChange* dependency_move_handle(Dependency *dep, Handle *handle,
					    Point *to, ConnectionPoint *cp,
					    HandleMoveReason reason, ModifierKeys modifiers);
static ObjectChange* dependency_move(Dependency *dep, Point *to);
static void dependency_draw(Dependency *dep, DiaRenderer *renderer);
static DiaObject *dependency_create(Point *startpoint,
				 void *user_data,
				 Handle **handle1,
				 Handle **handle2);
static void dependency_destroy(Dependency *dep);
static DiaMenu *dependency_get_object_menu(Dependency *dep,
					   Point *clickedpoint);

static DiaObject *dependency_load(ObjectNode obj_node, int version,
			       const char *filename);
static PropDescription *dependency_describe_props(Dependency *dependency);
static void dependency_get_props(Dependency * dependency, GPtrArray *props);
static void dependency_set_props(Dependency * dependency, GPtrArray *props);

static void dependency_update_data(Dependency *dep);

static ObjectTypeOps dependency_type_ops =
{
  (CreateFunc) dependency_create,
  (LoadFunc)   dependency_load,/*using_properties*/     /* load */
  (SaveFunc)   object_save_using_properties,      /* save */
  (GetDefaultsFunc)   NULL, 
  (ApplyDefaultsFunc) NULL
};

DiaObjectType dependency_type =
{
  "UML - Dependency",   /* name */
  /* Version 0 had no autorouting and so shouldn't have it set by default. */
  1,                      /* version */
  (char **) dependency_xpm,  /* pixmap */
  
  &dependency_type_ops,      /* ops */
  NULL,                 /* pixmap_file */
  0                     /* default_user_data */
};

static ObjectOps dependency_ops = {
  (DestroyFunc)         dependency_destroy,
  (DrawFunc)            dependency_draw,
  (DistanceFunc)        dependency_distance_from,
  (SelectFunc)          dependency_select,
  (CopyFunc)            object_copy_using_properties,
  (MoveFunc)            dependency_move,
  (MoveHandleFunc)      dependency_move_handle,
  (GetPropertiesFunc)   object_create_props_dialog,
  (ApplyPropertiesDialogFunc) object_apply_props_from_dialog,
  (ObjectMenuFunc)      dependency_get_object_menu,
  (DescribePropsFunc)   dependency_describe_props,
  (GetPropsFunc)        dependency_get_props,
  (SetPropsFunc)        dependency_set_props,
  (TextEditFunc) 0,
  (ApplyPropertiesListFunc) object_apply_props,
};

static PropDescription dependency_props[] = {
  ORTHCONN_COMMON_PROPERTIES,
  /* can't use PROP_STD_TEXT_COLOUR_OPTIONAL cause it has PROP_FLAG_DONT_SAVE. It is designed to fill the Text object - not some subset */
  PROP_STD_TEXT_COLOUR_OPTIONS(PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD|PROP_FLAG_OPTIONAL),
  PROP_STD_LINE_COLOUR_OPTIONAL, 
  { "name", PROP_TYPE_STRING, PROP_FLAG_VISIBLE,
    N_("Name:"), NULL, NULL },
  { "stereotype", PROP_TYPE_STRING, PROP_FLAG_VISIBLE,
    N_("Stereotype:"), NULL, NULL },
  { "draw_arrow", PROP_TYPE_BOOL, PROP_FLAG_VISIBLE,
    N_("Show arrow:"), NULL, NULL },
  PROP_DESC_END
};

static PropDescription *
dependency_describe_props(Dependency *dependency)
{
  if (dependency_props[0].quark == 0) {
    prop_desc_list_calculate_quarks(dependency_props);
  }
  return dependency_props;
}

static PropOffset dependency_offsets[] = {
  ORTHCONN_COMMON_PROPERTIES_OFFSETS,
  { "text_colour", PROP_TYPE_COLOUR, offsetof(Dependency, text_color) },
  { "line_colour", PROP_TYPE_COLOUR, offsetof(Dependency, line_color) },
  { "name", PROP_TYPE_STRING, offsetof(Dependency, name) },
  { "stereotype", PROP_TYPE_STRING, offsetof(Dependency, stereotype) },
  { "draw_arrow", PROP_TYPE_BOOL, offsetof(Dependency, draw_arrow) },
  { NULL, 0, 0 }
};

static void
dependency_get_props(Dependency * dependency, GPtrArray *props)
{
  object_get_props_from_offsets(&dependency->orth.object,
                                dependency_offsets,props);
}

static void
dependency_set_props(Dependency *dependency, GPtrArray *props)
{
  object_set_props_from_offsets(&dependency->orth.object, 
                                dependency_offsets, props);
  g_free(dependency->st_stereotype);
  dependency->st_stereotype = NULL;
  dependency_update_data(dependency);
}

static real
dependency_distance_from(Dependency *dep, Point *point)
{
  OrthConn *orth = &dep->orth;
  return orthconn_distance_from(orth, point, DEPENDENCY_WIDTH);
}

static void
dependency_select(Dependency *dep, Point *clicked_point,
		  DiaRenderer *interactive_renderer)
{
  orthconn_update_data(&dep->orth);
}

static ObjectChange*
dependency_move_handle(Dependency *dep, Handle *handle,
		       Point *to, ConnectionPoint *cp,
		       HandleMoveReason reason, ModifierKeys modifiers)
{
  ObjectChange *change;
  assert(dep!=NULL);
  assert(handle!=NULL);
  assert(to!=NULL);
  
  change = orthconn_move_handle(&dep->orth, handle, to, cp, reason, modifiers);
  dependency_update_data(dep);

  return change;
}

static ObjectChange*
dependency_move(Dependency *dep, Point *to)
{
  ObjectChange *change;

  change = orthconn_move(&dep->orth, to);
  dependency_update_data(dep);

  return change;
}

static void
dependency_draw(Dependency *dep, DiaRenderer *renderer)
{
  DiaRendererClass *renderer_ops = DIA_RENDERER_GET_CLASS (renderer);
  OrthConn *orth = &dep->orth;
  Point *points;
  int n;
  Point pos;
  Arrow arrow;

  points = &orth->points[0];
  n = orth->numpoints;
  
  renderer_ops->set_linewidth(renderer, DEPENDENCY_WIDTH);
  renderer_ops->set_linestyle(renderer, LINESTYLE_DASHED);
  renderer_ops->set_dashlength(renderer, DEPENDENCY_DASHLEN);
  renderer_ops->set_linejoin(renderer, LINEJOIN_MITER);
  renderer_ops->set_linecaps(renderer, LINECAPS_BUTT);

  arrow.type = ARROW_LINES;
  arrow.length = DEPENDENCY_ARROWLEN;
  arrow.width = DEPENDENCY_ARROWWIDTH;

  renderer_ops->draw_polyline_with_arrows(renderer,
					   points, n,
					   DEPENDENCY_WIDTH,
					   &dep->line_color,
					   NULL, &arrow);

  renderer_ops->set_font(renderer, dep_font, DEPENDENCY_FONTHEIGHT);
  pos = dep->text_pos;
  
  if (dep->st_stereotype != NULL && dep->st_stereotype[0] != '\0') {
    renderer_ops->draw_string(renderer,
			       dep->st_stereotype,
			       &pos, dep->text_align,
			       &dep->text_color);

    pos.y += DEPENDENCY_FONTHEIGHT;
  }
  
  if (dep->name != NULL && dep->name[0] != '\0') {
    renderer_ops->draw_string(renderer,
			       dep->name,
			       &pos, dep->text_align,
			       &dep->text_color);
  }
  
}

static void
dependency_update_data(Dependency *dep)
{
  OrthConn *orth = &dep->orth;
  DiaObject *obj = &orth->object;
  PolyBBExtras *extra = &orth->extra_spacing;
  int num_segm, i;
  Point *points;
  Rectangle rect;
  
  orthconn_update_data(orth);

  dep->stereotype = remove_stereotype_from_string(dep->stereotype);
  if (!dep->st_stereotype) {
    dep->st_stereotype =  string_to_stereotype(dep->stereotype);
  }

  dep->text_width = 0.0;
  if (dep->name)
    dep->text_width = dia_font_string_width(dep->name, dep_font,
					DEPENDENCY_FONTHEIGHT);
  if (dep->stereotype)
    dep->text_width = MAX(dep->text_width,
			  dia_font_string_width(dep->stereotype, dep_font,
					    DEPENDENCY_FONTHEIGHT));
  
  extra->start_trans = 
    extra->start_long = 
    extra->middle_trans = DEPENDENCY_WIDTH/2.0;
  
  extra->end_trans = 
    extra->end_long = (dep->draw_arrow?
                       (DEPENDENCY_WIDTH + DEPENDENCY_ARROWLEN)/2.0:
                       DEPENDENCY_WIDTH/2.0);

  orthconn_update_boundingbox(orth);
  
  /* Calc text pos: */
  num_segm = dep->orth.numpoints - 1;
  points = dep->orth.points;
  i = num_segm / 2;
  
  if ((num_segm % 2) == 0) { /* If no middle segment, use horizontal */
    if (dep->orth.orientation[i]==VERTICAL)
      i--;
  }

  switch (dep->orth.orientation[i]) {
  case HORIZONTAL:
    dep->text_align = ALIGN_CENTER;
    dep->text_pos.x = 0.5*(points[i].x+points[i+1].x);
    dep->text_pos.y = points[i].y;
    if (dep->name)
      dep->text_pos.y -= dia_font_descent(dep->name,
					  dep_font,
					  DEPENDENCY_FONTHEIGHT);
    break;
  case VERTICAL:
    dep->text_align = ALIGN_LEFT;
    dep->text_pos.x = points[i].x + 0.1;
    dep->text_pos.y =
      0.5*(points[i].y+points[i+1].y);
    if (dep->name)
      dep->text_pos.y -= dia_font_descent(dep->name,
					  dep_font,
					  DEPENDENCY_FONTHEIGHT);
    break;
  }

  /* Add the text recangle to the bounding box: */
  rect.left = dep->text_pos.x;
  if (dep->text_align == ALIGN_CENTER)
    rect.left -= dep->text_width/2.0;
  rect.right = rect.left + dep->text_width;
  rect.top = dep->text_pos.y;
  if (dep->name)
    rect.top -= dia_font_ascent(dep->name,
				dep_font,
				DEPENDENCY_FONTHEIGHT);
  rect.bottom = rect.top + 2*DEPENDENCY_FONTHEIGHT;

  rectangle_union(&obj->bounding_box, &rect);
}

static ObjectChange *
dependency_add_segment_callback(DiaObject *obj, Point *clicked, gpointer data)
{
  ObjectChange *change;
  change = orthconn_add_segment((OrthConn *)obj, clicked);
  dependency_update_data((Dependency *)obj);
  return change;
}

static ObjectChange *
dependency_delete_segment_callback(DiaObject *obj, Point *clicked, gpointer data)
{
  ObjectChange *change;
  change = orthconn_delete_segment((OrthConn *)obj, clicked);
  dependency_update_data((Dependency *)obj);
  return change;
}


static DiaMenuItem object_menu_items[] = {
  { N_("Add segment"), dependency_add_segment_callback, NULL, 1 },
  { N_("Delete segment"), dependency_delete_segment_callback, NULL, 1 },
  ORTHCONN_COMMON_MENUS,
};

static DiaMenu object_menu = {
  "Dependency",
  sizeof(object_menu_items)/sizeof(DiaMenuItem),
  object_menu_items,
  NULL
};

static DiaMenu *
dependency_get_object_menu(Dependency *dep, Point *clickedpoint)
{
  OrthConn *orth;

  orth = &dep->orth;
  /* Set entries sensitive/selected etc here */
  object_menu_items[0].active = orthconn_can_add_segment(orth, clickedpoint);
  object_menu_items[1].active = orthconn_can_delete_segment(orth, clickedpoint);
  orthconn_update_object_menu(orth, clickedpoint, &object_menu_items[2]);

  return &object_menu;
}

static DiaObject *
dependency_create(Point *startpoint,
	       void *user_data,
  	       Handle **handle1,
	       Handle **handle2)
{
  Dependency *dep;
  OrthConn *orth;
  DiaObject *obj;

  if (dep_font == NULL) {
      dep_font = dia_font_new_from_style(DIA_FONT_MONOSPACE, DEPENDENCY_FONTHEIGHT);
  }
  
  dep = g_new0(Dependency, 1);
  orth = &dep->orth;
  obj = (DiaObject *)dep;
  
  obj->type = &dependency_type;

  obj->ops = &dependency_ops;

  orthconn_init(orth, startpoint);

  dep->text_color = color_black;
  dep->line_color = attributes_get_foreground();
  dep->draw_arrow = TRUE;
  dep->name = NULL;
  dep->stereotype = NULL;
  dep->st_stereotype = NULL;
  dep->text_width = 0;

  dependency_update_data(dep);
  
  *handle1 = orth->handles[0];
  *handle2 = orth->handles[orth->numpoints-2];

  return (DiaObject *)dep;
}

static void
dependency_destroy(Dependency *dep)
{
  g_free(dep->name);
  g_free(dep->stereotype);
  g_free(dep->st_stereotype);

  orthconn_destroy(&dep->orth);
}

static DiaObject *
dependency_load(ObjectNode obj_node, int version, const char *filename)
{
  DiaObject *obj = object_load_using_properties(&dependency_type,
                                                obj_node,version,filename);
  if (version == 0) {
    AttributeNode attr;
    /* In old objects with no autorouting, set it to false. */
    attr = object_find_attribute(obj_node, "autorouting");
    if (attr == NULL)
      ((OrthConn*)obj)->autorouting = FALSE;
  }
  return obj;
}


