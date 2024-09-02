#pragma once
#include <sqlite3.h>

typedef struct
{
	sqlite3_int64       id;	
	sqlite3_int64       creation_time;
	sqlite3_int64       modification_time;
	unsigned char       *content;
	int                 is_document;
} Block;
