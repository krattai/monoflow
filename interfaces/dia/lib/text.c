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
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <math.h>

#include <gdk/gdkkeysyms.h>

#include "propinternals.h"
#include "text.h"
#include "message.h"
#include "diarenderer.h"
#include "diagramdata.h"
#include "objchange.h"
#include "textline.h"

static int text_key_event(Focus *focus, guint keysym,
			  const gchar *str, int strlen,
			  ObjectChange **change);

enum change_type {
  TYPE_DELETE_BACKWARD,
  TYPE_DELETE_FORWARD,
  TYPE_INSERT_CHAR,
  TYPE_JOIN_ROW,
  TYPE_SPLIT_ROW,
  TYPE_DELETE_ALL
};

struct TextObjectChange {
  ObjectChange obj_change;

  Text *text;
  enum change_type type;
  gunichar ch;
  int pos;
  int row;
  gchar *str;
};

#define CURSOR_HEIGHT_RATIO 20

/* *** Encapsulation functions for transferring to text_line *** */
gchar *
text_get_line(Text *text, int line)
{
  return text_line_get_string(text->lines[line]);
}

/** Raw sets one line to a given text, not copying, not freeing.
 */
static void
text_set_line(Text *text, int line_no, gchar *line)
{
  text_line_set_string(text->lines[line_no], line);
}

/** Set the text of a line, freeing, copying and mallocing as required.
 * Updates strlen and row_width entries, but not max_width.
 */
static void
text_set_line_text(Text *text, int line_no, gchar *line)
{
  text_set_line(text, line_no, line);
}

/** Delete the line, freeing appropriately and moving stuff up.
 * This function circumvents the normal free/alloc cycle of 
 * text_set_line_text. */
static void
text_delete_line(Text *text, int line_no)
{
  int i;

  g_free(text->lines[line_no]);
  for (i = line_no; i < text->numlines - 1; i++) {
    text->lines[i] = text->lines[i+1];
  }
  text->numlines -= 1;
  text->lines = g_realloc(text->lines, sizeof(TextLine *)*text->numlines);
}

/** Insert a new (empty) line at line_no.
 * This function circumvents the normal free/alloc cycle of 
 * text_set_line_text. */
static void
text_insert_line(Text *text, int line_no)
{
  int i;
  text->numlines += 1;
  text->lines = g_realloc(text->lines, sizeof(char *)*text->numlines);

  for (i = text->numlines - 1; i > line_no; i--) {
    text->lines[i] = text->lines[i - 1];
  }
  text->lines[line_no] = text_line_new("", text->font, text->height);;
}

/** Get the in-diagram width of the given line.
 * @param text The text object;
 * @param line_no The index of the line in the text object, starting at 0.
 * @returns The width in cm of the indicated line.
 */
real
text_get_line_width(Text *text, int line_no)
{
  return text_line_get_width(text->lines[line_no]);
}

/** Get the number of characters of the given line.
 * @param text The text object;
 * @param line_no The index of the line in the text object, starting at 0.
 * @returns The number of UTF-8 characters of the indicated line.
 */
int
text_get_line_strlen(Text *text, int line_no)
{
  return g_utf8_strlen(text_line_get_string(text->lines[line_no]), -1);
}

real
text_get_max_width(Text *text)
{
  return text->max_width;
}

/** Get the *average* ascent of this Text object.
 * @param a Text object
 * @returns the average of the ascents of each line (height above baseline)
 */
real
text_get_ascent(Text *text)
{
  return text->ascent;
}

/** Get the *average* descent of this Text object.
 * @param a Text object
 * @returns the average of the descents of each line (height below baseline)
 */
real
text_get_descent(Text *text)
{
  return text->descent;
}

static ObjectChange *text_create_change(Text *text, enum change_type type,
					gunichar ch, int pos, int row);

static void
calc_width(Text *text)
{
  real width;
  int i;

  width = 0.0;
  for (i = 0; i < text->numlines; i++) {
    width = MAX(width, text_get_line_width(text, i));
  }
  
  text->max_width = width;
}

