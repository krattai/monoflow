/* Dia -- an diagram creation/manipulation program
 * Copyright (C) 1998 Alexander Larsson
 *
 * dia-render-script-import.c - plugin for dia
 * Copyright (C) 2009, Hans Breuer, <Hans@Breuer.Org>
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

/*!  \file dia-render-script-import.c import of dia-render-script either to 
 * diagram with objects or maybe as one object */
#include <config.h>

#include "geometry.h"
#include "color.h"
#include "diagramdata.h"

#include <libxml/tree.h>

static real
_parse_real (xmlNodePtr node, const char *attrib)
{
  xmlChar *str = xmlGetProp(node, (const xmlChar *)attrib);
  real val = 0;
  if (str) {
    val = g_strtod ((gchar *)str, NULL);
    xmlFree(str);
  }
  return val;
}
static Point *
_parse_point (xmlNodePtr node, const char *attrib)
{
  xmlChar *str = xmlGetProp(node, (const xmlChar *)attrib);
  Point *pt = g_new0(Point, 1);
  if (str) {
    gchar *ep = NULL;
    pt->x = g_strtod ((gchar *)str, &ep);
    if (ep) {
      ++ep;
      pt->y = g_strtod (ep, NULL);
    }
    xmlFree(str);
  }
  return pt;
}
static GArray *
_parse_points (xmlNodePtr node, const char *attrib)
{
  xmlChar *str = xmlGetProp(node, (const xmlChar *)attrib);
  GArray *arr = NULL;
  
  if (str) {
    GArray *arr = g_array_new(FALSE, TRUE, sizeof(Point));
    gint i;
    gchar **split = g_strsplit ((gchar *)str, " ", -1);
    gchar *val, *ep = NULL;
    for (i = 0, val = split[i]; split[i] != NULL; ++i)
      /* count them */;
    g_array_set_size (arr, i);
    for (i = 0, val = split[i]; split[i] != 0; ++i) {
      Point *pt = &g_array_index(arr, Point, i);
      
      pt->x = g_strtod (val, &ep);
      pt->y = ep ? ++ep, g_strtod (ep, &ep) : 0;
    }
    g_strfreev(split);
    xmlFree(str);
  }
  return arr;
}
static GArray *
_parse_bezpoints (xmlNodePtr node, const char *attrib)
{
  xmlChar *str = xmlGetProp(node, (const xmlChar *)attrib);
  GArray *arr = NULL;
  
  if (str) {
    GArray *arr = g_array_new(FALSE, TRUE, sizeof(BezPoint));
    gint i;
    gchar **split = g_strsplit ((gchar *)str, " ", -1);
    gchar *val, *ep = NULL;
    for (i = 0, val = split[i]; split[i] != NULL; ++i)
      /* count them */;
    g_array_set_size (arr, i);
    for (i = 0, val = split[i]; split[i] != 0; ++i) {
      BezPoint *pt = &g_array_index(arr, BezPoint, i);
      pt->type = val[0] == 'M' ? BEZ_MOVE_TO : (val[0] == 'L' ? BEZ_LINE_TO : BEZ_CURVE_TO);
      ep = (gchar *)str + 1;
      
      pt->p1.x = ep ? g_strtod (ep, &ep) : 0;
      pt->p1.y = ep ? ++ep, g_strtod (ep, &ep) : 0;
      pt->p2.x = ep ? ++ep, g_strtod (ep, &ep) : 0;
      pt->p2.y = ep ? ++ep, g_strtod (ep, &ep) : 0;
      pt->p3.x = ep ? ++ep, g_strtod (ep, &ep) : 0;
      pt->p3.y = ep ? ++ep, g_strtod (ep, &ep) : 0;
    }    
    g_strfreev(split);
    xmlFree(str);
  }
  return arr;
}
static Color *
_parse_color (xmlNodePtr node, const char *attrib)
{
  xmlChar *str = xmlGetProp(node, (const xmlChar *)attrib);
  Color *val = NULL;
  
  if (str) {
    PangoColor color;
    if (!pango_color_parse (&color, (gchar *)str)) {
      val = g_new (Color, 1);
      val->red = color.red / 65535.0; 
      val->green = color.green / 65535.0; 
      val->blue = color.blue / 65535.0;
    }
    xmlFree(str);
  }
  return val;
}

typedef struct _RenderOp RenderOp;
struct _RenderOp {
  void (*render) (RenderOp *self, ...);
  void (*destroy)(RenderOp *self);
  void *params[6];
};

/*!
 * Fill a GList* with objects which is to be put in a
 * diagram or a group by the caller. 
 * Can be called recusively to allow groups in groups.
 */
static GList*
read_items (xmlNodePtr startnode)
{
  xmlNodePtr node;
  GList *items = NULL;

  for (node = startnode; node != NULL; node = node->next) {
    if (xmlIsBlankNode(node)) 
      continue;
    if (node->type != XML_ELEMENT_NODE)
      continue;
    
  }
  return items;
}

/* imports the given SVG file, returns TRUE if successful */
gboolean
import_drs (const gchar *filename, DiagramData *dia, void* user_data) 
{
  GList *item, *items;
  xmlDocPtr doc = xmlParseFile(filename);

  items = read_items (doc->xmlChildrenNode);
  for (item = items; item != NULL; item = g_list_next (item)) {
    DiaObject *obj = (DiaObject *)item->data;
    layer_add_object(dia->active_layer, obj);
  }
  g_list_free (items);
  xmlFreeDoc(doc);
  return TRUE;
}
