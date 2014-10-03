/* Dia -- a diagram creation/manipulation program
 * Copyright (C) 1998 Alexander Larsson
 *
 * Property system for dia objects/shapes.
 * Copyright (C) 2000 James Henstridge
 * Copyright (C) 2001 Cyrille Chepelov
 * Major restructuration done in August 2001 by C. Chepelov
 *
 * properties.c: initialisation routines and stuff that didn't fit anywhere 
 * else. Most of what used to be here has been moved elsewhere !
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

#include "geometry.h"
#ifndef LIBDIA_COMPILATION
#  define LIBDIA_COMPILATION
#endif
#undef G_INLINE_FUNC
#define G_INLINE_FUNC extern
#define G_CAN_INLINE 1
#include "properties.h"
#include "propinternals.h"

/* --------------------------------------- */

void 
stdprops_init(void)
{
  prop_basic_register();
  prop_inttypes_register();
  prop_geomtypes_register();
  prop_attr_register();
  prop_text_register();
  prop_widgets_register();
  prop_sdarray_register();
  prop_dicttypes_register();
}

/* --------------------------------------- */

#ifdef G_OS_WIN32
/* moved to properties.h ... */
#else
/* standard property extra data members */
PropNumData prop_std_line_width_data = { 0.0, 10.0, 0.01 };
PropNumData prop_std_text_height_data = { 0.1, 10.0, 0.1 };
PropEnumData prop_std_text_align_data[] = {
  { N_("Left"), ALIGN_LEFT },
  { N_("Center"), ALIGN_CENTER },
  { N_("Right"), ALIGN_RIGHT },
  { NULL, 0 }
};
#endif

#ifdef FOR_TRANSLATORS_ONLY
static char *list [] = {
	N_("Line color"),
	N_("Line style"),
	N_("Fill color"),
	N_("Draw background"),
	N_("Start arrow"),
	N_("End arrow"),
	N_("Text"),
	N_("Text alignment"),
	N_("Font"),
	N_("Font size"),
	N_("Text color")
};
#endif
