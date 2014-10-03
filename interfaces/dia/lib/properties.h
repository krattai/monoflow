/* Dia -- a diagram creation/manipulation program -*- c -*-
 * Copyright (C) 1998 Alexander Larsson
 *
 * Property system for dia objects/shapes.
 * Copyright (C) 2000 James Henstridge
 * Copyright (C) 2001 Cyrille Chepelov
 * Major restructuration done in August 2001 by C. Chepelov
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

#ifndef LIB_PROPERTIES_H
#define LIB_PROPERTIES_H

#ifndef WIDGET
#ifdef GTK_WIDGET
#define WIDGET GtkWidget
#else
#define WIDGET void
#endif
#endif 


#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#include <glib.h>
#include "diatypes.h"

#include "geometry.h"
#include "arrows.h"
#include "color.h"
#include "font.h"
#include "dia_xml.h"
#include "intl.h"
#include "textattr.h"

G_BEGIN_DECLS

typedef gboolean (*PropDescToPropPredicate)(const PropDescription *pdesc);

struct _PropWidgetAssoc {
  Property *prop;
  WIDGET *widget;
};

struct _PropDialog { /* This is to be treated as opaque ! */
  WIDGET *widget; /* widget of self */

  GPtrArray *props; /* of Property * */
  GArray *prop_widgets; /* of PropWidgetAssoc. This is a "flat" listing of 
                           properties and widgets (no nesting information is 
                           kept here) */
  GList *objects; /* a copy of the objects, to be used as scratch. */
  GList *copies;  /* The original objects. */

  GPtrArray *containers;
  WIDGET *lastcont;
  WIDGET *curtable;
  guint currow;
};

struct _PropEventData {
  PropDialog *dialog;
  guint my_index; /* in dialog->propwidgets */
  WIDGET *widget;
  Property *self; 
};

typedef gboolean (*PropEventHandler) (DiaObject *obj, Property *prop);

struct _PropEventHandlerChain {
  PropEventHandler handler;
  PropEventHandlerChain *chain;
};

/* PropertyType operations : */

/* allocate a new, empty property. */
typedef Property * (*PropertyType_New) (const PropDescription *pdesc,
                                    PropDescToPropPredicate reason);
/* free the property -- must skip NULL values. */
typedef void (* PropertyType_Free)(Property *prop);
/* Copy the data member of the property. -- must skip NULL values. */
typedef Property *(* PropertyType_Copy)(Property *src);
/* Create a widget capable of editing the property. Add it to the last container (or
   add/remove container levels). */
typedef WIDGET *(* PropertyType_GetWidget)(Property *prop,
                                       PropDialog *dialog);

/* Get the value of the property into the widget */
typedef void (* PropertyType_ResetWidget)(const Property *prop, WIDGET *widget);
/* Set the value of the property from the current value of the widget */
typedef void (* PropertyType_SetFromWidget)(Property *prop, WIDGET *widget);
/* load/save a property */
typedef void (*PropertyType_Load)(Property *prop, AttributeNode attr, DataNode data);
typedef void (*PropertyType_Save)(Property *prop, AttributeNode attr);

/* If a property descriptor can be merged with another 
   (DONT_MERGE has already been handled) */
typedef gboolean (*PropertyType_CanMerge)(const PropDescription *pd1,const PropDescription *pd2);
typedef void (*PropertyType_GetFromOffset)(const Property *prop,
                                         void *base, guint offset, guint offset2);
typedef void (*PropertyType_SetFromOffset)(Property *prop,
                                         void *base, guint offset, guint offset2);
typedef int (*PropertyType_GetDataSize)(Property *prop);


struct _PropertyOps {
  PropertyType_New  new_prop;
  PropertyType_Free free;
  PropertyType_Copy copy;
  PropertyType_Load load;
  PropertyType_Save save;
  PropertyType_GetWidget get_widget;
  PropertyType_ResetWidget reset_widget;
  PropertyType_SetFromWidget set_from_widget;

  PropertyType_CanMerge can_merge;
  