static void
calc_ascent_descent(Text *text)
{
  real sig_a = 0.0,sig_d = 0.0;
  guint i;
    
  for ( i = 0; i < text->numlines; i++) {
    sig_a += text_line_get_ascent(text->lines[i]);
    sig_d += text_line_get_descent(text->lines[i]);
  }
  
  text->ascent = sig_a / (real)text->numlines;
  text->descent = sig_d / (real)text->numlines;
}

static void
free_string(Text *text)
{
  int i;

  for (i=0;i<text->numlines;i++) {
    text_line_destroy(text->lines[i]);
  }

  g_free(text->lines);
  text->lines = NULL;
}

static void
set_string(Text *text, const char *string)
{
  int numlines, i;
  const char *s,*s2;
  
  s = string;
  
  numlines = 1;
  if (s != NULL) 
    while ( (s = g_utf8_strchr(s, -1, '\n')) != NULL ) {
      numlines++;
      if (*s) {
	s = g_utf8_next_char(s);
      }
    }
  text->numlines = numlines;
  text->lines = g_new0(TextLine *, numlines);
  for (i = 0; i < numlines; i++) {
    text->lines[i] = text_line_new("", text->font, text->height);
  }

  s = string;

  if (string == NULL) {
    text_set_line_text(text, 0, "");
    return;
  }

  for (i = 0; i < numlines; i++) {
    gchar *string_line;
    s2 = g_utf8_strchr(s, -1, '\n');
    if (s2 == NULL) { /* No newline */
      s2 = s + strlen(s);
    }
    string_line = g_strndup(s, s2 - s);
    text_set_line_text(text, i, string_line);
    g_free(string_line);
    s = s2;
    if (*s) {
      s = g_utf8_next_char(s);
    }
  }

  if (text->cursor_row >= text->numlines) {
    text->cursor_row = text->numlines - 1;
  }
  
  if (text->cursor_pos > text_get_line_strlen(text, text->cursor_row)) {
    text->cursor_pos = text_get_line_strlen(text, text->cursor_row);
  }
}

void
text_set_string(Text *text, const char *string)
{
  if (text->lines != NULL)
    free_string(text);
  
  set_string(text, string);
}

Text *
new_text(const char *string, DiaFont *font, real height,
	 Point *pos, Color *color, Alignment align)
{
  Text *text;

  text = g_new(Text, 1);

  text->font = dia_font_ref(font);
  text->height = height;

  text->position = *pos;
  text->color = *color;
  text->alignment = align;

  text->cursor_pos = 0;
  text->cursor_row = 0;
  
  text->focus.obj = NULL;
  text->focus.has_focus = FALSE;
  text->focus.user_data = (void *)text;
  text->focus.key_event = text_key_event;
  text->focus.text = text;
  
  set_string(text, string);

  calc_ascent_descent(text);

  return text;
}

Text *
text_copy(Text *text)
{
  Text *copy;
  int i;

  copy = g_new(Text, 1);
  copy->numlines = text->numlines;
  copy->lines = g_new(TextLine *, text->numlines);
  
  copy->font = dia_font_copy(text->font);
  copy->height = text->height;
  copy->position = text->position;
  copy->color = text->color;
  copy->alignment = text->alignment;

  for (i=0;i<text->numlines;i++) {
    TextLine *text_line = text->lines[i];
    copy->lines[i] = text_line_new(text_line_get_string(text_line),
				   text_line_get_font(text_line),
				   text_line_get_height(text_line));
  }

  copy->cursor_pos = 0;
  copy->cursor_row = 0;
  copy->focus.obj = NULL;
  copy->focus.has_focus = FALSE;
  copy->focus.user_data = (void *)copy;
  copy->focus.key_event = text_key_event;
  copy->focus.text = copy;
  
  copy->ascent = text->ascent;
  copy->descent = text->descent;
  copy->max_width = text->max_width;
  
  return copy;
}

void
text_destroy(Text *text)
{
  free_string(text);
  dia_font_unref(text->font);
  g_free(text);
}

