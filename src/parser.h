#pragma once
#include<gtk/gtk.h>

typedef struct
{
	GtkTextMark *start;
	GtkTextMark *end;
} Token;

GList* tokenize(GtkTextBuffer *buffer);
