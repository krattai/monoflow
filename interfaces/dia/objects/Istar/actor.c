/* Dia -- an diagram creation/manipulation program
 * Copyright (C) 1998 Alexander Larsson
 *
 * Objects for drawing i* actor diagrams.
 * Copyright (C) 2002 Christophe Ponsard
 *
 * based on Flowchart toolbox -- objects for drawing flowcharts.
 * Copyright (C) 1999 James Henstridge.
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
#include "element.h"
#include "connectionpoint.h"
#include "diarenderer.h"
#include "attributes.h"
#include "text.h"
#include "widgets.h"
#include "message.h"
#include "properties.h"

#include "pixmaps/actor.xpm"

#define ACTOR_RADIUS 2.0
#define ACTOR_FG_COLOR color_black
#define ACTOR_BG_COLOR color_white
#define ACTOR_BORDER_WIDTH 0.12
#define ACTOR_FONT 0.7

#define NUM_CONNECTIONS 17

/* used when resizing to decide which side of the shape to expand/shrink */
typedef enum {
  ANCHOR_MIDDLE,
  ANCHOR_START,
  ANCHOR_END
} AnchorShape;

typedef enum {
  ACTOR_UNSPECIFIED,
  ACTOR_AGENT,
  ACTOR_POSITION,
  ACTOR_ROLE
} ActorType;

static PropEnumData prop_actor_type_data[] = {
  { N_("Unspecified"), ACTOR_UNSPECIFIED },
  { N_("Agent"),       ACTOR_AGENT },
  { N_("Position"),    ACTOR_POSITION },
  { N_("Role"),        ACTOR_ROLE },
  { NULL, 0}
};

typedef struct _Actor Actor;

struct _Actor {
  Element element;
  ActorType type;
  ConnectionPoint connections[NUM_CONNECTIONS];
  Text *text;
  TextAttributes attrs;
  int init;
};

static real actor_distance_from(Actor *actor, Point *point);
static void actor_select(Actor *actor, Point *clicked_point,
		       DiaRenderer *interactive_renderer);
static ObjectChange* actor_move_handle(Actor *actor, Handle *handle,
			    Point *to, ConnectionPoint *cp,
			    HandleMoveReason reason, ModifierKeys modifiers);
static ObjectChange* actor_move(Actor *actor, Point *to);
static void actor_draw(Actor *actor, DiaRenderer *renderer);
static void actor_update_data(Actor *actor, AnchorShape h,AnchorShape v);
static DiaObject *actor_create(Point *startpoint,
			  void *user_data,
			  Handle **handle1,
			  Handle **handle2);
static void actor_destroy(Actor *actor);

static PropDescription *actor_describe_props(Actor *actor);
static void actor_get_props(Actor *actor, GPtrArray *props);
static void actor_set_props(Actor *actor, GPtrArray *props);

static DiaObject *actor_load(ObjectNode obj_node, int version, const char *filename);

static ObjectTypeOps actor_type_ops =
{
  (CreateFunc) actor_create,
  (LoadFunc)   actor_load,
  (SaveFunc)   object_save_using_properties,
  (GetDefaultsFunc)   NULL,
  (ApplyDefaultsFunc) NULL
};

DiaObjectType istar_actor_type =
{
  "Istar - actor",           /* name */
  0,                         /* version */
  (char **) istar_actor_xpm, /* pixmap */

  &actor_type_ops            /* ops */
};

static ObjectOps actor_ops = {
  (DestroyFunc)         actor_destroy,
  (DrawFunc)            actor_draw,
  (DistanceFunc)        actor_distance_from,
  (SelectFunc)          actor_select,
  (CopyFunc)            object_copy_using_properties,
  (MoveFunc)            actor_move,
  (MoveHandleFunc)      actor_move_handle,
  (GetPropertiesFunc)   object_create_props_dialog,
  (ApplyPropertiesDialogFunc) object_apply_props_from_dialog,
  (ObjectMenuFunc)      NULL,
  (DescribePropsFunc)   actor_describe_props,
  (GetPropsFunc)        actor_get_props,
  (SetPropsFunc)        actor_set_props,
  (TextEditFunc) 0,
  (ApplyPropertiesListFunc) object_apply_props,
};