  PropertyType_GetFromOffset get_from_offset;
  PropertyType_SetFromOffset set_from_offset;
  PropertyType_GetDataSize get_data_size;
};

typedef const gchar *PropertyType;

/* Basic types (can be used as building blocks) : */
#define PROP_TYPE_INVALID "invalid"   /* InvalidProperty */
#define PROP_TYPE_NOOP "noop"         /* NoopProperty */
#define PROP_TYPE_UNIMPLEMENTED "unimplemented" /* UnimplementedProperty */

/* Integral types : */
#define PROP_TYPE_CHAR "char"             /* CharProperty */
#define PROP_TYPE_BOOL "bool"             /* BoolProperty */
#define PROP_TYPE_INT "int"               /* IntProperty */
#define PROP_TYPE_INTARRAY "intarray"     /* IntarrayProperty */
#define PROP_TYPE_ENUM "enum"             /* EnumProperty */
#define PROP_TYPE_ENUMARRAY "enumarray"   /* EnumarrayProperty */

/* Text types : */
#define PROP_TYPE_MULTISTRING "multistring"  /* StringProperty */
/* (same as STRING but with (gint)extra_data lines) */
#define PROP_TYPE_STRING "string"            /* StringProperty */
#define PROP_TYPE_FILE "file"                /* StringProperty */
#define PROP_TYPE_TEXT "text" /* can't be visible */ /* TextProperty */
#define PROP_TYPE_STRINGLIST "stringlist" /* can't be visible */ /* StringListProperty */

/* Geometric types : */
#define PROP_TYPE_REAL "real"                /* RealProperty */
#define PROP_TYPE_LENGTH "length"            /* LengthProperty */
#define PROP_TYPE_FONTSIZE "fontsize"        /* FontsizeProperty */
#define PROP_TYPE_POINT "point"              /* PointProperty */
#define PROP_TYPE_POINTARRAY "pointarray"    /* PointarrayProperty */
#define PROP_TYPE_BEZPOINT "bezpoint"        /* BezPointProperty */
#define PROP_TYPE_BEZPOINTARRAY "bezpointarray" /* BezPointarrayProperty */
#define PROP_TYPE_RECT "rect"                /* RectProperty */
#define PROP_TYPE_ENDPOINTS "endpoints"      /* EndpointsProperty */
#define PROP_TYPE_CONNPOINT_LINE "connpoint_line"  /* Connpoint_LineProperty */

/* Attribute types : */
#define PROP_TYPE_LINESTYLE "linestyle"    /* LinestyleProperty */
#define PROP_TYPE_ARROW "arrow"            /* ArrowProperty */
#define PROP_TYPE_COLOUR "colour"          /* ColorProperty */
#define PROP_TYPE_FONT "font"              /* FontProperty */

/* Widget types : */
#define PROP_TYPE_STATIC "static"          /* StaticProperty */
/* (tooltip is used as a (potentially big) static label) */
#define PROP_TYPE_BUTTON "button"          /* ButtonProperty */
/* (tooltip is the button's label. Put an empty description). */
#define PROP_TYPE_NOTEBOOK_BEGIN "nb_begin" /* NotebookProperty */
#define PROP_TYPE_NOTEBOOK_END "nb_end"     /* NotebookProperty */
#define PROP_TYPE_NOTEBOOK_PAGE "nb_page"   /* NotebookProperty */
#define PROP_TYPE_MULTICOL_BEGIN "mc_begin" /* MulticolProperty */
#define PROP_TYPE_MULTICOL_END "mc_end"     /* MulticolProperty */
#define PROP_TYPE_MULTICOL_COLUMN "mc_col"  /* MulticolProperty */
#define PROP_TYPE_FRAME_BEGIN "f_begin" /* FrameProperty */
#define PROP_TYPE_FRAME_END "f_end"     /* FrameProperty */
#define PROP_TYPE_LIST "list"  /* ListProperty */
/* (offset is a GPtrArray of (const gchar *). offset2 is a gint, index of the 
   active item, -1 if none active.) */

