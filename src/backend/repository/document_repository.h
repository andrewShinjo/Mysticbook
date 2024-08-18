#include <sqlite3.h>
#include "../entity/document_entity.h"

// *********************************************************************
// * Interface
// *********************************************************************

sqlite3_int64     document_repository_create     ();
DocumentEntity *  document_repository_find_by_id (sqlite3_int64 id);
DocumentEntity ** document_repository_find_all   ();