static PropDescription actor_props[] = {
  ELEMENT_COMMON_PROPERTIES,
  { "type", PROP_TYPE_ENUM, PROP_FLAG_VISIBLE|PROP_FLAG_NO_DEFAULTS,
    N_("Type"),
    N_("Type"),
    prop_actor_type_data},

  { "text", PROP_TYPE_TEXT, 0,NULL,NULL},

  PROP_DESC_END
};

static PropDescription *
actor_describe_props(Actor *actor)
{

  if (actor_props[0].quark == 0)
    prop_desc_list_calculate_quarks(actor_props);
  return actor_props;
}


static PropOffset actor_offsets[] = {
  ELEMENT_COMMON_PROPERTIES_OFFSETS,
  { "type", PROP_TYPE_ENUM, offsetof(Actor,type)},
  { "text", PROP_TYPE_TEXT, offsetof(Actor,text)},
  {NULL}
};

static void
actor_get_props(Actor *actor, GPtrArray *props)
{
  text_get_attributes(actor->text,&actor->attrs);
  object_get_props_from_offsets(&actor->element.object,
                                actor_offsets,props);
}

static void
actor_set_props(Actor *actor, GPtrArray *props)
{
  if (actor->init==-1) { actor->init++; return; } /* init bug workaround */

  object_set_props_from_offsets(&actor->element.object,
                                actor_offsets,props);
  apply_textattr_properties(props,actor->text,"text",&actor->attrs);
  actor_update_data(actor, ANCHOR_MIDDLE, ANCHOR_MIDDLE);
}

/* returns the radius of the actor along the ray from the centre of the
 * actor to the point (px, py) */
static real
actor_radius(Actor *actor, real px, real py)
{
  Element *elem = &actor->element;
  real w2 = elem->width * elem->width;
  real h2 = elem->height * elem->height;
  real scale;

  /* find the point of intersection between line (x=cx+(px-cx)t; y=cy+(py-cy)t)
   * and actor ((x-cx)^2)/(w/2)^2 + ((y-cy)^2)/(h/2)^2 = 1 */
  /* radius along ray is sqrt((px-cx)^2 * t^2 + (py-cy)^2 * t^2) */

  /* normalize coordinates ... */
  px -= elem->corner.x + elem->width  / 2;
  py -= elem->corner.y + elem->height / 2;
  /* square them ... */
  px *= px;
  py *= py;

  scale = w2 * h2 / (4*h2*px + 4*w2*py);
  return sqrt((px + py)*scale);
}

static real
actor_distance_from(Actor *actor, Point *point)
{
  Element *elem = &actor->element;
  Point c;
  real dist, rad;

  c.x = elem->corner.x + elem->width / 2;
  c.y = elem->corner.y + elem->height/ 2;
  dist = distance_point_point(point, &c);
  rad = actor_radius(actor, point->x, point->y) + ACTOR_BORDER_WIDTH/2;


  if (dist <= rad)
    return 0;
  return dist - rad;
}

static void
actor_select(Actor *actor, Point *clicked_point,
	   DiaRenderer *interactive_renderer)
{
  text_set_cursor(actor->text, clicked_point, interactive_renderer);
  text_grab_focus(actor->text, &actor->element.object);

  element_update_handles(&actor->element);
}

static ObjectChange*
actor_move_handle(Actor *actor, Handle *handle,
		Point *to, ConnectionPoint *cp, 
		HandleMoveReason reason, ModifierKeys modifiers)
{
  AnchorShape horiz = ANCHOR_MIDDLE, vert = ANCHOR_MIDDLE;

  assert(actor!=NULL);
  assert(handle!=NULL);
  assert(to!=NULL);

  element_move_handle(&actor->element, handle->id, to, cp, reason, modifiers);

  switch (handle->id) {
  case HANDLE_RESIZE_NW:
    horiz = ANCHOR_END; vert = ANCHOR_END; break;
  case HANDLE_RESIZE_N:
    vert = ANCHOR_END; break;
  case HANDLE_RESIZE_NE:
    horiz = ANCHOR_START; vert = ANCHOR_END; break;
  case HANDLE_RESIZE_E:
    horiz = ANCHOR_START; break;
  case HANDLE_RESIZE_SE:
    horiz = ANCHOR_START; vert = ANCHOR_START; break;
  case HANDLE_RESIZE_S:
    vert = ANCHOR_START; break;
  case HANDLE_RESIZE_SW:
    horiz = ANCHOR_END; vert = ANCHOR_START; break;
  case HANDLE_RESIZE_W:
    horiz = ANCHOR_END; break;
  default:
    break;
  }
  actor_update_data(actor, horiz, vert);
  return NULL;
}