void
text_set_height(Text *text, real height)
{
  int i;
  text->height = height;
  for (i = 0; i < text->numlines; i++) {
    text_line_set_height(text->lines[i], height);
  }
  calc_width(text);
  calc_ascent_descent(text);
}

void
text_set_font(Text *text, DiaFont *font)
{
  DiaFont *old_font = text->font;
  int i;

  text->font = dia_font_ref(font);
  dia_font_unref(old_font);

  for (i = 0; i < text->numlines; i++) {
    text_line_set_font(text->lines[i], font);
  }
  
  calc_width(text);
  calc_ascent_descent(text);
}

void
text_set_position(Text *text, Point *pos)
{
  text->position = *pos;
}

void
text_set_color(Text *text, Color *col)
{
  text->color = *col;
}

void
text_set_alignment(Text *text, Alignment align)
{
  text->alignment = align;
}

void
text_calc_boundingbox(Text *text, Rectangle *box)
{
  calc_width(text);
  calc_ascent_descent(text);

  if (box == NULL) return; /* For those who just want the text info
			      updated */
  box->left = text->position.x;
  switch (text->alignment) {
  case ALIGN_LEFT:
    break;
  case ALIGN_CENTER:
    box->left -= text->max_width / 2.0;
    break;
  case ALIGN_RIGHT:
    box->left -= text->max_width;
    break;
  }

  box->right = box->left + text->max_width;
  
  box->top = text->position.y - text->ascent;
#if 0
  box->bottom = box->top + text->height*text->numlines + text->descent;
#else
  /* why should we add one descent? isn't ascent+descent~=height? */
  box->bottom = box->top + (text->ascent+text->descent+text->height*(text->numlines-1));
#endif
  if (text->focus.has_focus) {
    real height = text->ascent + text->descent;
    if (text->cursor_pos == 0) {
      /* Half the cursor width */
      box->left -= height/(CURSOR_HEIGHT_RATIO*2);
    } else {
      /* Half the cursor width. Assume that
	 if it isn't at position zero, it might be 
	 at the last position possible. */
      box->right += height/(CURSOR_HEIGHT_RATIO*2);
    }
   
    /* Account for the size of the cursor top and bottom */
    box->top -= height/(CURSOR_HEIGHT_RATIO*2);
    box->bottom += height/CURSOR_HEIGHT_RATIO;
  }
}

char *
text_get_string_copy(Text *text)
{
  int num,i;
  char *str;
  
  num = 0;
  for (i=0;i<text->numlines;i++) {
    /* This is for allocation, so it should not use g_utf8_strlen() */
    num += strlen(text_get_line(text, i))+1; 
  }

  str = g_malloc(num);

  *str = 0;
  
  for (i=0;i<text->numlines;i++) {
    strcat(str, text_get_line(text, i));
    if (i != (text->numlines-1)) {
      strcat(str, "\n");
    }
  }
  
  return str;
}

real
text_distance_from(Text *text, Point *point)
{
  real dx, dy;
  real topy, bottomy;
  real left, right;
  int line;
  
  topy = text->position.y - text->ascent;
  bottomy = topy + text->height*text->numlines;
  if (point->y <= topy) {
    dy = topy - point->y;
    line = 0;
  } else if (point->y >= bottomy) {
    dy = point->y - bottomy;
    line = text->numlines - 1;
  } else {
    dy = 0.0;
    line = (int) floor( (point->y - topy) / text->height );
  }

  left = text->position.x;
  switch (text->alignment) {
  case ALIGN_LEFT:
    break;
  case ALIGN_CENTER:
    left -= text_get_line_width(text, line) / 2.0;
    break;
  case ALIGN_RIGHT:
    left -= text_get_line_width(text, line);
    break;
  }
  right = left + text_get_line_width(text, line);

  if (point->x <= left) {
    dx = left - point->x;
  } else if (point->x >= right) {
    dx = point->x - right;
  } else {
    dx = 0.0;
  }

  return dx + dy;
}

