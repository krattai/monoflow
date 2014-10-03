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

/** \file color.h Diagram and object tinting */

#ifndef COLOR_H
#define COLOR_H

#include "diatypes.h"
#include <gdk/gdk.h>
#include "diavar.h"

struct _Color {
  float red;
  float green;
  float blue;
};

void color_init(void);
Color *color_new_rgb(float r, float g, float b);
void color_convert(Color *color, GdkColor *gdkcolor);
gboolean color_equals(Color *color1, Color *color2);

#ifdef G_OS_WIN32
static Color color_black = { 0.0f, 0.0f, 0.0f };
static Color color_white = { 1.0f, 1.0f, 1.0f };
#else
DIAVAR Color color_black, color_white;
#endif
DIAVAR GdkColor color_gdk_black, color_gdk_white;

#define DIA_COLOR_TO_GDK(from, to) \
(to).pixel = 0; \
(to).red = (from).red*65535; \
(to).green = (from).green*65535; \
(to).blue = (from).blue*65535;
#define GDK_COLOR_TO_DIA(from, to) \
(to).red = (from).red/65535.0; \
(to).green = (from).green/65535.0; \
(to).blue = (from).blue/65535.0;

#endif /* COLOR_H */