static ObjectChange*
actor_move(Actor *actor, Point *to)
{
  actor->element.corner = *to;

  actor_update_data(actor, ANCHOR_MIDDLE, ANCHOR_MIDDLE);
  return NULL;
}

/* drawing stuff */
static void
actor_draw(Actor *actor, DiaRenderer *renderer)
{
  DiaRendererClass *renderer_ops = DIA_RENDERER_GET_CLASS (renderer);
  Element *elem;
  Point center;
  double dx,dy,r,th;
  Point ps1,ps2,pi1,pi2;

  assert(actor != NULL);
  assert(renderer != NULL);

  elem = &actor->element;

  center.x = elem->corner.x + elem->width/2;
  center.y = elem->corner.y + elem->height/2;

  /* background */
  renderer_ops->set_fillstyle(renderer, FILLSTYLE_SOLID);

  renderer_ops->fill_ellipse(renderer, &center, elem->width, elem->height, &ACTOR_BG_COLOR);

  /* foreground */
  renderer_ops->set_linewidth(renderer, ACTOR_BORDER_WIDTH);
  renderer_ops->set_linejoin(renderer, LINEJOIN_MITER);
  renderer_ops->draw_ellipse(renderer, &center, elem->width, elem->height, &ACTOR_FG_COLOR);

  /* text */
  text_draw(actor->text, renderer);

  /* computing and drawing decorations */
  r  = elem->height/2.0;
  th = actor->text->height;
  dy = r - th;
  dx = r*r - dy*dy;
  if (dx>0) dx=sqrt(dx); else dx=0;
  dx= r-dx;
  ps1.x=elem->corner.x + dx;
  ps1.y=elem->corner.y + th;
  ps2.x=elem->corner.x + elem->width-dx;
  ps2.y=ps1.y;
  pi1.x=ps1.x;
  pi1.y=elem->corner.y + elem->height - th;
  pi2.x=ps2.x;
  pi2.y=pi1.y;

  renderer_ops->set_linewidth(renderer, ACTOR_BORDER_WIDTH);
  switch (actor->type) {
    case ACTOR_AGENT:
      renderer_ops->draw_line(renderer, &ps1, &ps2, &ACTOR_FG_COLOR);
      break;
    case ACTOR_POSITION:
      renderer_ops->draw_line(renderer, &ps1, &ps2, &ACTOR_FG_COLOR);
      renderer_ops->draw_line(renderer, &pi1, &pi2, &ACTOR_FG_COLOR);
      break;
    case ACTOR_ROLE:
      renderer_ops->draw_line(renderer, &pi1, &pi2, &ACTOR_FG_COLOR);
      break;
    case ACTOR_UNSPECIFIED:
      break;
  }
}


