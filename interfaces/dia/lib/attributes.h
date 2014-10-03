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
#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "dia-enums.h"
#include "geometry.h"
#include "color.h"
#include "arrows.h"
#include "font.h"

Color attributes_get_foreground(void);
Color attributes_get_background(void);
void attributes_set_foreground(Color *color);
void attributes_set_background(Color *color);
void attributes_swap_fgbg(void);
void attributes_default_fgbg(void);

real attributes_get_default_linewidth(void);
void attributes_set_default_linewidth(real width);

Arrow attributes_get_default_start_arrow(void);
void attributes_set_default_start_arrow(Arrow arrow);

Arrow attributes_get_default_end_arrow(void);
void attributes_set_default_end_arrow(Arrow arrow);

void attributes_get_default_line_style(LineStyle *style, real *dash_length);
void attributes_set_default_line_style(LineStyle style, real dash_length);

void attributes_get_default_font(DiaFont **font, real *font_height);
void attributes_set_default_font(DiaFont *font, real font_height);

#endif /* ATTRIBUTES_H */