/* Special types : */
#define PROP_TYPE_SARRAY "sarray" /* ArrayProperty */
#define PROP_TYPE_DARRAY "darray" /* ArrayProperty */
#define PROP_TYPE_DICT "dict" /* DictProperty */

/* **************************************************************** */

struct _PropDescription {
  const gchar *name;
  PropertyType type;
  guint flags;
  const gchar *description;
  const gchar *tooltip;

  /* Holds some extra data whose meaning is dependent on the property type.
   * For example, int or float may use bounds for a spin button, and enums
   * may use a list of string names for enumeration values. */
  gpointer extra_data;

  /* if the property widget can send events when it's somehow interacted with,
     control will be passed to object_type-supplied event_handler, and 
     event->dialog->obj_copy will be current with the dialog's values.
     When control comes back, event->dialog->obj_copy's properties will be 
     brought back into the dialog. */
  PropEventHandler event_handler;

  GQuark quark; /* quark for property name -- helps speed up lookups. */
  GQuark type_quark;

  /* only used by dynamically constructed property descriptors (eg. groups) */ 
  PropEventHandlerChain chain_handler;

  const PropertyOps *ops;      
};

#define PROP_FLAG_VISIBLE   0x0001
#define PROP_FLAG_DONT_SAVE 0x0002
#define PROP_FLAG_DONT_MERGE 0x0004 /* in case group properties are edited */
#define PROP_FLAG_NO_DEFAULTS 0x0008 /* don't edit this in defaults dlg. */
#define PROP_FLAG_LOAD_ONLY 0x0010 /* for loading old formats */
#define PROP_FLAG_STANDARD 0x0020 /* One of the default toolbox props */
#define PROP_FLAG_MULTIVALUE 0x0040 /* Multiple values for prop in group */
#define PROP_FLAG_WIDGET_ONLY 0x0080 /* only cosmetic property, no data */
#define PROP_FLAG_OPTIONAL 0x0100 /* don't complain if it does not exist */

typedef enum {PROP_UNION, PROP_INTERSECTION} PropMergeOption;

#define PROP_DESC_END { NULL, 0, 0, NULL, NULL, NULL, 0 }

/* extra data pointers for various property types */
struct _PropNumData {
  gfloat min, max, step;
};
struct _PropEnumData {
  const gchar *name;
  guint enumv;
};

typedef gpointer (*NewRecordFunc)(void);
typedef void (*FreeRecordFunc)(gpointer rec);

struct _PropDescCommonArrayExtra { /* don't use this directly. 
                                      Use one of below */
  PropDescription *record;
  PropOffset      *offsets; /* the offsets into the structs in the list/array */
  const gchar     *composite_type; /* can be NULL. */ 
};

struct _PropDescDArrayExtra {
  PropDescCommonArrayExtra common; /* must be first */
  NewRecordFunc newrec;        
  FreeRecordFunc freerec; 
};

struct  _PropDescSArrayExtra {
  PropDescCommonArrayExtra common; /* must be first */
  guint element_size;  /* sizeof(record) */
  guint array_len;
};


/* ******************* */
/* The Property itself */
/* ******************* */ 
struct _Property {
  const gchar *name;
  GQuark name_quark; 
  PropertyType type;
  GQuark type_quark;
  const PropDescription *descr;
  gpointer extra_data;
  PropEventData self;
  PropEventHandler event_handler;
  PropDescToPropPredicate reason; /* why has this property been created from
                                     the pdesc ? */
  guint experience;       /* flags PXP_.... */
 
  const PropertyOps *ops;       /* points to common_prop_ops */
  const PropertyOps *real_ops;  /* == descr->ops */
};

/* prop->experience flags */
#define PXP_COPIED           0x00000001  /* has been copied */
#define PXP_COPY             0x00000002  /* is a copy */
#define PXP_GET_WIDGET       0x00000004
#define PXP_RESET_WIDGET     0x00000008
#define PXP_SET_FROM_WIDGET  0x00000010
#define PXP_LOADED           0x00000020
#define PXP_SAVED            0x00000040
#define PXP_GFO              0x00000080
#define PXP_SFO              0x00000100
#define PXP_NOTSET           0x00000200
#define PXP_SHAMELESS        0xFFFFFFFF

