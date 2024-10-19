#pragma once
#include <glib.h>
#include <sqlite3.h>

typedef struct
{
	sqlite3_int64       id;	
	sqlite3_int64       creation_time;
	sqlite3_int64       modification_time;
	const unsigned char *content;
	int                 is_document;
} Block;

int           block_delete_by_id(sqlite3_int64 id);
void          block_find_by_id(sqlite3_int64 id, Block *b);
GArray*       block_get_all();
GArray*       block_get_all_ids();
sqlite3_int64 block_new();
int           block_update_content(sqlite3_int64 id, gchar *content);
