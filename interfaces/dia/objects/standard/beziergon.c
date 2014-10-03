/* Dia -- an diagram creation/manipulation program
 * Copyright (C) 1999 Alexander Larsson
 *
 * beziergon.c - a beziergon shape
 * Copyright (C) 2000 James Henstridge
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

#include "intl.h"
#include "object.h"
#include "beziershape.h"
#include "connectionpoint.h"
#include "diarenderer.h"
#include "attributes.h"
#include "widgets.h"
#include "diamenu.h"
#include "message.h"
#include "properties.h"
#include "create.h"

#include "tool-icons.h"

#define DEFAULT_WIDTH 0.15

typedef struct _BeziergonProperties BeziergonProperties;

typedef struct _Beziergon {
  BezierShape bezier;

  Color line_color;
  LineStyle line_style;
  Color inner_color;
  gboolean show_background;
  real dashlength;
  real line_width;
} Beziergon;

static struct _BeziergonProperties {
  gboolean show_background;
} default_properties = { TRUE };

static ObjectChange* beziergon_move_handle(Beziergon *beziergon, Handle *handle,
					   Point *to, ConnectionPoint *cp,
					   HandleMoveReason reason, ModifierKeys modifiers);
static ObjectChange* beziergon_move(Beziergon *beziergon, Point *to);
static void beziergon_select(Beziergon *beziergon, Point *clicked_point,
			     DiaRenderer *interactive_renderer);
static void beziergon_draw(Beziergon *beziergon, DiaRenderer *renderer);
static DiaObject *beziergon_create(Point *startpoint,
				void *user_data,
				Handle **handle1,
				Handle **handle2);
static real beziergon_distance_from(Beziergon *beziergon, Point *point);
static void beziergon_update_data(Beziergon *beziergon);
static void beziergon_destroy(Beziergon *beziergon);
static DiaObject *beziergon_copy(Beziergon *beziergon);

static PropDescription *beziergon_describe_props(Beziergon *beziergon);
static void beziergon_get_props(Beziergon *beziergon, GPtrArray *props);
static void beziergon_set_props(Beziergon *beziergon, GPtrArray *props);

static void beziergon_save(Beziergon *beziergon, ObjectNode obj_node,
			  const char *filename);
static DiaObject *beziergon_load(ObjectNode obj_node, int version,
			     const char *filename);
static DiaMenu *beziergon_get_object_menu(Beziergon *beziergon,
					  Point *clickedpoint);

static ObjectTypeOps beziergon_type_ops =
{
  (CreateFunc)beziergon_create,   /* create */
  (LoadFunc)  beziergon_load,     /* load */
  (SaveFunc)  beziergon_save,      /* save */
  (GetDefaultsFunc)   NULL,
  (ApplyDefaultsFunc) NULL
};

static DiaObjectType beziergon_type =
{
  "Standard - Beziergon",   /* name */
  0,                         /* version */
  (char **) beziergon_icon,      /* pixmap */
  
  &beziergon_type_ops,      /* ops */
  NULL,                     /* pixmap_file */
  0                         /* default_user_data */
};

DiaObjectType *_beziergon_type = (DiaObjectType *) &beziergon_type;


static ObjectOps beziergon_ops = {
  (DestroyFunc)         beziergon_destroy,
  (DrawFunc)            beziergon_draw,
  (DistanceFunc)        beziergon_distance_from,
  (SelectFunc)          beziergon_select,
  (CopyFunc)            beziergon_copy,
  (MoveFunc)            beziergon_move,
  (MoveHandleFunc)      beziergon_move_handle,
  (GetPropertiesFunc)   object_create_props_dialog,
  (ApplyPropertiesDialogFunc) object_apply_props_from_dialog,
  (ObjectMenuFunc)      beziergon_get_object_menu,
  (DescribePropsFunc)   beziergon_describe_props,
  (GetPropsFunc)        beziergon_get_props,
  (SetPropsFunc)        beziergon_set_props,
  (TextEditFunc) 0,
  (ApplyPropertiesListFunc) object_apply_props,
};

static PropDescription beziergon_props[] = {
  BEZSHAPE_COMMON_PROPERTIES,
  PROP_STD_LINE_WIDTH,
  PROP_STD_LINE_COLOUR,
  PROP_STD_LINE_STYLE,
  PROP_STD_FILL_COLOUR,
  PROP_STD_SHOW_BACKGROUND,
  PROP_DESC_END
};