/* ***************************************************************** */
/* Operations on property descriptors and property descriptor lists. */

void prop_desc_list_calculate_quarks(PropDescription *plist);
/* plist must have all quarks calculated in advance */
const PropDescription *prop_desc_list_find_prop(const PropDescription *plist, 
                                                const gchar *name);
/* finds the real handler in case there are several levels of indirection */
PropEventHandler prop_desc_find_real_handler(const PropDescription *pdesc);
/* free a handler indirection list */
void prop_desc_free_handler_chain(PropDescription *pdesc);
/* free a handler indirection list in a list of descriptors */
void prop_desc_list_free_handler_chain(PropDescription *pdesc);
/* insert an event handler */
void prop_desc_insert_handler(PropDescription *pdesc, 
                              PropEventHandler handler);

/* operations on lists of property description lists */
PropDescription *prop_desc_lists_union(GList *plists);
PropDescription *prop_desc_lists_intersection(GList *plists);


/* ********************************************* */
/* Functions for dealing with the Type Registry  */
void prop_type_register(PropertyType type, const PropertyOps *ops);
const PropertyOps *prop_type_get_ops(PropertyType type);

/* *********************************************************** */
/* functions for manipulating a property array.                */

void       prop_list_free(GPtrArray *plist);


/* copies the whole property structure, including the data. */
GPtrArray *prop_list_copy(GPtrArray *plist);
/* copies the whole property structure, excluding the data. */
GPtrArray *prop_list_copy_empty(GPtrArray *plist);
/* Appends copies of the properties in the second list to the first. */
void prop_list_add_list (GPtrArray *props, const GPtrArray *ptoadd);

GPtrArray *prop_list_from_descs(const PropDescription *plist, 
                                PropDescToPropPredicate pred);

GPtrArray *prop_list_from_single(Property *prop);

/* Some predicates: */
gboolean pdtpp_true(const PropDescription *pdesc); /* always true */
gboolean pdtpp_is_visible(const PropDescription *pdesc); 
gboolean pdtpp_is_visible_no_standard(const PropDescription *pdesc); 
gboolean pdtpp_is_not_visible(const PropDescription *pdesc); 
gboolean pdtpp_do_save(const PropDescription *pdesc); 
gboolean pdtpp_do_save_no_standard(const PropDescription *pdesc);
gboolean pdtpp_do_load(const PropDescription *pdesc);
gboolean pdtpp_do_not_save(const PropDescription *pdesc); 
/* all but DONT_MERGE and NO_DEFAULTS: */
gboolean pdtpp_defaults(const PropDescription *pdesc); 
/* actually used for the "reason" parameter, not as predicates (synonyms for pdtpp_true) */
gboolean pdtpp_synthetic(const PropDescription *pdesc);
gboolean pdtpp_from_object(const PropDescription *pdesc);


/* Swallows the property into a single property list. Can be given NULL. 
   Don't free yourself the property afterwards; prop_list_free() the list 
   instead.
   You regain responsibility for the property if you g_ptr_array_destroy() the
   list. */
GPtrArray *prop_list_of_single(Property *prop);

/* Create a new property of the required type, with the required name.
   A PropDescription might be created on the fly. The property's value is not 
   initialised (actually, it's zero). */
Property *make_new_prop(const char *name, PropertyType type, guint flags);



/* Offset to fields in objects */

/* calculates the offset of a structure member within the structure */
#ifndef offsetof
#define offsetof(type, member) ( (int) & ((type*)0) -> member )
#endif
struct _PropOffset {
  const gchar *name;
  PropertyType type;
  int offset;
  int offset2; /* maybe for point lists, etc */
  GQuark name_quark;
  GQuark type_quark;
  const PropertyOps *ops;
};

/* ************************************************ */
/* routines used by Objects or to deal with Objects */

/* returns TRUE if this object can be handled (at least in part) through this
   library. */
gboolean object_complies_with_stdprop(const DiaObject *obj);
gboolean objects_comply_with_stdprop(GList *objects);