void
text_draw(Text *text, DiaRenderer *renderer)
{
  DIA_RENDERER_GET_CLASS(renderer)->draw_text(renderer, text);

  if ((renderer->is_interactive) && (text->focus.has_focus)) {
    real curs_x, curs_y;
    real str_width_first;
    real str_width_whole;
    Point p1, p2;
    real height = text->ascent+text->descent;
    curs_y = text->position.y - text->ascent + text->cursor_row*text->height; 

    DIA_RENDERER_GET_CLASS(renderer)->set_font(renderer, text->font, text->height);

    str_width_first =
      DIA_RENDERER_GET_CLASS(renderer)->get_text_width(renderer,
						       text_get_line(text, text->cursor_row),
						       text->cursor_pos);
    str_width_whole =
      DIA_RENDERER_GET_CLASS(renderer)->get_text_width(renderer,
						       text_get_line(text, text->cursor_row),
						       text_get_line_strlen(text, text->cursor_row));
    curs_x = text->position.x + str_width_first;

    switch (text->alignment) {
    case ALIGN_LEFT:
      break;
    case ALIGN_CENTER:
      curs_x -= str_width_whole / 2.0;
      break;
    case ALIGN_RIGHT:
      curs_x -= str_width_whole;
      break;
    }

    p1.x = curs_x;
    p1.y = curs_y;
    p2.x = curs_x;
    p2.y = curs_y + height;
    
    DIA_RENDERER_GET_CLASS(renderer)->set_linestyle(renderer, LINESTYLE_SOLID);
    DIA_RENDERER_GET_CLASS(renderer)->set_linewidth(renderer, height/CURSOR_HEIGHT_RATIO);
    DIA_RENDERER_GET_CLASS(renderer)->draw_line(renderer, &p1, &p2, &color_black);
  }
}

void
text_grab_focus(Text *text, DiaObject *object)
{
  text->focus.obj = object;
  request_focus(&text->focus);
}

void
text_set_cursor_at_end( Text* text )
{
  text->cursor_row = text->numlines - 1 ;
  text->cursor_pos = text_get_line_strlen(text, text->cursor_row) ;
}

/* The renderer is only used to determine where the click is, so is not
 * required when no point is given. */
void
text_set_cursor(Text *text, Point *clicked_point,
		DiaRenderer *renderer)
{
  real str_width_whole;
  real str_width_first;
  real top;
  real start_x;
  int row;
  int i;

  if (clicked_point != NULL) {
    top = text->position.y - text->ascent;
  
    row = (int)floor((clicked_point->y - top) / text->height);

    if (row < 0)
      row = 0;

    if (row >= text->numlines)
      row = text->numlines - 1;
    
    text->cursor_row = row;
    text->cursor_pos = 0;

    if (!renderer->is_interactive) {
      message_error("Internal error: Select gives non interactive renderer!\n"
		    "val: %d\n", renderer->is_interactive);
      return;
    }


    DIA_RENDERER_GET_CLASS(renderer)->set_font(renderer, text->font, text->height);
    str_width_whole =
      DIA_RENDERER_GET_CLASS(renderer)->get_text_width(renderer,
						       text_get_line(text, row),
						       text_get_line_strlen(text, row));
    start_x = text->position.x;
    switch (text->alignment) {
    case ALIGN_LEFT:
      break;
    case ALIGN_CENTER:
      start_x -= str_width_whole / 2.0;
      break;
    case ALIGN_RIGHT:
      start_x -= str_width_whole;
      break;
    }

    /* Do an ugly linear search for the cursor index:
       TODO: Change to binary search */
  
    for (i=0;i<=text_get_line_strlen(text, row);i++) {
      str_width_first =
	DIA_RENDERER_GET_CLASS(renderer)->get_text_width(renderer, text_get_line(text, row), i);
      if (clicked_point->x - start_x >= str_width_first) {
	text->cursor_pos = i;
      } else {
	return;
      }
    }
    text->cursor_pos = text_get_line_strlen(text, row);
  } else {
    /* No clicked point, leave cursor where it is */
  }
}