static PropDescription *
beziergon_describe_props(Beziergon *beziergon)
{
  if (beziergon_props[0].quark == 0)
    prop_desc_list_calculate_quarks(beziergon_props);
  return beziergon_props;
}

static PropOffset beziergon_offsets[] = {
  BEZSHAPE_COMMON_PROPERTIES_OFFSETS,
  { PROP_STDNAME_LINE_WIDTH, PROP_STDTYPE_LINE_WIDTH, offsetof(Beziergon, line_width) },
  { "line_colour", PROP_TYPE_COLOUR, offsetof(Beziergon, line_color) },
  { "line_style", PROP_TYPE_LINESTYLE,
    offsetof(Beziergon, line_style), offsetof(Beziergon, dashlength) },
  { "fill_colour", PROP_TYPE_COLOUR, offsetof(Beziergon, inner_color) },
  { "show_background", PROP_TYPE_BOOL, offsetof(Beziergon, show_background) },
  { NULL, 0, 0 }
};

static void
beziergon_get_props(Beziergon *beziergon, GPtrArray *props)
{
  object_get_props_from_offsets(&beziergon->bezier.object, beziergon_offsets,
				props);
}

static void
beziergon_set_props(Beziergon *beziergon, GPtrArray *props)
{
  object_set_props_from_offsets(&beziergon->bezier.object, beziergon_offsets,
				props);
  beziergon_update_data(beziergon);
}

static real
beziergon_distance_from(Beziergon *beziergon, Point *point)
{
  return beziershape_distance_from(&beziergon->bezier, point,
				   beziergon->line_width);
}

static int
beziergon_closest_segment(Beziergon *beziergon, Point *point)
{
  return beziershape_closest_segment(&beziergon->bezier, point,
				     beziergon->line_width);
}

static void
beziergon_select(Beziergon *beziergon, Point *clicked_point,
		  DiaRenderer *interactive_renderer)
{
  beziershape_update_data(&beziergon->bezier);
}

static ObjectChange*
beziergon_move_handle(Beziergon *beziergon, Handle *handle,
		      Point *to, ConnectionPoint *cp,
		      HandleMoveReason reason, ModifierKeys modifiers)
{
  assert(beziergon!=NULL);
  assert(handle!=NULL);
  assert(to!=NULL);

  beziershape_move_handle(&beziergon->bezier, handle, to, cp, reason, modifiers);
  beziergon_update_data(beziergon);

  return NULL;
}


static ObjectChange*
beziergon_move(Beziergon *beziergon, Point *to)
{
  beziershape_move(&beziergon->bezier, to);
  beziergon_update_data(beziergon);

  return NULL;
}

static void
beziergon_draw(Beziergon *beziergon, DiaRenderer *renderer)
{
  DiaRendererClass *renderer_ops = DIA_RENDERER_GET_CLASS (renderer);

  BezierShape *bezier = &beziergon->bezier;
  BezPoint *points;
  int n;
  
  points = &bezier->points[0];
  n = bezier->numpoints;

  renderer_ops->set_linewidth(renderer, beziergon->line_width);
  renderer_ops->set_linestyle(renderer, beziergon->line_style);
  renderer_ops->set_dashlength(renderer, beziergon->dashlength);
  renderer_ops->set_linejoin(renderer, LINEJOIN_MITER);
  renderer_ops->set_linecaps(renderer, LINECAPS_BUTT);

  if (beziergon->show_background)
    renderer_ops->fill_bezier(renderer, points, n, &beziergon->inner_color);

  renderer_ops->draw_bezier(renderer, points, n, &beziergon->line_color);

  /* these lines should only be displayed when object is selected.
   * Unfortunately the draw function is not aware of the selected
   * state.  This is a compromise until I fix this properly. */
  if (renderer->is_interactive &&
      dia_object_is_selected((DiaObject*)beziergon)) {
    beziershape_draw_control_lines(&beziergon->bezier, renderer);
  }
}

