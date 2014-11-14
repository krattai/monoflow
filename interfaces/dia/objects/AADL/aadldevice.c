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
#include "pixmaps/aadldevice.xpm"
/***********************************************
 **                 AADL DEVICE                 **
 ***********************************************/

#define AADL_DEVICE_DEPTH 0.5

static void aadldevice_draw_borders(Aadlbox *aadlbox, DiaRenderer *renderer)
{
  DiaRendererClass *renderer_ops = DIA_RENDERER_GET_CLASS (renderer);
  Element *elem;
  real x, y, w, h;
  Point points[4];

  assert(aadlbox != NULL);
  assert(renderer != NULL);

  elem = &aadlbox->element;

  x = elem->corner.x;
  y = elem->corner.y;
  w = elem->width;
  h = elem->height;

  renderer_ops->set_fillstyle(renderer, FILLSTYLE_SOLID);
  renderer_ops->set_linewidth(renderer, AADLBOX_BORDERWIDTH);
  renderer_ops->set_linestyle(renderer, LINESTYLE_SOLID);

  points[0].x = x;     points[0].y = y;
  points[1].x = x + w; points[1].y = y + h;

  renderer_ops->fill_rect(renderer, points, points + 1, &aadlbox->fill_color);
  renderer_ops->draw_rect(renderer, points, points + 1, &aadlbox->line_color);

  points[1].x = x - AADL_DEVICE_DEPTH;
  points[1].y = y - AADL_DEVICE_DEPTH;

  points[2].x = x + w + AADL_DEVICE_DEPTH;
  points[2].y = y - AADL_DEVICE_DEPTH;

  points[3].x = x + w, points[3].y = y;

  renderer_ops->fill_polygon(renderer, points, 4, &aadlbox->fill_color);
  renderer_ops->draw_polygon(renderer, points, 4, &aadlbox->line_color);

  points[0].x = points[3].x, points[0].y = points[3].y;
  points[1].x = points[0].x + AADL_DEVICE_DEPTH;
  points[1].y = points[0].y - AADL_DEVICE_DEPTH;

  points[2].x = points[0].x + AADL_DEVICE_DEPTH;
  points[2].y = points[0].y + AADL_DEVICE_DEPTH + h;

  points[3].x = points[0].x, points[3].y = points[0].y + h;

  renderer_ops->fill_polygon(renderer, points, 4, &aadlbox->fill_color);
  renderer_ops->draw_polygon(renderer, points, 4, &aadlbox->line_color);

  points[0].x = x + w;
  points[0].y = y + h;

  points[1].x = x + w + AADL_DEVICE_DEPTH;
  points[1].y = y + h + AADL_DEVICE_DEPTH;

  points[2].x = x - AADL_DEVICE_DEPTH;
  points[2].y = y + h + AADL_DEVICE_DEPTH;

  points[3].x = x;
  points[3].y = y + h;

  renderer_ops->fill_polygon(renderer, points, 4, &aadlbox->fill_color);
  renderer_ops->draw_polygon(renderer, points, 4, &aadlbox->line_color);

  points[0].x = x;
  points[0].y = y;

  points[1].x = x - AADL_DEVICE_DEPTH;
  points[1].y = y - AADL_DEVICE_DEPTH;

  renderer_ops->fill_polygon(renderer, points, 4, &aadlbox->fill_color);
  renderer_ops->draw_polygon(renderer, points, 4, &aadlbox->line_color);

}

static Aadlbox_specific aadldevice_specific =
{
  (AadlProjectionFunc) aadldata_project_point_on_nearest_border,
  (AadlTextPosFunc)    aadldata_text_position,
  (AadlSizeFunc) aadldata_minsize
};



static void aadldevice_draw(Aadlbox *aadlbox, DiaRenderer *renderer)
{
  aadldevice_draw_borders(aadlbox, renderer);
  aadlbox_draw(aadlbox, renderer);
}

ObjectTypeOps aadldevice_type_ops;

DiaObjectType aadldevice_type =
{
  "AADL - Device",           /* name */
  0,                      /* version */
  (char **) aadldevice_xpm,  /* pixmap */

  &aadldevice_type_ops,       /* ops */
  NULL,
  &aadldevice_specific      /* user data */
};


static ObjectOps aadldevice_ops =
{
  (DestroyFunc)         aadlbox_destroy,
  (DrawFunc)            aadldevice_draw,              /* redefined */
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



static DiaObject *aadldevice_create(Point *startpoint, void *user_data, Handle **handle1, Handle **handle2)
{
  DiaObject *obj = aadlbox_create(startpoint, user_data, handle1, handle2);

  obj->type = &aadldevice_type;
  obj->ops  = &aadldevice_ops;
      
  return obj;
}

static DiaObject *aadldevice_load(ObjectNode obj_node, int version, const char *filename)
{
  DiaObject *obj;
  Point startpoint = {0.0,0.0};
  Handle *handle1,*handle2;
  
  obj = aadldevice_create(&startpoint,&aadldevice_specific, &handle1,&handle2);
  aadlbox_load(obj_node, version, filename, (Aadlbox *) obj);
  return obj;
}


ObjectTypeOps aadldevice_type_ops =
{
  (CreateFunc) aadldevice_create,
  (LoadFunc)   aadldevice_load,/*using_properties*/     /* load */
  (SaveFunc)   aadlbox_save,      /* save */
  (GetDefaultsFunc)   NULL,
  (ApplyDefaultsFunc) NULL
};
