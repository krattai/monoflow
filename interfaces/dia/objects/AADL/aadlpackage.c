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
#include "pixmaps/aadlpackage.xpm"

/* To have a really useful package element, we should make it a class of its 
   own, not inherited from aadlbox, with 2 special handles -- that could be 
   moved -- to draw the public/private separation */

/***********************************************
 **                 AADL PACKAGE              **
 ***********************************************/


static void aadlpackage_draw_borders(Aadlbox *aadlbox, DiaRenderer *renderer)
{
  DiaRendererClass *renderer_ops = DIA_RENDERER_GET_CLASS (renderer);
  Element *elem;
  real x, y, w, h;
  Point points[9];

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

  points[0].x = x;                 points[0].y = y;
  points[1].x = x + 0.03 * w ;     points[1].y = y;
  points[2].x = x + 0.08 * w ;     points[2].y = y -  AADL_PORT_MAX_OUT;
  points[3].x = x + 0.40 * w ;     points[3].y = y -  AADL_PORT_MAX_OUT;
  points[4].x = x + 0.45 * w ;     points[4].y = y;
  points[5].x = x + w - 0.05 * w;  points[5].y = y;
  points[6].x = x + w;             points[6].y = y + 0.05 * h;
  points[7].x = x + w;             points[7].y = y + h;
  points[8].x = x ;                points[8].y = y + h;

  renderer_ops->fill_polygon(renderer, points, 9, &aadlbox->fill_color);
  renderer_ops->draw_polygon(renderer, points, 9, &aadlbox->line_color);
}


static void aadlpackage_draw(Aadlbox *aadlbox, DiaRenderer *renderer)
{
  aadlpackage_draw_borders(aadlbox, renderer);
  aadlbox_draw(aadlbox, renderer);
}


static Aadlbox_specific aadlpackage_specific =
{
  (AadlProjectionFunc) aadldata_project_point_on_nearest_border,
  (AadlTextPosFunc)    aadldata_text_position,
  (AadlSizeFunc) aadldata_minsize
};

ObjectTypeOps aadlpackage_type_ops;

DiaObjectType aadlpackage_type =
{
  "AADL - Package",           /* name */
  0,                      /* version */
  (char **) aadlpackage_xpm,  /* pixmap */

  &aadlpackage_type_ops,       /* ops */
  NULL,
  &aadlpackage_specific      /* user data */
};


static ObjectOps aadlpackage_ops =
{
  (DestroyFunc)         aadlbox_destroy,
  (DrawFunc)            aadlpackage_draw,              /* redefined */
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



static DiaObject *aadlpackage_create(Point *startpoint, void *user_data, Handle **handle1, Handle **handle2)
{
  DiaObject *obj = aadlbox_create(startpoint, user_data, handle1, handle2);

  obj->type = &aadlpackage_type;
  obj->ops  = &aadlpackage_ops;
      
  return obj;
}

static DiaObject *aadlpackage_load(ObjectNode obj_node, int version, const char *filename)
{
  DiaObject *obj;
  Point startpoint = {0.0,0.0};
  Handle *handle1,*handle2;
  
  obj = aadlpackage_create(&startpoint,&aadlpackage_specific, &handle1,&handle2);
  aadlbox_load(obj_node, version, filename, (Aadlbox *) obj);
  
  return obj;
}


ObjectTypeOps aadlpackage_type_ops =
{
  (CreateFunc) aadlpackage_create,
  (LoadFunc)   aadlpackage_load,      /* load */
  (SaveFunc)   aadlbox_save,          /* save */
  (GetDefaultsFunc)   NULL,
  (ApplyDefaultsFunc) NULL
};
