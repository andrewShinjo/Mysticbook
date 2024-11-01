#pragma once
#include <glib.h>

typedef struct
{
	gint64 id;	
	gint64 creation_time;
	gint64 modification_time;
	const unsigned char *content;
	int is_document;
} Block;
