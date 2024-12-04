#include "./parser.h"

// Token = word or whitespace
GList* tokenize(GtkTextBuffer *buffer)
{
	g_print("Tokenize:\n");

	GList *start_iter_list = NULL;
	GList *end_iter_list = NULL;

	GtkTextIter iter;
	gtk_text_buffer_get_start_iter(buffer, &iter);

	while(!gtk_text_iter_is_end(&iter))
	{
		gunichar c = gtk_text_iter_get_char(&iter);

		if(c == ' ')
		{
			GtkTextIter *copy = gtk_text_iter_copy(&iter);
			start_iter_list = g_list_append(start_iter_list, copy);
			gtk_text_iter_forward_char(&iter);
			GtkTextIter *copy2 = gtk_text_iter_copy(&iter);
			end_iter_list= g_list_append(end_iter_list, copy2);
		}
		else if(c == '\n')
		{
			GtkTextIter *copy = gtk_text_iter_copy(&iter);
			start_iter_list = g_list_append(start_iter_list, copy);
			gtk_text_iter_forward_char(&iter);
			GtkTextIter *copy2 = gtk_text_iter_copy(&iter);
			end_iter_list= g_list_append(end_iter_list, copy2);
		}
		else
		{
			GtkTextIter word_end = iter;
			gtk_text_iter_forward_char(&word_end);
			gunichar last_char = gtk_text_iter_get_char(&word_end);
			while(last_char != ' ' && last_char != '\n' && !gtk_text_iter_is_end(&word_end))
			{
				gtk_text_iter_forward_char(&word_end);
				last_char = gtk_text_iter_get_char(&word_end);
			}

			GtkTextIter *copy1 = gtk_text_iter_copy(&iter);
			GtkTextIter *copy2 = gtk_text_iter_copy(&word_end);
			start_iter_list = g_list_append(start_iter_list, copy1);
			end_iter_list = g_list_append(end_iter_list, copy2);

			iter = word_end;
		}
	}

	GList *start_curr = start_iter_list;
	GList *end_curr = end_iter_list;

	while(start_curr != NULL && end_curr != NULL)
	{
		GtkTextIter *iter1 = (GtkTextIter*) start_curr->data;
		GtkTextIter *iter2 = (GtkTextIter*) end_curr->data;
		int start_pos = gtk_text_iter_get_offset(iter1);
		int end_pos = gtk_text_iter_get_offset(iter2);
		gchar *text = gtk_text_buffer_get_text(buffer, iter1, iter2, TRUE);
		g_print("Pos: [%d, %d], ", start_pos, end_pos);
		if(g_strcmp0(text, " ") == 0) 
		{
			g_print("Token: WHITESPACE\n");
		}
		else if(g_strcmp0(text, "\n") == 0)
		{
			g_print("Token: NEWLINE\n");
		}
		else
		{
			g_print("Token: %s\n", text);
		}
		start_curr = start_curr->next;
		end_curr = end_curr->next;
	}

	return NULL;
}