static void
text_join_lines(Text *text, int first_line)
{
  gchar *combined_line;
  int len1;
  
  len1 = text_get_line_strlen(text, first_line);

  combined_line = g_strconcat(text_get_line(text, first_line), 
			      text_get_line(text, first_line + 1), NULL);
  text_delete_line(text, first_line);
  text_set_line_text(text, first_line, combined_line);
  g_free(combined_line);

  text->max_width = MAX(text->max_width, text_get_line_width(text, first_line));

  text->cursor_row = first_line;
  text->cursor_pos = len1;
}

static void
text_delete_forward(Text *text)
{
  int row;
  int i;
  real width;
  gchar *line;
  gchar *utf8_before, *utf8_after;
  gchar *str1, *str;
  
  row = text->cursor_row;
  
  if (text->cursor_pos >= text_get_line_strlen(text, row)) {
    if (row + 1 < text->numlines)
      text_join_lines(text, row);
    return;
  }

  line = text_get_line(text, row);
  utf8_before = g_utf8_offset_to_pointer(line, (glong)(text->cursor_pos));
  utf8_after = g_utf8_offset_to_pointer(utf8_before, 1);
  str1 = g_strndup(line, utf8_before - line);
  str = g_strconcat(str1, utf8_after, NULL);
  text_set_line_text(text, row, str);
  g_free(str1);
  g_free(str);

  if (text->cursor_pos > text_get_line_strlen(text, text->cursor_row))
    text->cursor_pos = text_get_line_strlen(text, text->cursor_row);

  width = 0.0;
  for (i = 0; i < text->numlines; i++) {
    width = MAX(width, text_get_line_width(text, i));
  }
  text->max_width = width;
}

static void
text_delete_backward(Text *text)
{
  int row;
  int i;
  real width;
  gchar *line;
  gchar *utf8_before, *utf8_after;
  gchar *str1, *str;
  
  row = text->cursor_row;
  
  if (text->cursor_pos <= 0) {
    if (row > 0)
      text_join_lines(text, row-1);
    return;
  }

  line = text_get_line(text, row);
  utf8_before = g_utf8_offset_to_pointer(line, (glong)(text->cursor_pos - 1));
  utf8_after = g_utf8_offset_to_pointer(utf8_before, 1);
  str1 = g_strndup(line, utf8_before - line);
  str = g_strconcat(str1, utf8_after, NULL);
  text_set_line_text(text, row, str);
  g_free(str);
  g_free(str1);

  text->cursor_pos --;
  if (text->cursor_pos > text_get_line_strlen(text, text->cursor_row))
    text->cursor_pos = text_get_line_strlen(text, text->cursor_row);

  width = 0.0;
  for (i = 0; i < text->numlines; i++) {
    width = MAX(width, text_get_line_width(text, i));
  }
  text->max_width = width;
}

static void
text_split_line(Text *text)
{
  int i;
  char *line;
  real width;
  gchar *utf8_before;
  gchar *str1, *str2;
  
  /* Split the lines at cursor_pos */
  line = text_get_line(text, text->cursor_row);
  text_insert_line(text, text->cursor_row);
  utf8_before = g_utf8_offset_to_pointer(line, (glong)(text->cursor_pos));
  str1 = g_strndup(line, utf8_before - line);
  str2 = g_strdup(utf8_before); /* Must copy before dealloc */
  text_set_line_text(text, text->cursor_row, str1);
  text_set_line_text(text, text->cursor_row + 1, str2);
  g_free(str2);
  g_free(str1);

  text->cursor_row ++;
  text->cursor_pos = 0;

  width = 0.0;
  for (i=0;i<text->numlines;i++) {
    width = MAX(width, text_get_line_width(text, i));
  }
  text->max_width = width;
}

static void
text_insert_char(Text *text, gunichar c)
{
  gchar ch[7];
  int unilen;
  int row;
  gchar *line, *str;
  gchar *utf8_before;
  gchar *str1;

  /* Make a string of the the char */
  unilen = g_unichar_to_utf8 (c, ch);
  ch[unilen] = 0;
  
  row = text->cursor_row;

  /* Copy the before and after parts with the new char in between */
  line = text_get_line(text, row);
  utf8_before = g_utf8_offset_to_pointer(line, (glong)(text->cursor_pos));
  str1 = g_strndup(line, utf8_before - line);
  str = g_strconcat(str1, ch, utf8_before, NULL);
  text_set_line_text(text, row, str);
  g_free(str);
  g_free(str1);

  text->cursor_pos++;
  text->max_width = MAX(text->max_width, text_get_line_width(text, row));
}

