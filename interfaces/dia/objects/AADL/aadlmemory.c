/* AADL plugin for DIA
*
* Copyright (C) 2005 Laboratoire d'Informatique de Paris 6
* Author: Pierre Duquesne
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


#include "aadl.h"
#include "pixmaps/aadlmemory.xpm"
/***********************************************
 **               AADL MEMORY             **
 ***********************************************/

static void aadlmemory_draw_borders(Aadlbox *aadlbox, DiaRenderer *renderer)
{
  DiaRendererClass *renderer_ops = DIA_RENDERER_GET_CLASS (renderer);
  Element *elem;
  real x, y, w, h;
  BezPoint bez[5];

  assert(aadlbox != NULL);
  assert(renderer != NULL);

  elem = &aadlbox->element;

  x = elem->corner.x;
  y = elem->corner.y;
  w = elem->width;
  h = elem->height;

  bez[0].type = BEZ_MOVE_TO;
  bez[1].type = BEZ_CURVE_TO;
  bez[2].type = BEZ_LINE_TO;
  bez[3].type = BEZ_CURVE_TO;
  bez[4].type = BEZ_LINE_TO;

  bez[0].p1.x = x;
  bez[0].p1.y = y + h*AADL_MEMORY_FACTOR;

  bez[1].p1.x = x;
  bez[1].p1.y = y;
  bez[1].p2.x = x+w;
  bez[1].p2.y = y;
  bez[1].p3.x = x + w;
  bez[1].p3.y = y + h*AADL_MEMORY_FACTOR;

  bez[2].p1.x = x + w;
  bez[2].p1.y = y + h - h*AADL_MEMORY_FACTOR;

  bez[3].p1.x = x + w;
  bez[3].p1.y = y + h;
  bez[3].p2.x = x;
  bez[3].p2.y = y + h;
  bez[3].p3.x = x;
  bez[3].p3.y = y + h - h*AADL_MEMORY_FACTOR;

  point_copy(&bez[4].p1, &bez[0].p1);

  renderer_ops->set_fillstyle(renderer, FILLSTYLE_SOLID);
  renderer_ops->set_linewidth(renderer, AADLBOX_BORDERWIDTH);
  renderer_ops->set_linestyle(renderer, LINESTYLE_SOLID);

  renderer_ops->fill_bezier(renderer, bez, 5, &aadlbox->fill_color);
  renderer_ops->draw_bezier(renderer, bez, 5, &aadlbox->line_color);

  bez[1].p1.x = x;
  bez[1].p1.y = y + 2*h*AADL_MEMORY_FACTOR;
  bez[1].p2.x = x+w;
  bez[1].p2.y = y+ 2*h*AADL_MEMORY_FACTOR;
  bez[1].p3.x = x + w;
  bez[1].p3.y = y + h*AADL_MEMORY_FACTOR;

  renderer_ops->draw_bezier(renderer, bez, 3, &aadlbox->line_color);

}

static Aadlbox_specific aadlmemory_specific =
{
  (AadlProjectionFunc) aadldata_project_point_on_nearest_border,
  (AadlTextPosFunc)    aadlmemory_text_position,
  (AadlSizeFunc) aadlmemory_minsize
};



static void aadlmemory_draw(Aadlbox *aadlbox, DiaRenderer *renderer)
{
  aadlmemory_draw_borders(aadlbox, renderer);
  aadlbox_draw(aadlbox, renderer);
}

ObjectTypeOps aadlmemory_type_ops;

DiaObjectType aadlmemory_type =
{
  "AADL - Memory",           /* name */
  0,                      /* version */
  (char **) aadlmemory_xpm,  /* pixmap */

  &aadlmemory_type_ops,       /* ops */
  NULL,
  &aadlmemory_specific      /* user data */
};


static ObjectOps aadlmemory_ops =
{
  (DestroyFunc)         aadlbox_destroy,
  (DrawFunc)            aadlmemory_draw,              /* redefined */
  (DistanceFunc)        aadlbox_distance_from,
  (SelectFunc)          aadlbox_select,
  (CopyFunc)            aadlbox_copy,
  (MoveFunc)            aadlbox_move,
  (MoveHandleFunc)      aadlbox_move_handle,
  (GetPropertiesFunc)   object_create_props_dialog,
  (ApplyPropertiesDialogFunc) object_apply_props_from_dialog,
  (ObjectMenuFunc)      aadlbox_get_object_menu,
  (DescribePropsFunc)   aadlbox_describe_props,
  (GetPropsFunc)        aadlbox_get_props,
  (SetPropsFunc)        aadlbox_set_props,
  (TextEditFunc) 0,
  (ApplyPropertiesListFunc) object_apply_props,
};



static DiaObject *aadlmemory_create(Point *startpoint, void *user_data, Handle **handle1, Handle **handle2)
{
  DiaObject *obj = aadlbox_create(startpoint, user_data, handle1, handle2);

  obj->type = &aadlmemory_type;
  obj->ops  = &aadlmemory_ops;
      
  return obj;
}

static DiaObject *aadlmemory_load(ObjectNode obj_node, int version, const char *filename)
{
  DiaObject *obj;
  Point startpoint = {0.0,0.0};
  Handle *handle1,*handle2;
  
  obj = aadlmemory_create(&startpoint,&aadlmemory_specific, &handle1,&handle2);
  aadlbox_load(obj_node, version, filename, (Aadlbox *) obj);
  return obj;
}


ObjectTypeOps aadlmemory_type_ops =
{
  (CreateFunc) aadlmemory_create,
  (LoadFunc)   aadlmemory_load,/*using_properties*/     /* load */
  (SaveFunc)   aadlbox_save,      /* save */
  (GetDefaultsFunc)   NULL,
  (ApplyDefaultsFunc) NULL
};
