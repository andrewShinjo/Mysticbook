#pragma once

#include <gtk/gtk.h>

void editor_apply_tag_to_line(GtkTextBuffer *buffer, GtkTextTag *tag, int line_number);
void editor_remove_all_tags_from_line(GtkTextBuffer *buffer, int line_number);
void editor_remove_tag_from_line(GtkTextBuffer *buffer, GtkTextTag *tag, int line_number);
int editor_get_cursor_position_line_number (GtkTextBuffer *buffer);
int editor_get_heading_level(const gchar *heading);
int editor_get_line_count(GtkTextBuffer *buffer);
gchar* editor_get_text_at_line(GtkTextBuffer *buffer, int line_number);
gboolean editor_is_line_heading(gchar *line);
void editor_print_buffer(GtkTextBuffer *buffer);