
#ifndef __dia_marshal_MARSHAL_H__
#define __dia_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:VOID (./diamarshal.list:25) */
#define dia_marshal_VOID__VOID	g_cclosure_marshal_VOID__VOID

/* VOID:OBJECT (./diamarshal.list:26) */
#define dia_marshal_VOID__OBJECT	g_cclosure_marshal_VOID__OBJECT

/* VOID:INT (./diamarshal.list:27) */
#define dia_marshal_VOID__INT	g_cclosure_marshal_VOID__INT

/* VOID:POINTER,POINTER (./diamarshal.list:28) */
extern void dia_marshal_VOID__POINTER_POINTER (GClosure     *closure,
                                               GValue       *return_value,
                                               guint         n_param_values,
                                               const GValue *param_values,
                                               gpointer      invocation_hint,
                                               gpointer      marshal_data);

/* VOID:STRING,FLAGS (./diamarshal.list:29) */
extern void dia_marshal_VOID__STRING_FLAGS (GClosure     *closure,
                                            GValue       *return_value,
                                            guint         n_param_values,
                                            const GValue *param_values,
                                            gpointer      invocation_hint,
                                            gpointer      marshal_data);

G_END_DECLS

#endif /* __dia_marshal_MARSHAL_H__ */

