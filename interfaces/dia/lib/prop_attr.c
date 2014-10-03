/* Dia -- a diagram creation/manipulation program -*- c -*-
 * Copyright (C) 1998 Alexander Larsson
 *
 * Property system for dia objects/shapes.
 * Copyright (C) 2000 James Henstridge
 * Copyright (C) 2001 Cyrille Chepelov
 * Major restructuration done in August 2001 by C. Chépélov
 *
 * Property types for "attribute" types (line style, arrow, colour, font)
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

#include <gtk/gtk.h>
#include "dia_xml.h"
#include "widgets.h"
#include "properties.h"
#include "propinternals.h"
#include "diaarrowchooser.h"

/***************************/
/* The LINESTYLE property type.  */
/***************************/

static LinestyleProperty *
linestyleprop_new(const PropDescription *pdesc, 
                  PropDescToPropPredicate reason)
{
  LinestyleProperty *prop = g_new0(LinestyleProperty,1);
  initialize_property(&prop->common,pdesc,reason);
  prop->style = LINESTYLE_SOLID;
  prop->dash = 0.0;
  return prop;
}

static LinestyleProperty *
linestyleprop_copy(LinestyleProperty *src) 
{
  LinestyleProperty *prop = 
    (LinestyleProperty *)src->common.ops->new_prop(src->common.descr,
                                                    src->common.reason);
  copy_init_property(&prop->common,&src->common);
  prop->style = src->style;
  prop->dash = src->dash;
  return prop;
}

static WIDGET *
linestyleprop_get_widget(LinestyleProperty *prop, PropDialog *dialog)
{ 
  GtkWidget *ret = dia_line_style_selector_new();
  prophandler_connect(&prop->common, G_OBJECT(ret), "value-changed");
  return ret;
}

static void 
linestyleprop_reset_widget(LinestyleProperty *prop, WIDGET *widget)
{
  dia_line_style_selector_set_linestyle(DIALINESTYLESELECTOR(widget),
                                        prop->style,
                                        prop->dash);
}

static void 
linestyleprop_set_from_widget(LinestyleProperty *prop, WIDGET *widget) 
{
  dia_line_style_selector_get_linestyle(DIALINESTYLESELECTOR(widget),
                                        &prop->style,
                                        &prop->dash);
}

static void 
linestyleprop_load(LinestyleProperty *prop, AttributeNode attr, DataNode data)
{
  prop->style = data_enum(data);
  prop->dash = 1.0;
  /* don't bother checking dash length if we have a solid line. */
  if (prop->style != LINESTYLE_SOLID) {
    data = data_next(data);
    if (data)
      prop->dash = data_real(data);
    else {
      ObjectNode obj_node = attr->parent;
      /* backward compatibility */
      if ((attr = object_find_attribute(obj_node, "dashlength")) &&
          (data = attribute_first_data(attr)))
        prop->dash = data_real(data);
    }
  }
}

static void 
linestyleprop_save(LinestyleProperty *prop, AttributeNode attr) 
{
  data_add_enum(attr, prop->style);
  data_add_real(attr, prop->dash);
  /* for compatibility.  It makes more sense to link the two together */
  /* 
     data_add_real(new_attribute(attr->parent, "dashlength"),
     prop->dash);

     To hell with compatibility with very old dias ! -- CC */
}

static void 
linestyleprop_get_from_offset(LinestyleProperty *prop,
                              void *base, guint offset, guint offset2) 
{
  prop->style = struct_member(base, offset, LineStyle);
  prop->dash = struct_member(base, offset2, real);
}

static void 
linestyleprop_set_from_offset(LinestyleProperty *prop,
                              void *base, guint offset, guint offset2)
{
  struct_member(base, offset, LineStyle) = prop->style;
  struct_member(base, offset2, real) = prop->dash;
}

static const PropertyOps linestyleprop_ops = {
  (PropertyType_New) linestyleprop_new,
  (PropertyType_Free) noopprop_free,
  (PropertyType_Copy) linestyleprop_copy,
  (PropertyType_Load) linestyleprop_load,
  (PropertyType_Save) linestyleprop_save,
  (PropertyType_GetWidget) linestyleprop_get_widget,
  (PropertyType_ResetWidget) linestyleprop_reset_widget,
  (PropertyType_SetFromWidget) linestyleprop_set_from_widget,

  (PropertyType_CanMerge) noopprop_can_merge,
  (PropertyType_GetFromOffset) linestyleprop_get_from_offset,
  (PropertyType_SetFromOffset) linestyleprop_set_from_offset
};

/***************************/
/* The ARROW property type.  */
/***************************/

static ArrowProperty *
arrowprop_new(const PropDescription *pdesc, PropDescToPropPredicate reason)
{
  ArrowProperty *prop = g_new0(ArrowProperty,1);
  initialize_property(&prop->common,pdesc,reason);
  prop->arrow_data.type = ARROW_NONE;
  prop->arrow_data.length = 0.0;
  prop->arrow_data.width = 0.0;
  return prop;
}