gboolean
text_delete_key_handler(Focus *focus, ObjectChange ** change)
{
  Text *text;
  int row, i;
  const char *utf;
  gunichar c;

  text = (Text *)focus->user_data;
  row = text->cursor_row;
  if (text->cursor_pos >= text_get_line_strlen(text, row)) {
    if (row+1 < text->numlines) {
      *change = text_create_change(text, TYPE_JOIN_ROW, 'Q',
				   text->cursor_pos, row);
    } else {
      return FALSE;
    }
  } else {
    utf = text_get_line(text, row);
    for (i = 0; i < text->cursor_pos; i++)
      utf = g_utf8_next_char (utf);
    c = g_utf8_get_char (utf);
    *change = text_create_change (text, TYPE_DELETE_FORWARD, c,
				  text->cursor_pos, text->cursor_row);
  }
  text_delete_forward(text);
  return TRUE;;
}

static int
text_key_event(Focus *focus, guint keyval, const gchar *str, int strlen,
               ObjectChange **change)
{
  Text *text;
  int return_val = FALSE;
  int row, i;
  const char *utf;
  gunichar c;

  *change = NULL;
  
  text = (Text *)focus->user_data;

  switch(keyval) {
      case GDK_Up:
        text->cursor_row--;
        if (text->cursor_row<0)
          text->cursor_row = 0;

        if (text->cursor_pos > text_get_line_strlen(text, text->cursor_row))
          text->cursor_pos = text_get_line_strlen(text, text->cursor_row);

        break;
      case GDK_Down:
        text->cursor_row++;
        if (text->cursor_row >= text->numlines)
          text->cursor_row = text->numlines - 1;

        if (text->cursor_pos > text_get_line_strlen(text, text->cursor_row))
          text->cursor_pos = text_get_line_strlen(text, text->cursor_row);
    
        break;
      case GDK_Left:
        text->cursor_pos--;
        if (text->cursor_pos<0)
          text->cursor_pos = 0;
        break;
      case GDK_Right:
        text->cursor_pos++;
        if (text->cursor_pos > text_get_line_strlen(text, text->cursor_row))
          text->cursor_pos = text_get_line_strlen(text, text->cursor_row);
        break;
      case GDK_Home:
        text->cursor_pos = 0;
        break;
      case GDK_End:
        text->cursor_pos = text_get_line_strlen(text, text->cursor_row);
        break;
      case GDK_Delete:
        return_val = text_delete_key_handler(focus, change);
        break;
      case GDK_BackSpace:
        return_val = TRUE;
        row = text->cursor_row;
        if (text->cursor_pos <= 0) {
          if (row > 0) {
            *change = text_create_change(text, TYPE_JOIN_ROW, 'Q',
                                         text_get_line_strlen(text, row-1), row-1);
          } else {
            return_val = FALSE;
            break;
          }
        } else {
          utf = text_get_line(text, row);
          for (i = 0; i < (text->cursor_pos - 1); i++)
            utf = g_utf8_next_char (utf);
          c = g_utf8_get_char (utf);
          *change = text_create_change (text, TYPE_DELETE_BACKWARD, c,
                                        text->cursor_pos - 1,
                                        text->cursor_row);
        }
        text_delete_backward(text);
        break;
      case GDK_Return:
      case GDK_KP_Enter:
        return_val = TRUE;
        *change = text_create_change(text, TYPE_SPLIT_ROW, 'Q',
                                     text->cursor_pos, text->cursor_row);
        text_split_line(text);
        break;
      case GDK_Shift_L:
      case GDK_Shift_R:
      case GDK_Control_L:
      case GDK_Control_R:
      case GDK_Alt_L:
      case GDK_Alt_R:
      case GDK_Meta_L:
      case GDK_Meta_R:
        return_val = FALSE; /* no text change for modifiers */
        break;
      default:
        if (str || (strlen>0)) {
          if (strlen == 1 && *str == '\r')
            break; /* avoid putting junk into our string */
          return_val = TRUE;
          utf = str;
          for (utf = str; utf && *utf && strlen > 0 ;
	       utf = g_utf8_next_char (utf), strlen--) {
            c = g_utf8_get_char (utf);
            
            *change = text_create_change (text, TYPE_INSERT_CHAR, c,
                                          text->cursor_pos, text->cursor_row);
            text_insert_char (text, c);
          }
        }
        break;
  }  
  
  return return_val;
}

