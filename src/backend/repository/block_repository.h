#pragma once
#include "../entity/block.h"
#include <glib.h>
#include <sqlite3.h>

sqlite3_int64     
block_repository_create();

Block *  
block_repository_find_by_id(sqlite3_int64 id);

GArray *
block_repository_find_all();

int
block_repository_delete_by_id(sqlite3_int64 id);

gboolean
update_block(sqlite3_int64 id, gchar *text);