void object_list_get_props(GList *objects, GPtrArray *props);

/* will do whatever is needed to make the PropDescription * list look good to 
   the rest of the properties code. Can return NULL. */
const PropDescription *object_get_prop_descriptions(const DiaObject *obj);
const PropDescription *object_list_get_prop_descriptions(GList *objects,
							 PropMergeOption option);

gboolean object_get_props_from_offsets(DiaObject *obj, PropOffset *offsets,
				       GPtrArray *props);
gboolean object_set_props_from_offsets(DiaObject *obj, PropOffset *offsets,
				       GPtrArray *props);

/* apply some properties and return a corresponding object change */
ObjectChange *object_apply_props(DiaObject *obj, GPtrArray *props);

/* standard properties dialogs that can be used for objects that
 * implement describe_props, get_props and set_props.
 * If is_default is set, this is a default dialog, not an object dialog.
 */
WIDGET *object_create_props_dialog     (DiaObject *obj, gboolean is_default);
WIDGET *object_list_create_props_dialog(GList *obj, gboolean is_default);
ObjectChange *object_apply_props_from_dialog (DiaObject *obj, WIDGET *dialog);

/* create a property from the object's property descriptors. To be freed with
   prop->ops->free(prop); or put it into a single property list. NULL if object
   has nothing matching. Property's value is initialised by the object.
   Serve cold. */
Property *object_prop_by_name(DiaObject *obj, const char *name);
Property *object_prop_by_name_type(DiaObject *obj, const char *name, const char *type);

/* standard way to load/save properties of an object */
void          object_load_props(DiaObject *obj, ObjectNode obj_node);
void          object_save_props(DiaObject *obj, ObjectNode obj_node);

/* standard way to copy the properties of an object into another (of the
   same type) */
void          object_copy_props(DiaObject *dest, const DiaObject *src,
                                gboolean is_default);

/* Return a reference to objects property with 'name' or NULL */
Property     *object_get_prop_by_name (DiaObject *obj, const char* name);
 
/* ************************************************************* */ 

void stdprops_init(void);

/* ************************************************************* */ 

/* standard properties.  By using these, the intersection of the properties
 * of a number of objects should be greater, making setting properties on
 * groups better. */

/* HB: exporting the following two vars by GIMPVAR==dllexport/dllimport, 
 * does mean the pointers used below have to be calculated 
 * at run-time by the loader, because they will exist
 * only once in the process space and dynamic link libraries may be
 * relocated. As a result their address is no longer constant. 
 * Indeed it causes compile time errors with MSVC (initialzer 
 * not a constant).
 * To fix it they are moved form properties.c and declared as static 
 * on Win32
 */
#ifdef G_OS_WIN32
static PropNumData prop_std_line_width_data = { 0.0f, 10.0f, 0.01f };
static PropNumData prop_std_text_height_data = { 0.1f, 10.0f, 0.1f };
static PropEnumData prop_std_text_align_data[] = {
  { N_("Left"), ALIGN_LEFT },
  { N_("Center"), ALIGN_CENTER },
  { N_("Right"), ALIGN_RIGHT },
  { NULL, 0 }
};
#else
extern PropNumData prop_std_line_width_data, prop_std_text_height_data;
extern PropEnumData prop_std_text_align_data[];
#endif

#define PROP_STDNAME_LINE_WIDTH "line_width"
#define PROP_STDTYPE_LINE_WIDTH PROP_TYPE_LENGTH
#define PROP_STD_LINE_WIDTH \
  { PROP_STDNAME_LINE_WIDTH, PROP_STDTYPE_LINE_WIDTH, PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD, \
    N_("Line width"), NULL, &prop_std_line_width_data }
#define PROP_STD_LINE_WIDTH_OPTIONAL \
  { PROP_STDNAME_LINE_WIDTH, PROP_STDTYPE_LINE_WIDTH, PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD|PROP_FLAG_OPTIONAL, \
    N_("Line width"), NULL, &prop_std_line_width_data }
