#include "../database.h"
#include "document_repository.h"

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