static ArrowProperty *
arrowprop_copy(ArrowProperty *src) 
{
  ArrowProperty *prop = 
    (ArrowProperty *)src->common.ops->new_prop(src->common.descr,
                                                src->common.reason);
  copy_init_property(&prop->common,&src->common);
  prop->arrow_data = src->arrow_data;
  return prop;
}

static WIDGET *
arrowprop_get_widget(ArrowProperty *prop, PropDialog *dialog)
{ 
  GtkWidget *ret = dia_arrow_selector_new();
  prophandler_connect(&prop->common, G_OBJECT(ret), "value-changed");
  return ret;
}

static void 
arrowprop_reset_widget(ArrowProperty *prop, WIDGET *widget)
{
  dia_arrow_selector_set_arrow(DIA_ARROW_SELECTOR(widget),
                               prop->arrow_data);
}

static void 
arrowprop_set_from_widget(ArrowProperty *prop, WIDGET *widget) 
{
  prop->arrow_data = dia_arrow_selector_get_arrow(DIA_ARROW_SELECTOR(widget));
}

static void 
arrowprop_load(ArrowProperty *prop, AttributeNode attr, DataNode data)
{
  /* Maybe it would be better to store arrows as a single composite
   * attribute rather than three seperate attributes. This would break
   * binary compatibility though.*/
  prop->arrow_data.type = data_enum(data);
  prop->arrow_data.length = DEFAULT_ARROW_SIZE;
  prop->arrow_data.width = DEFAULT_ARROW_SIZE;
  if (prop->arrow_data.type != ARROW_NONE) {
    ObjectNode obj_node = attr->parent;
    gchar *str = g_strconcat(prop->common.name, "_length", NULL);
    if ((attr = object_find_attribute(obj_node, str)) &&
        (data = attribute_first_data(attr)))
      prop->arrow_data.length = data_real(data);
    g_free(str);
    str = g_strconcat(prop->common.name, "_width", NULL);
    if ((attr = object_find_attribute(obj_node, str)) &&
        (data = attribute_first_data(attr)))
      prop->arrow_data.width = data_real(data);
    g_free(str);
  }
}

static void 
arrowprop_save(ArrowProperty *prop, AttributeNode attr) 
{
  data_add_enum(attr, prop->arrow_data.type);
  if (prop->arrow_data.type != ARROW_NONE) {
    ObjectNode obj_node = attr->parent;
    gchar *str = g_strconcat(prop->common.name, "_length", NULL);
    attr = new_attribute(obj_node, str);
    g_free(str);
    data_add_real(attr, prop->arrow_data.length);
    str = g_strconcat(prop->common.name, "_width", NULL);
    attr = new_attribute(obj_node, str);
    g_free(str);
    data_add_real(attr, prop->arrow_data.width);
  }
}

static void 
arrowprop_get_from_offset(ArrowProperty *prop,
                          void *base, guint offset, guint offset2) 
{
  prop->arrow_data = struct_member(base,offset,Arrow);
}

static void 
arrowprop_set_from_offset(ArrowProperty *prop,
                          void *base, guint offset, guint offset2)
{
  struct_member(base,offset,Arrow) = prop->arrow_data;
}

static const PropertyOps arrowprop_ops = {
  (PropertyType_New) arrowprop_new,
  (PropertyType_Free) noopprop_free,
  (PropertyType_Copy) arrowprop_copy,
  (PropertyType_Load) arrowprop_load,
  (PropertyType_Save) arrowprop_save,
  (PropertyType_GetWidget) arrowprop_get_widget,
  (PropertyType_ResetWidget) arrowprop_reset_widget,
  (PropertyType_SetFromWidget) arrowprop_set_from_widget,

  (PropertyType_CanMerge) noopprop_can_merge,
  (PropertyType_GetFromOffset) arrowprop_get_from_offset,
  (PropertyType_SetFromOffset) arrowprop_set_from_offset
};

/***************************/
/* The COLOR property type.  */
/***************************/

static ColorProperty *
colorprop_new(const PropDescription *pdesc, PropDescToPropPredicate reason)
{
  ColorProperty *prop = g_new0(ColorProperty,1);
  initialize_property(&prop->common,pdesc,reason);
  prop->color_data.red = 0.0;
  prop->color_data.green = 0.0;
  prop->color_data.blue = 1.0;
  return prop;
}

static ColorProperty *
colorprop_copy(ColorProperty *src) 
{
  ColorProperty *prop = 
    (ColorProperty *)src->common.ops->new_prop(src->common.descr,
                                                src->common.reason);
  copy_init_property(&prop->common,&src->common);
  prop->color_data = src->color_data;
  return prop;
}

static WIDGET *
colorprop_get_widget(ColorProperty *prop, PropDialog *dialog)
{ 
  GtkWidget *ret = dia_color_selector_new();
  prophandler_connect(&prop->common, G_OBJECT(ret), "value-changed");
  return ret;
}

static void 
colorprop_reset_widget(ColorProperty *prop, WIDGET *widget)
{
  dia_color_selector_set_color(widget,
                               &prop->color_data);
}