static DiaObject *
beziergon_create(Point *startpoint,
		  void *user_data,
		  Handle **handle1,
		  Handle **handle2)
{
  Beziergon *beziergon;
  BezierShape *bezier;
  DiaObject *obj;
  Point defaultx = { 1.0, 0.0 };
  Point defaulty = { 0.0, 1.0 };

  beziergon = g_new0(Beziergon, 1);
  bezier = &beziergon->bezier;
  obj = &bezier->object;

  obj->type = &beziergon_type;
  obj->ops = &beziergon_ops;

  if (user_data == NULL) {
    beziershape_init(bezier, 3);

    bezier->points[0].p1 = *startpoint;
    bezier->points[0].p3 = *startpoint;
    bezier->points[2].p3 = *startpoint;

    bezier->points[1].p1 = *startpoint;
    point_add(&bezier->points[1].p1, &defaultx);
    bezier->points[2].p2 = *startpoint;
    point_sub(&bezier->points[2].p2, &defaultx);

    bezier->points[1].p3 = *startpoint;
    point_add(&bezier->points[1].p3, &defaulty);
    bezier->points[1].p2 = bezier->points[1].p3;
    point_add(&bezier->points[1].p2, &defaultx);
    bezier->points[2].p1 = bezier->points[1].p3;
    point_sub(&bezier->points[2].p1, &defaultx);
  } else {
    BezierCreateData *bcd = (BezierCreateData*)user_data;

    beziershape_init(bezier, bcd->num_points);
    beziershape_set_points(bezier, bcd->num_points, bcd->points);
  }  
  beziergon->line_width =  attributes_get_default_linewidth();
  beziergon->line_color = attributes_get_foreground();
  beziergon->inner_color = attributes_get_background();
  attributes_get_default_line_style(&beziergon->line_style,
				    &beziergon->dashlength);
  beziergon->show_background = default_properties.show_background;

  beziergon_update_data(beziergon);

  *handle1 = bezier->object.handles[5];
  *handle2 = bezier->object.handles[2];
  return &beziergon->bezier.object;
}

static void
beziergon_destroy(Beziergon *beziergon)
{
  beziershape_destroy(&beziergon->bezier);
}

static DiaObject *
beziergon_copy(Beziergon *beziergon)
{
  Beziergon *newbeziergon;
  BezierShape *bezier, *newbezier;
  DiaObject *newobj;

  bezier = &beziergon->bezier;
 
  newbeziergon = g_malloc0(sizeof(Beziergon));
  newbezier = &newbeziergon->bezier;
  newobj = &newbezier->object;

  beziershape_copy(bezier, newbezier);

  newbeziergon->line_color = beziergon->line_color;
  newbeziergon->line_width = beziergon->line_width;
  newbeziergon->line_style = beziergon->line_style;
  newbeziergon->dashlength = beziergon->dashlength;
  newbeziergon->inner_color = beziergon->inner_color;
  newbeziergon->show_background = beziergon->show_background;

  return &newbeziergon->bezier.object;
}

static void
beziergon_update_data(Beziergon *beziergon)
{
  BezierShape *bezier = &beziergon->bezier;
  DiaObject *obj = &bezier->object;
  ElementBBExtras *extra = &bezier->extra_spacing;
  
  beziershape_update_data(bezier);
  
  extra->border_trans = beziergon->line_width / 2.0;
  beziershape_update_boundingbox(bezier);

  /* update the enclosing box using the control points */
  {
    int i, num_points = bezier->numpoints;
    obj->enclosing_box = obj->bounding_box;
    for (i = 0; i < num_points; ++i) {
      if (bezier->points[i].type != BEZ_CURVE_TO)
        continue;
      rectangle_add_point(&obj->enclosing_box, &bezier->points[i].p1);      
      rectangle_add_point(&obj->enclosing_box, &bezier->points[i].p2);      
    }
  }
  obj->position = bezier->points[0].p1;
}

static void
beziergon_save(Beziergon *beziergon, ObjectNode obj_node,
	      const char *filename)
{
  beziershape_save(&beziergon->bezier, obj_node);

  if (!color_equals(&beziergon->line_color, &color_black))
    data_add_color(new_attribute(obj_node, "line_color"),
		   &beziergon->line_color);
  
  if (beziergon->line_width != 0.1)
    data_add_real(new_attribute(obj_node, PROP_STDNAME_LINE_WIDTH),
		  beziergon->line_width);
  
  if (!color_equals(&beziergon->inner_color, &color_white))
    data_add_color(new_attribute(obj_node, "inner_color"),
		   &beziergon->inner_color);
  
  data_add_boolean(new_attribute(obj_node, "show_background"),
		   beziergon->show_background);

  if (beziergon->line_style != LINESTYLE_SOLID)
    data_add_enum(new_attribute(obj_node, "line_style"),
		  beziergon->line_style);

  if (beziergon->line_style != LINESTYLE_SOLID &&
      beziergon->dashlength != DEFAULT_LINESTYLE_DASHLEN)
    data_add_real(new_attribute(obj_node, "dashlength"),
		  beziergon->dashlength);
  
}