#define PROP_STD_LINE_COLOUR \
  { "line_colour", PROP_TYPE_COLOUR, PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD, \
    N_("Line color"), NULL, NULL }
#define PROP_STD_LINE_COLOUR_OPTIONAL \
  { "line_colour", PROP_TYPE_COLOUR, PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD|PROP_FLAG_OPTIONAL, \
    N_("Line color"), NULL, NULL }
#define PROP_STD_LINE_STYLE \
  { "line_style", PROP_TYPE_LINESTYLE, PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD, \
    N_("Line style"), NULL, NULL }
#define PROP_STD_LINE_STYLE_OPTIONAL \
  { "line_style", PROP_TYPE_LINESTYLE, PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD|PROP_FLAG_OPTIONAL, \
    N_("Line style"), NULL, NULL }

#define PROP_STD_FILL_COLOUR \
  { "fill_colour", PROP_TYPE_COLOUR, PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD, \
    N_("Fill color"), NULL, NULL }
#define PROP_STD_FILL_COLOUR_OPTIONAL \
  { "fill_colour", PROP_TYPE_COLOUR, PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD|PROP_FLAG_OPTIONAL, \
    N_("Fill color"), NULL, NULL }
#define PROP_STD_SHOW_BACKGROUND \
  { "show_background", PROP_TYPE_BOOL, PROP_FLAG_VISIBLE, \
    N_("Draw background"), NULL, NULL }
#define PROP_STD_SHOW_BACKGROUND_OPTIONAL \
  { "show_background", PROP_TYPE_BOOL, PROP_FLAG_VISIBLE|PROP_FLAG_OPTIONAL, \
    N_("Draw background"), NULL, NULL }

#define PROP_STD_START_ARROW \
  { "start_arrow", PROP_TYPE_ARROW, PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD, \
    N_("Start arrow"), NULL, NULL }
#define PROP_STD_END_ARROW \
  { "end_arrow", PROP_TYPE_ARROW, PROP_FLAG_VISIBLE|PROP_FLAG_STANDARD, \
    N_("End arrow"), NULL, NULL }

#define PROP_STD_TEXT_OPTIONS(options) \
  { "text", PROP_TYPE_TEXT, (options), \
    N_("Text"), NULL, NULL }
#define PROP_STD_TEXT PROP_STD_TEXT_OPTIONS(PROP_FLAG_DONT_SAVE)
#define PROP_STD_SAVED_TEXT PROP_STD_TEXT_OPTIONS(0)

#define PROP_STD_TEXT_ALIGNMENT_OPTIONS(options) \
  { "text_alignment", PROP_TYPE_ENUM, (options), \
    N_("Text alignment"), NULL, prop_std_text_align_data }
#define PROP_STD_TEXT_ALIGNMENT \
        PROP_STD_TEXT_ALIGNMENT_OPTIONS(PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE)

#define PROP_STD_TEXT_FONT_OPTIONS(options) \
  { "text_font", PROP_TYPE_FONT, (options), N_("Font"), NULL, NULL }
#define PROP_STD_TEXT_FONT \
        PROP_STD_TEXT_FONT_OPTIONS(PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE)

#define PROP_STDNAME_TEXT_HEIGHT "text_height"
#define PROP_STDTYPE_TEXT_HEIGHT PROP_TYPE_FONTSIZE
#define PROP_STD_TEXT_HEIGHT_OPTIONS(options) \
  { PROP_STDNAME_TEXT_HEIGHT, PROP_STDTYPE_TEXT_HEIGHT, (options), \
    N_("Font size"), NULL, &prop_std_text_height_data }
#define PROP_STD_TEXT_HEIGHT \
        PROP_STD_TEXT_HEIGHT_OPTIONS(PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE)

#define PROP_STD_TEXT_COLOUR_OPTIONS(options) \
  { "text_colour", PROP_TYPE_COLOUR, (options), \
    N_("Text color"), NULL, NULL }
#define PROP_STD_TEXT_COLOUR \
        PROP_STD_TEXT_COLOUR_OPTIONS(PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE|PROP_FLAG_STANDARD)
