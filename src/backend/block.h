#pragma once
#include <glib.h>

typedef struct
{
	gint64 id;	
	gint64 creation_time;
  gint64 is_document;
	gint64 modification_time;
  gint64 position;
  gint64 parent_id;
  gint64 expanded;
	const unsigned char *content;
} Block;
