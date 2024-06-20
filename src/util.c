#include "util.h"
#include<string.h>

// PUBLIC

void editor_apply_tag_to_line(GtkTextBuffer *buffer, GtkTextTag *tag, int line_number)
{
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_iter_set_line(&start, line_number);
    end = start;

    if(!gtk_text_iter_ends_line(&end))
    {
        gtk_text_iter_forward_to_line_end(&end);
    }

    gtk_text_buffer_apply_tag(buffer, tag, &start, &end);
}

void editor_remove_all_tags_from_line(GtkTextBuffer *buffer, int line_number)
{
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_iter_set_line(&start, line_number);
    end = start;

    if(!gtk_text_iter_ends_line(&end))
    {
        gtk_text_iter_forward_to_line_end(&end);
        gtk_text_iter_forward_char(&end);
    }

    gtk_text_buffer_remove_all_tags(buffer, &start, &end);
}

void editor_remove_tag_from_line(GtkTextBuffer *buffer, GtkTextTag *tag, int line_number)
{
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_iter_set_line(&start, line_number);
    end = start;

    if(!gtk_text_iter_ends_line(&end))
    {
        gtk_text_iter_forward_to_line_end(&end);
        gtk_text_iter_forward_char(&end);
    }

    gtk_text_buffer_remove_tag(buffer, tag, &start, &end);
}

int editor_get_cursor_position_line_number(GtkTextBuffer *buffer)
{
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));
    return gtk_text_iter_get_line(&iter);
}

int editor_get_heading_level(const gchar *heading)
{
    int level = 0;
    while(heading[level] == '*') 
    {
        level++;
    }
    return level;
}

int editor_get_line_count(GtkTextBuffer *buffer)
{
    return gtk_text_buffer_get_line_count(buffer);
}

gchar* editor_get_text_at_line(GtkTextBuffer *buffer, int line_number)
{
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_iter_set_line(&start, line_number);
    end = start;

    if(!gtk_text_iter_ends_line(&end))
    {
        gtk_text_iter_forward_to_line_end(&end);
    }
    return gtk_text_iter_get_text(&start, &end);
}

gboolean editor_is_line_heading(gchar *line)
{
    size_t length = strlen(line);

    if(length < 2 || line[0] != '*')
    {
        return false;
    }

    for(int i = 1; i < length; i++)
    {
        if(line[i] == ' ')
        {
            return true;
        }
        if(line[i] != '*')
        {
            return false;;
        }
    }

    return false;
}

void editor_print_buffer(GtkTextBuffer *buffer)
{

    g_print("\e[1;1H\e[2J");

    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);
    GtkTextTag *heading_tag = gtk_text_tag_table_lookup(tag_table, "heading");
    
    GtkTextIter iter, cursor;
    gtk_text_buffer_get_start_iter(buffer, &iter);
    gtk_text_buffer_get_iter_at_mark(buffer, &cursor, gtk_text_buffer_get_insert(buffer));

    int line_number = 0;
    int line_count = editor_get_line_count(buffer);

    g_print("Line count: %d\n\n", line_count);
    g_print("%d: ", line_number);

    while(!gtk_text_iter_is_end(&iter))
    {

        gunichar c = gtk_text_iter_get_char(&iter);

        if(gtk_text_iter_equal(&cursor, &iter))
        {
            g_print("\e[34m");
            g_print("|");
            g_print("\e[0m");
        }

        if(c == '\n')
        {
            line_number++;
            if(gtk_text_iter_has_tag(&iter, heading_tag))
            {
                g_print("\e[43m\e[30m");
                g_print("\\n");
                g_print("\e[0m");
            }
            else 
            {
                g_print("\\n");
            }
            
            g_print("\n%d: ", line_number);
        }
        else
        {
            if(gtk_text_iter_has_tag(&iter, heading_tag))
            {
                g_print("\e[43m\e[30m");
            }
            g_print("%c", c);
            if(gtk_text_iter_has_tag(&iter, heading_tag))
            {
                g_print("\e[0m");
            }
        }

        gtk_text_iter_forward_char(&iter);
    }

    if(gtk_text_iter_is_end(&cursor))
    {
        g_print("\e[34m");
        g_print("|");
        g_print("\e[0m");
    }

    // Print end of buffer in red
    {
        g_print("\e[41m");
        g_print(" ");
        printf("\e[0m");
    }   

    g_print("\n\n");
}