#define PROP_STD_TEXT_COLOUR_OPTIONAL \
        PROP_STD_TEXT_COLOUR_OPTIONS(PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE|PROP_FLAG_STANDARD|PROP_FLAG_OPTIONAL)

/* Convenience macros */
#define PROP_NOTEBOOK_BEGIN(name) \
  { "nbook_" name, PROP_TYPE_NOTEBOOK_BEGIN, \
              PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE|PROP_FLAG_WIDGET_ONLY|PROP_FLAG_DONT_MERGE, NULL, NULL}
#define PROP_STD_NOTEBOOK_BEGIN PROP_NOTEBOOK_BEGIN("std")
#define PROP_OFFSET_NOTEBOOK_BEGIN(name) \
  { "nbook_" name, PROP_TYPE_NOTEBOOK_BEGIN, 0}
#define PROP_OFFSET_STD_NOTEBOOK_BEGIN PROP_OFFSET_NOTEBOOK_BEGIN("std")

#define PROP_NOTEBOOK_END(name) \
  { "nbook_" name "_end", PROP_TYPE_NOTEBOOK_END, \
      PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE|PROP_FLAG_WIDGET_ONLY|PROP_FLAG_DONT_MERGE, NULL, NULL}
#define PROP_STD_NOTEBOOK_END PROP_NOTEBOOK_END("std")
#define PROP_OFFSET_NOTEBOOK_END(name) \
  { "nbook_" name "_end", PROP_TYPE_NOTEBOOK_END, 0}
#define PROP_OFFSET_STD_NOTEBOOK_END PROP_OFFSET_NOTEBOOK_END("std")

#define PROP_NOTEBOOK_PAGE(name,flags,descr) \
  { "nbook_page_" name, PROP_TYPE_NOTEBOOK_PAGE, \
 PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE|PROP_FLAG_WIDGET_ONLY|PROP_FLAG_DONT_MERGE|flags,descr,NULL}
#define PROP_OFFSET_NOTEBOOK_PAGE(name) \
  { "nbook_page_" name , PROP_TYPE_NOTEBOOK_PAGE, 0}

#define PROP_MULTICOL_BEGIN(name) \
  { "mcol_" name, PROP_TYPE_MULTICOL_BEGIN, \
              PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE|PROP_FLAG_WIDGET_ONLY, NULL, NULL}
#define PROP_OFFSET_MULTICOL_BEGIN(name) \
  { "mcol_" name, PROP_TYPE_NOTEBOOK_BEGIN, 0}

#define PROP_MULTICOL_END(name) \
  { "mcol_" name "_end", PROP_TYPE_MULTICOL_END, \
      PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE|PROP_FLAG_WIDGET_ONLY, NULL, NULL}
#define PROP_OFFSET_MULTICOL_END(name) \
  { "mcol_" name "_end", PROP_TYPE_MULTICOL_END, 0}

#define PROP_MULTICOL_COLUMN(name) \
  { "mcol_col_" name, PROP_TYPE_MULTICOL_COLUMN, \
 PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE|PROP_FLAG_WIDGET_ONLY,NULL,NULL}
#define PROP_OFFSET_MULTICOL_COLUMN(name) \
  { "mcol_col_" name, PROP_TYPE_MULTICOL_COLUMN, 0}

#define PROP_FRAME_BEGIN(name,flags,descr) \
  { "frame_" name, PROP_TYPE_FRAME_BEGIN, \
              PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE|PROP_FLAG_WIDGET_ONLY|flags, descr, NULL}
#define PROP_OFFSET_FRAME_BEGIN(name) \
  { "frame_" name, PROP_TYPE_FRAME_BEGIN, 0}

#define PROP_FRAME_END(name,flags) \
  { "frame_" name "_end", PROP_TYPE_FRAME_END, \
      PROP_FLAG_VISIBLE|PROP_FLAG_DONT_SAVE|PROP_FLAG_WIDGET_ONLY|flags, NULL, NULL}
#define PROP_OFFSET_FRAME_END(name) \
  { "frame_" name "_end", PROP_TYPE_FRAME_END, 0}

G_END_DECLS

#endif