int text_is_empty(Text *text)
{
  int i;
  for (i = 0; i < text->numlines; i++) {
    if (text_get_line_strlen(text, i) != 0) {
      return FALSE;
    }
  }
  return TRUE;
}

int
text_delete_all(Text *text, ObjectChange **change)
{
  if (!text_is_empty(text)) {
    *change = text_create_change(text, TYPE_DELETE_ALL,
				 0, text->cursor_pos, text->cursor_row);
    
    text_set_string(text, "");
    calc_ascent_descent(text);
    return TRUE;
  }
  return FALSE;
}

void
data_add_text(AttributeNode attr, Text *text)
{
  DataNode composite;
  char *str;

  composite = data_add_composite(attr, "text");

  str = text_get_string_copy(text);
  data_add_string(composite_add_attribute(composite, "string"),
		  str);
  g_free(str);
  data_add_font(composite_add_attribute(composite, "font"),
		text->font);
  data_add_real(composite_add_attribute(composite, "height"),
		text->height);
  data_add_point(composite_add_attribute(composite, "pos"),
		    &text->position);
  data_add_color(composite_add_attribute(composite, "color"),
		 &text->color);
  data_add_enum(composite_add_attribute(composite, "alignment"),
		text->alignment);
}


Text *
data_text(AttributeNode text_attr)
{
  char *string = NULL;
  DiaFont *font;
  real height;
  Point pos = {0.0, 0.0};
  Color col;
  Alignment align;
  AttributeNode attr;
  DataNode composite_node;
  Text *text;

  composite_node = attribute_first_data(text_attr);

  attr = composite_find_attribute(text_attr, "string");
  if (attr != NULL)
    string = data_string(attribute_first_data(attr));

  height = 1.0;
  attr = composite_find_attribute(text_attr, "height");
  if (attr != NULL)
    height = data_real(attribute_first_data(attr));

  attr = composite_find_attribute(text_attr, "font");
  if (attr != NULL) {
    font = data_font(attribute_first_data(attr));
  } else {
    font = dia_font_new_from_style(DIA_FONT_SANS,1.0);
  }
  
  attr = composite_find_attribute(text_attr, "pos");
  if (attr != NULL)
    data_point(attribute_first_data(attr), &pos);

  col = color_black;
  attr = composite_find_attribute(text_attr, "color");
  if (attr != NULL)
    data_color(attribute_first_data(attr), &col);

  align = ALIGN_LEFT;
  attr = composite_find_attribute(text_attr, "alignment");
  if (attr != NULL)
    align = data_enum(attribute_first_data(attr));
  
  text = new_text(string ? string : "", font, height, &pos, &col, align);
  if (font) dia_font_unref(font);
  if (string) g_free(string);
  return text;
}

void
text_get_attributes(Text *text, TextAttributes *attr)
{    
  DiaFont *old_font;
  old_font = attr->font;
  attr->font = dia_font_ref(text->font);
  if (old_font != NULL) dia_font_unref(old_font);
  attr->height = text->height;
  attr->position = text->position;
  attr->color = text->color;
  attr->alignment = text->alignment;
}

void
text_set_attributes(Text *text, TextAttributes *attr)
{
  if (text->font != attr->font) {
    text_set_font(text, attr->font);
  }
  text_set_height(text, attr->height);
  text->position = attr->position;
  text->color = attr->color;
  text->alignment = attr->alignment;
}

