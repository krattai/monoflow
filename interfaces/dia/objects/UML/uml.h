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

/** \file objects/UML/uml.h  Objects contained  in 'UML - Class' type also and helper functions */

#ifndef UML_H
#define UML_H

#include <glib.h>
#include "intl.h"
#include "connectionpoint.h"
#include "dia_xml.h"

typedef struct _UMLAttribute UMLAttribute;
typedef struct _UMLOperation UMLOperation;
typedef struct _UMLParameter UMLParameter;
typedef struct _UMLFormalParameter UMLFormalParameter;

/** the visibility (allowed acces) of (to) various UML sub elements */
typedef enum _UMLVisibility {
  UML_PUBLIC, /**< everyone can use it */
  UML_PRIVATE, /**< only accessible inside the class itself */
  UML_PROTECTED, /**< the class and its inheritants ca use this */
  UML_IMPLEMENTATION /**< ?What's this? Means implementation decision */
} UMLVisibility;

/** In some languages there are different kinds of class inheritances */
typedef enum _UMLInheritanceType {
  UML_ABSTRACT, /**< Pure virtual method: an object of this class cannot be instanciated */
  UML_POLYMORPHIC, /**< Virtual method : could be reimplemented in derivated classes */
  UML_LEAF /**< Final method: can't be redefined in subclasses */
} UMLInheritanceType;

/** describes the data flow between caller and callee */
typedef enum _UMLParameterKind {
  UML_UNDEF_KIND, /**< not defined */
  UML_IN, /**< by value */
  UML_OUT, /**< by ref, can be passed in uninitialized */
  UML_INOUT /**< by ref */
} UMLParameterKind;

typedef gchar * UMLStereotype;

/** \brief A list of UMLAttribute is contained in UMLClass
 * Some would call them member variables ;)
 */
struct _UMLAttribute {
  gint internal_id; /**< Arbitrary integer to recognize attributes after 
		     * the user has shuffled them in the dialog. */
  gchar *name; /**< the member variables name */
  gchar *type; /**< the return value */
  gchar *value; /**< default parameter : Can be NULL => No default value */
  gchar *comment; /**< comment */
  UMLVisibility visibility; /**< attributes visibility */
  int abstract; /**< not sure if this applicable */
  int class_scope; /**< in C++ : static member */
  
  ConnectionPoint* left_connection; /**< left */
  ConnectionPoint* right_connection; /**< right */
};

/** \brief A list of UMLOperation is contained in UMLClass
 * Some would call them member functions ;)
 */
struct _UMLOperation {
  gint internal_id; /**< Arbitrary integer to recognize operations after
		     * the user has shuffled them in the dialog. */
  gchar *name; /**< the function name */
  gchar *type; /**< Return type, NULL => No return type */
  gchar *comment; /**< comment */  
  UMLStereotype stereotype; /**< just some string */
  UMLVisibility visibility; /**< allowed access */
  UMLInheritanceType inheritance_type;
  int query; /**< Do not modify the object, in C++ this is a const function */
  int class_scope;
  GList *parameters; /**< List of UMLParameter */

  ConnectionPoint* left_connection; /**< left */
  ConnectionPoint* right_connection; /**< right */

  gboolean needs_wrapping; /** Whether this operation needs wrapping */
  gint wrap_indent; /** The amount of indentation in chars */
  GList *wrappos; /** Absolute wrapping positions */
  real ascent; /** The ascent amount used for line distance in wrapping */
};


/** \brief A list of UMLParameter is contained in UMLOperation
 * Some would call them functions parameters ;)
 */
struct _UMLParameter {
  gchar *name; /**<  name*/
  gchar *type; /**< return value */
  gchar *value; /**< Initialization,  can be NULL => No default value */
  gchar *comment; /**< comment */
  UMLParameterKind kind; /**< Not currently used */
};

/** \brief A list of UMLFormalParameter is contained in UMLOperation
 * Some would call them template parameters ;)
 */
struct _UMLFormalParameter {
  gchar *name; /**< name */
  gchar *type; /**< Can be NULL => Type parameter */
};

/* Characters used to start/end stereotypes: */
/** start stereotype symbol(like \xab) for local locale */
#define UML_STEREOTYPE_START _("<<")
/** end stereotype symbol(like \xbb) for local locale */
#define UML_STEREOTYPE_END _(">>")

/** calculated the 'formated' representation */
extern gchar *uml_get_attribute_string (UMLAttribute *attribute);
/** calculated the 'formated' representation */
extern gchar *uml_get_operation_string(UMLOperation *operation);
/** calculated the 'formated' representation */
extern gchar *uml_get_parameter_string(UMLParameter *param);
/** calculated the 'formated' representation */
extern gchar *uml_get_formalparameter_string(UMLFormalParameter *parameter);
extern void uml_attribute_copy_into(UMLAttribute *srcattr, UMLAttribute *destattr);
extern UMLAttribute *uml_attribute_copy(UMLAttribute *attr);
extern void uml_operation_copy_into(UMLOperation *srcop, UMLOperation *destop);
extern UMLOperation *uml_operation_copy(UMLOperation *op);
extern UMLFormalParameter *uml_formalparameter_copy(UMLFormalParameter *param);
extern void uml_attribute_destroy(UMLAttribute *attribute);
extern void uml_operation_destroy(UMLOperation *op);
extern void uml_parameter_destroy(UMLParameter *param);
extern void uml_formalparameter_destroy(UMLFormalParameter *param);
extern UMLAttribute *uml_attribute_new(void);
extern UMLOperation *uml_operation_new(void);
extern UMLParameter *uml_parameter_new(void);
extern UMLFormalParameter *uml_formalparameter_new(void);

extern void uml_attribute_ensure_connection_points (UMLAttribute *attr, DiaObject* obj);
extern void uml_operation_ensure_connection_points (UMLOperation *oper, DiaObject* obj);

extern void uml_attribute_write(AttributeNode attr_node, UMLAttribute *attr);
extern void uml_operation_write(AttributeNode attr_node, UMLOperation *op);
extern void uml_formalparameter_write(AttributeNode attr_node, UMLFormalParameter *param);

#endif /* UML_H */

