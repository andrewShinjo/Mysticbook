#pragma once
#include <sqlite3.h>

// **********************************************************************
// Interface
// **********************************************************************

int           database_open  (const char *database_path);
int           database_close ();
sqlite3 *     database_get   ();
