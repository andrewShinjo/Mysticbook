#include "./parser.h"

// Token = word or whitespace
GList* tokenize(GtkTextBuffer *buffer)
{
	g_print("Tokenize:\n");
	GtkTextIter iter;
	gtk_text_buffer_get_start_iter(buffer, &iter);

	while(!gtk_text_iter_is_end(&iter))
	{
		gunichar c = gtk_text_iter_get_char(&iter);

		if(c == ' ')
		{
			g_print("Char: WHITESPACE\n");
		}
		else if(c == '\n')
		{
			g_print("Char: NEWLINE\n");
		}
		else
		{
			g_print("Char: %c\n", c);
		}

		gtk_text_iter_forward_char(&iter);
	}
	return NULL;
}