static DiaObject *
beziergon_load(ObjectNode obj_node, int version, const char *filename)
{
  Beziergon *beziergon;
  BezierShape *bezier;
  DiaObject *obj;
  AttributeNode attr;

  beziergon = g_malloc0(sizeof(Beziergon));

  bezier = &beziergon->bezier;
  obj = &bezier->object;
  
  obj->type = &beziergon_type;
  obj->ops = &beziergon_ops;

  beziershape_load(bezier, obj_node);

  beziergon->line_color = color_black;
  attr = object_find_attribute(obj_node, "line_color");
  if (attr != NULL)
    data_color(attribute_first_data(attr), &beziergon->line_color);

  beziergon->line_width = 0.1;
  attr = object_find_attribute(obj_node, PROP_STDNAME_LINE_WIDTH);
  if (attr != NULL)
    beziergon->line_width = data_real(attribute_first_data(attr));

  beziergon->inner_color = color_white;
  attr = object_find_attribute(obj_node, "inner_color");
  if (attr != NULL)
    data_color(attribute_first_data(attr), &beziergon->inner_color);
  
  beziergon->show_background = TRUE;
  attr = object_find_attribute(obj_node, "show_background");
  if (attr != NULL)
    beziergon->show_background = data_boolean( attribute_first_data(attr) );

  beziergon->line_style = LINESTYLE_SOLID;
  attr = object_find_attribute(obj_node, "line_style");
  if (attr != NULL)
    beziergon->line_style = data_enum(attribute_first_data(attr));

  beziergon->dashlength = DEFAULT_LINESTYLE_DASHLEN;
  attr = object_find_attribute(obj_node, "dashlength");
  if (attr != NULL)
    beziergon->dashlength = data_real(attribute_first_data(attr));

  beziergon_update_data(beziergon);

  return &beziergon->bezier.object;
}

static ObjectChange *
beziergon_add_segment_callback (DiaObject *obj, Point *clicked, gpointer data)
{
  Beziergon *bezier = (Beziergon*) obj;
  int segment;
  ObjectChange *change;
  
  segment = beziergon_closest_segment(bezier, clicked);
  change = beziershape_add_segment(&bezier->bezier, segment, clicked);

  beziergon_update_data(bezier);
  return change;
}

static ObjectChange *
beziergon_delete_segment_callback (DiaObject *obj, Point *clicked, gpointer data)
{
  int seg_nr;
  Beziergon *bezier = (Beziergon*) obj;
  ObjectChange *change;
  
  seg_nr = beziergon_closest_segment(bezier, clicked);
  change = beziershape_remove_segment(&bezier->bezier, seg_nr);

  beziergon_update_data(bezier);
  return change;
}

static ObjectChange *
beziergon_set_corner_type_callback (DiaObject *obj, Point *clicked, gpointer data)
{
  Handle *closest;
  Beziergon *beziergon = (Beziergon *) obj;
  ObjectChange *change;
  
  closest = beziershape_closest_major_handle(&beziergon->bezier, clicked);
  change = beziershape_set_corner_type(&beziergon->bezier, closest, 
				       GPOINTER_TO_INT(data));
  
  beziergon_update_data(beziergon);
  return change;
}

static DiaMenuItem beziergon_menu_items[] = {
  { N_("Add segment"), beziergon_add_segment_callback, NULL, 1 },
  { N_("Delete segment"), beziergon_delete_segment_callback, NULL, 1 },
  { NULL, NULL, NULL, 1 },
  { N_("Symmetric control"), beziergon_set_corner_type_callback, 
    GINT_TO_POINTER(BEZ_CORNER_SYMMETRIC), 1 },
  { N_("Smooth control"), beziergon_set_corner_type_callback, 
    GINT_TO_POINTER(BEZ_CORNER_SMOOTH), 1 },
  { N_("Cusp control"), beziergon_set_corner_type_callback,
    GINT_TO_POINTER(BEZ_CORNER_CUSP), 1 },
};

static DiaMenu beziergon_menu = {
  "Beziergon",
  sizeof(beziergon_menu_items)/sizeof(DiaMenuItem),
  beziergon_menu_items,
  NULL
};

static DiaMenu *
beziergon_get_object_menu(Beziergon *beziergon, Point *clickedpoint)
{
  /* Set entries sensitive/selected etc here */
  beziergon_menu_items[0].active = 1;
  beziergon_menu_items[1].active = beziergon->bezier.numpoints > 3;
  return &beziergon_menu;
}
