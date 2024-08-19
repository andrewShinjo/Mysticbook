#include <stdio.h>
#include "../database.h"
#include "document_repository.h"

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
	GArray *documents = (GArray *) data;
	for (int i = 0; i < column_count; i++)
	{
		DocumentEntity d;
		d.id = g_ascii_strtoll (column_text[i], 0, 10);
		g_array_append_val (documents, d);
	}
	return 0;
}

// *********************************************************************
// * Interface
// *********************************************************************

sqlite3_int64
document_repository_create ()
{
	sqlite3 *database = database_get ();
	const char *sql = "INSERT INTO documents DEFAULT VALUES;";
	int return_code = sqlite3_exec (database, sql, 0, 0, 0);
	if(return_code != 0)
	{
		return -1;
	}
	return sqlite3_last_insert_rowid (database);
}

GArray *
document_repository_find_all ()
{
	sqlite3 *database = database_get ();
	const char *sql = "SELECT * FROM documents;";
	GArray *documents = g_array_new (FALSE, FALSE, sizeof (DocumentEntity));
	int return_code = sqlite3_exec (
		database, 
		sql, 
		find_all_callback, 
		documents,
		NULL
	);
	return documents;
}
