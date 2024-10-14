#include <stdio.h>
#include "database.h"

sqlite3 *db;

int db_open(const char *db_path)
{
	if(db != NULL)
	{
		return -1;	
	}
	int rc = sqlite3_open(db_path, &db);
	return rc;
}

int db_close()
{
	if(db != NULL)
	{
    sqlite3_close(db);
    db = NULL;
	}
	return 0;
}
