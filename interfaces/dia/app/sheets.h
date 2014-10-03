/* Dia -- a diagram creation/manipulation program
 * Copyright (C) 1998 Alexander Larsson
 *
 * sheets.h : a sheets and objects dialog
 * Copyright (C) 2002 M.C. Nelson
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
 *  
 */

#ifndef SHEETS_H

#ifdef GNOME
#include <gnome.h>
#else
#include <gtk/gtk.h>
#endif

#include "../lib/sheet.h"

#include "sheets_dialog_callbacks.h"

/* The theory behind these structures is simple.  Sheets and SheetObjects
   are wrapped in SheetMod's and SheetObjectMod's respectively.  Any changes
   made by the user to the Sheet or SheetObject is reflected in the *Mod
   subclass.  When the user commits, the information is written back to 
   the datastore and the *Mod lists discarded. */

typedef struct _SheetObjectMod SheetObjectMod;
typedef struct _SheetMod       SheetMod;

struct _SheetObjectMod
{
  SheetObject sheet_object;

  enum { OBJECT_TYPE_SVG,
         OBJECT_TYPE_PROGRAMMED,
         OBJECT_TYPE_UNASSIGNED } type;

  enum { SHEET_OBJECT_MOD_NONE,
         SHEET_OBJECT_MOD_NEW,
         SHEET_OBJECT_MOD_CHANGED,
         SHEET_OBJECT_MOD_DELETED } mod;

  gchar *svg_filename;				/* For a new sheet_object */
};

struct _SheetMod
{
  Sheet sheet;
  
  enum { SHEETMOD_TYPE_NORMAL,
         SHEETMOD_TYPE_UNASSIGNED } type;	/* reserved for future use */

  enum { SHEETMOD_MOD_NONE,
         SHEETMOD_MOD_NEW,
         SHEETMOD_MOD_CHANGED,
         SHEETMOD_MOD_DELETED } mod;
};

extern GtkWidget *sheets_dialog;
extern GtkWidget *sheets_dialog_optionmenu_menu;
#ifndef GTK_DISABLE_DEPRECATED
extern GtkTooltips *sheets_dialog_tooltips;
#endif

SheetObjectMod *sheets_append_sheet_object_mod(SheetObject *so, SheetMod *sm);
SheetMod *sheets_append_sheet_mods(Sheet *sheet);
void sheets_optionmenu_create(GtkWidget *option_menu, GtkWidget *wrapbox,
                              gchar *sheet_name);
void create_object_pixmap(SheetObject *so, GtkWidget *parent,
                          GdkPixmap **pixmap, GdkBitmap **mask);

gchar *sheet_object_mod_get_type_string(SheetObjectMod *som);

gboolean sheets_dialog_create(void);
void sheets_dialog_show_callback(gpointer data, guint action,
                                 GtkWidget *widget);
GtkWidget *lookup_widget(GtkWidget       *widget,
                         const gchar     *widget_name);
GtkWidget *create_pixmap(GtkWidget *dialog, gchar *filename, gboolean arg3);

#endif /* SHEETS_H */