static void
actor_update_data(Actor *actor, AnchorShape horiz, AnchorShape vert)
{
  Element *elem = &actor->element;
  DiaObject *obj = &elem->object;
  ElementBBExtras *extra = &elem->extra_spacing;

  Point center, bottom_right,p,c;
  real width, height, dw, dh;
  real radius, mradius;
  int i;

  center = bottom_right = elem->corner;
  center.x += elem->width/2;
  bottom_right.x += elem->width;
  center.y += elem->height/2;
  bottom_right.y += elem->height;

  text_calc_boundingbox(actor->text, NULL);
  width = actor->text->max_width+0.5;
  height = actor->text->height * (actor->text->numlines + 3); /* added 3 blank lines for top */

  /* minimal radius */
  mradius=width;
  if (mradius<height) mradius=height;
  if (mradius<ACTOR_RADIUS) mradius=ACTOR_RADIUS;
  
  /* radius */
  radius=elem->width;
  if (radius<elem->height) radius=elem->height;
  
  /* enforce (minimal or resized) radius */
  if (radius<mradius) radius=mradius;
  elem->width=elem->height=radius;

  /* move shape if necessary ... (untouched) */
  switch (horiz) {
    case ANCHOR_MIDDLE:
      elem->corner.x = center.x - elem->width/2; break;
    case ANCHOR_END:
      elem->corner.x = bottom_right.x - elem->width; break;
    default:
      break;
  }
  switch (vert) {
    case ANCHOR_MIDDLE:
      elem->corner.y = center.y - elem->height/2; break;
    case ANCHOR_END:
      elem->corner.y = bottom_right.y - elem->height; break;
    default:
      break;
  }

  p = elem->corner;
  p.x += elem->width / 2.0;
  p.y += elem->height / 2.0 - actor->text->height*actor->text->numlines/2 +
    actor->text->ascent;
  text_set_position(actor->text, &p);

  /* compute connection positions */
  c.x = elem->corner.x + elem->width / 2;
  c.y = elem->corner.y + elem->height / 2;
  dw = elem->width  / 2.0;
  dh = elem->height / 2.0;
  for (i = 0; i < NUM_CONNECTIONS-1; i++) {
    real theta = M_PI / 8.0 * i;
    /* TODO: Set up directions for autorouting */
    actor->connections[i].pos.x = c.x + dw * cos(theta);
    actor->connections[i].pos.y = c.y - dh * sin(theta);
  }
  actor->connections[16].pos.x = c.x;
  actor->connections[16].pos.y = c.y;

  extra->border_trans = ACTOR_BORDER_WIDTH / 2.0;
  element_update_boundingbox(elem);

  obj->position = elem->corner;

  element_update_handles(elem);
}

/* creation stuff */
static DiaObject 
*actor_create(Point *startpoint,
	   void *user_data,
	   Handle **handle1,
	   Handle **handle2)
{
  Actor *actor;
  Element *elem;
  DiaObject *obj;
  Point p;
  int i;
  DiaFont *font = NULL;

  actor = g_malloc0(sizeof(Actor));
  elem = &actor->element;
  obj = &elem->object;

  obj->type = &istar_actor_type;

  obj->ops = &actor_ops;

  elem->corner = *startpoint;
  elem->width = ACTOR_RADIUS;
  elem->height = ACTOR_RADIUS;

  font = dia_font_new_from_style( DIA_FONT_SANS, ACTOR_FONT);
  p = *startpoint;
  p.x += elem->width / 2.0;
  p.y += elem->height / 2.0 + ACTOR_FONT / 2;
  actor->text = new_text("", font, ACTOR_FONT, &p, &ACTOR_FG_COLOR, ALIGN_CENTER);
  text_get_attributes(actor->text,&actor->attrs);
  dia_font_unref(font);

  element_init(elem, 8, NUM_CONNECTIONS);

  for (i=0;i<NUM_CONNECTIONS;i++) {
    obj->connections[i] = &actor->connections[i];
    actor->connections[i].object = obj;
    actor->connections[i].connected = NULL;
  }
  actor->connections[16].flags = CP_FLAGS_MAIN;

  /* init */
  switch (GPOINTER_TO_INT(user_data)) {
    case 1: actor->type=ACTOR_UNSPECIFIED; break;
    case 2:  actor->type=ACTOR_AGENT; break;
    case 3:  actor->type=ACTOR_POSITION; break;
    case 4:  actor->type=ACTOR_ROLE; break;
    default: actor->type=ACTOR_UNSPECIFIED; break;
  }

  if (GPOINTER_TO_INT(user_data)!=0) actor->init=-1; else actor->init=0;

  actor_update_data(actor, ANCHOR_MIDDLE, ANCHOR_MIDDLE);

  *handle1 = NULL;
  *handle2 = obj->handles[7];

  return &actor->element.object;
}

static void
actor_destroy(Actor *actor)
{
  text_destroy(actor->text);

  element_destroy(&actor->element);
}

static DiaObject *
actor_load(ObjectNode obj_node, int version, const char *filename)
{
  return object_load_using_properties(&istar_actor_type,
                                      obj_node,version,filename);
}



