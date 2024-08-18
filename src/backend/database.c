#include <stdio.h>
#include "database.h"

sqlite3 *database;

int
open_database (const char *database_path)
{
	if (database != NULL)
	{
		return -1;	
	}
	int return_code = sqlite3_open (database_path, &database);
	return return_code;
}

int
close_database ()
{
	if (database == NULL)
	{
		return 1;
	}
	sqlite3_close (database);
	database = NULL;
	return 0;
}