static void 
colorprop_set_from_widget(ColorProperty *prop, WIDGET *widget) 
{
  dia_color_selector_get_color(widget,
                               &prop->color_data);
}

static void 
colorprop_load(ColorProperty *prop, AttributeNode attr, DataNode data)
{
  data_color(data,&prop->color_data);
}

static void 
colorprop_save(ColorProperty *prop, AttributeNode attr) 
{
  data_add_color(attr,&prop->color_data);
}

static void 
colorprop_get_from_offset(ColorProperty *prop,
                          void *base, guint offset, guint offset2) 
{
  prop->color_data = struct_member(base,offset,Color);
}

static void 
colorprop_set_from_offset(ColorProperty *prop,
                          void *base, guint offset, guint offset2)
{
  struct_member(base,offset,Color) = prop->color_data;
}

static const PropertyOps colorprop_ops = {
  (PropertyType_New) colorprop_new,
  (PropertyType_Free) noopprop_free,
  (PropertyType_Copy) colorprop_copy,
  (PropertyType_Load) colorprop_load,
  (PropertyType_Save) colorprop_save,
  (PropertyType_GetWidget) colorprop_get_widget,
  (PropertyType_ResetWidget) colorprop_reset_widget,
  (PropertyType_SetFromWidget) colorprop_set_from_widget,

  (PropertyType_CanMerge) noopprop_can_merge,
  (PropertyType_GetFromOffset) colorprop_get_from_offset,
  (PropertyType_SetFromOffset) colorprop_set_from_offset
};


/***************************/
/* The FONT property type. */
/***************************/

static FontProperty *
fontprop_new(const PropDescription *pdesc, PropDescToPropPredicate reason)
{
  FontProperty *prop = g_new0(FontProperty,1);
  initialize_property(&prop->common,pdesc,reason);
  prop->font_data = NULL;
  return prop;
}

static void
fontprop_free(FontProperty *prop)
{
  if (prop->font_data)
    dia_font_unref(prop->font_data);
  g_free(prop);
}

static FontProperty *
fontprop_copy(FontProperty *src) 
{
  FontProperty *prop = 
    (FontProperty *)src->common.ops->new_prop(src->common.descr,
                                               src->common.reason);
  copy_init_property(&prop->common,&src->common);

  if (prop->font_data)
    dia_font_unref(prop->font_data);
  prop->font_data = dia_font_ref(src->font_data);

  return prop;
}

static WIDGET *
fontprop_get_widget(FontProperty *prop, PropDialog *dialog)
{ 
  GtkWidget *ret = dia_font_selector_new();
  prophandler_connect(&prop->common, G_OBJECT(ret), "value-changed");
  return ret;
}

static void 
fontprop_reset_widget(FontProperty *prop, WIDGET *widget)
{
  dia_font_selector_set_font(DIAFONTSELECTOR(widget),
                             prop->font_data);
}

static void 
fontprop_set_from_widget(FontProperty *prop, WIDGET *widget) 
{
  prop->font_data = dia_font_selector_get_font(DIAFONTSELECTOR(widget));
}

static void 
fontprop_load(FontProperty *prop, AttributeNode attr, DataNode data)
{
  if (prop->font_data)
    dia_font_unref(prop->font_data);
  prop->font_data = data_font(data);
}

static void 
fontprop_save(FontProperty *prop, AttributeNode attr) 
{
  data_add_font(attr,prop->font_data);
}

static void 
fontprop_get_from_offset(FontProperty *prop,
                         void *base, guint offset, guint offset2) 
{
  if (prop->font_data)
    dia_font_unref(prop->font_data);    
  prop->font_data = dia_font_ref(struct_member(base,offset,DiaFont *));
}

static void 
fontprop_set_from_offset(FontProperty *prop,
                         void *base, guint offset, guint offset2)
{
  if (prop->font_data) {
    if (struct_member(base,offset,DiaFont *))
      dia_font_unref(struct_member(base,offset,DiaFont *));   
    struct_member(base,offset,DiaFont *) = dia_font_ref(prop->font_data);
  }
}

static const PropertyOps fontprop_ops = {
  (PropertyType_New) fontprop_new,
  (PropertyType_Free) fontprop_free,
  (PropertyType_Copy) fontprop_copy,
  (PropertyType_Load) fontprop_load,
  (PropertyType_Save) fontprop_save,
  (PropertyType_GetWidget) fontprop_get_widget,
  (PropertyType_ResetWidget) fontprop_reset_widget,
  (PropertyType_SetFromWidget) fontprop_set_from_widget,

  (PropertyType_CanMerge) noopprop_can_merge,
  (PropertyType_GetFromOffset) fontprop_get_from_offset,
  (PropertyType_SetFromOffset) fontprop_set_from_offset
};

/* ************************************************************** */ 

void 
prop_attr_register(void)
{
  prop_type_register(PROP_TYPE_LINESTYLE,&linestyleprop_ops);
  prop_type_register(PROP_TYPE_ARROW,&arrowprop_ops);
  prop_type_register(PROP_TYPE_COLOUR,&colorprop_ops);
  prop_type_register(PROP_TYPE_FONT,&fontprop_ops);
}
