#include <stdio.h>
#include "../database.h"
#include "../entity/block.h"
#include "./block_repository.h"

// *********************************************************************
// * Private
// *********************************************************************

int
find_all_callback (
	void *data, 
	int column_count,
	char **column_text,
	char **column_name
)
{
	GArray *blocks = (GArray *) data;
	for (gint i = 0; i < column_count; i++)
	{
		int compare = g_strcmp0 (column_name[i], "id");
		if (compare != 0)
		{
			continue;
		}
		Block b;
		gchar *id_text = column_text[i];
		b.id = g_ascii_strtoull (id_text, NULL, 10);
		g_array_append_val (blocks, b);
	}
	return 0;
}

// *********************************************************************
// * Public
// *********************************************************************

sqlite3_int64
block_repository_create ()
{
	sqlite3 *database = database_get ();
	const char *sql = "INSERT INTO blocks(creation_time, modification_time, content, is_document) VALUES(0, 0, 'Untitled', 1);";
	int return_code = sqlite3_exec (database, sql, 0, 0, 0);
	if(return_code != 0)
	{
		return -1;
	}
	return sqlite3_last_insert_rowid (database);
}

GArray *
block_repository_find_all ()
{
	sqlite3 *database = database_get ();
	const char *sql = "SELECT * FROM blocks;";
	GArray *blocks = g_array_new (FALSE, FALSE, sizeof (Block));
	int return_code = sqlite3_exec (
		database, 
		sql, 
		find_all_callback, 
		blocks,
		NULL
	);
	return blocks;
}