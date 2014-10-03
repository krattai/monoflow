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
#ifndef GRID_H
#define GRID_H

#include <gtk/gtk.h>

typedef struct _Grid Grid;

#include "geometry.h"
struct _Grid {
  guint visible;
  guint snap;
};

#include "display.h"

void grid_draw(DDisplay *ddisp, Rectangle *update);
void pagebreak_draw(DDisplay *ddisp, Rectangle *update);
void snap_to_grid(DDisplay *ddisp, coord *x, coord *y);

#endif /* GRID_H */
