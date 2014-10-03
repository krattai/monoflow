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
#ifndef APP_PROCS_H
#define APP_PROCS_H

#include <glib.h>

void app_init(int argc, char **argv);
/** Exit the application, but asking the user for confirmation
 * if there are changed diagrams.
 * Returns TRUE if the application exits.
 */
gboolean app_exit(void);
int app_is_embedded(void);

void app_splash_init(const gchar* name);
void app_splash_done(void);

gboolean app_is_interactive(void);

void dia_redirect_console (void);

#endif /* APP_PROCS_H */
