#include <stdio.h>
#include "database.h"

sqlite3 *database;

int db_open(const char *db_path)
{
	if (database != NULL)
	{
		return -1;	
	}
	int return_code = sqlite3_open(db_path, &database);
	return return_code;
}

int db_close()
{
	if (database == NULL)
	{
		return 1;
	}
	sqlite3_close (database);
	database = NULL;
	return 0;
}

sqlite3 *database_get()
{
	return database;
}