static void
text_change_apply(struct TextObjectChange *change, DiaObject *obj)
{
  Text *text = change->text;
  switch (change->type) {
  case TYPE_INSERT_CHAR:
    text->cursor_pos = change->pos;
    text->cursor_row = change->row;
    text_insert_char(text, change->ch);
    break;
  case TYPE_DELETE_BACKWARD:
    text->cursor_pos = change->pos+1;
    text->cursor_row = change->row;
    text_delete_backward(text);
    break;
  case TYPE_DELETE_FORWARD:
    text->cursor_pos = change->pos;
    text->cursor_row = change->row;
    text_delete_forward(text);
    break;
  case TYPE_SPLIT_ROW:
    text->cursor_pos = change->pos;
    text->cursor_row = change->row;
    text_split_line(text);
    break;
  case TYPE_JOIN_ROW:
    text_join_lines(text, change->row);
    break;
  case TYPE_DELETE_ALL:
    set_string(text, "");
    text->cursor_pos = 0;
    text->cursor_row = 0;
    break;
  }
}

static void
text_change_revert(struct TextObjectChange *change, DiaObject *obj)
{
  Text *text = change->text;
  switch (change->type) {
  case TYPE_INSERT_CHAR:
    text->cursor_pos = change->pos;
    text->cursor_row = change->row;
    text_delete_forward(text);
    break;
  case TYPE_DELETE_BACKWARD:
    text->cursor_pos = change->pos;
    text->cursor_row = change->row;
    text_insert_char(text, change->ch);
    break;
  case TYPE_DELETE_FORWARD:
    text->cursor_pos = change->pos;
    text->cursor_row = change->row;
    text_insert_char(text, change->ch);
    text->cursor_pos = change->pos;
    text->cursor_row = change->row;
    break;
  case TYPE_SPLIT_ROW:
    text_join_lines(text, change->row);
    break;
  case TYPE_JOIN_ROW:
    text->cursor_pos = change->pos;
    text->cursor_row = change->row;
    text_split_line(text);
    break;
  case TYPE_DELETE_ALL:
    set_string(text, change->str);
    text->cursor_pos = change->pos;
    text->cursor_row = change->row;
    break;
  }
}

static void
text_change_free(struct TextObjectChange *change) {
  g_free(change->str);
}

static ObjectChange *
text_create_change(Text *text, enum change_type type,
		   gunichar ch, int pos, int row)
{
  struct TextObjectChange *change;

  change = g_new0(struct TextObjectChange, 1);

  change->obj_change.apply = (ObjectChangeApplyFunc) text_change_apply;
  change->obj_change.revert = (ObjectChangeRevertFunc) text_change_revert;
  change->obj_change.free = (ObjectChangeFreeFunc) text_change_free;

  change->text = text;
  change->type = type;
  change->ch = ch;
  change->pos = pos;
  change->row = row;
  if (type == TYPE_DELETE_ALL)
    change->str = text_get_string_copy(text);
  else
    change->str = NULL;
  return (ObjectChange *)change;
}

gboolean 
apply_textattr_properties(GPtrArray *props,
                          Text *text, const gchar *textname,
                          TextAttributes *attrs)
{
  TextProperty *textprop = 
    (TextProperty *)find_prop_by_name_and_type(props,textname,PROP_TYPE_TEXT);

  if ((!textprop) || 
      ((textprop->common.experience & (PXP_LOADED|PXP_SFO))==0 )) {
    /* most likely we're called after the dialog box has been applied */
    text_set_attributes(text,attrs);
    return TRUE; 
  }
  return FALSE;
}

gboolean 
apply_textstr_properties(GPtrArray *props,
                         Text *text, const gchar *textname,
                         const gchar *str)
{
  TextProperty *textprop = 
    (TextProperty *)find_prop_by_name_and_type(props,textname,PROP_TYPE_TEXT);

  if ((!textprop) || 
      ((textprop->common.experience & (PXP_LOADED|PXP_SFO))==0 )) {
    /* most likely we're called after the dialog box has been applied */
    text_set_string(text,str);
    return TRUE; 
  }
  return FALSE;
}

