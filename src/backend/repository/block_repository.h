#pragma once
#include "../entity/block.h"
#include <glib.h>
#include <sqlite3.h>

// *********************************************************************
// * Interface
// *********************************************************************

sqlite3_int64     
block_repository_create ();

Block *  
block_repository_find_by_id (sqlite3_int64 id);

GArray *
block_repository_find_all ();

