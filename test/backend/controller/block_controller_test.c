#include <assert.h>
#include <glib.h>
#include "../../../src/backend/controller/block_controller.h"
#include "../../../src/backend/database/database.h"
#include "../../../src/backend/repository/block_repository.h"

/* block_controller_get_document_ids() tests */

// Test empty blocks table.
static void block_controller_get_document_ids_test1()
{
  clear_blocks_table();
  GArray* document_ids = block_controller_get_document_ids();
  guint length = document_ids->len;
  g_free(document_ids);
  assert(length == 0);  
  g_print("block_controller_get_document_ids_test1 OK.\n");
}

// 2 document blocks, and 3 non-document blocks.
// Should return 2 document ids.
static void block_controller_get_document_ids_test2()
{
  clear_blocks_table();
  block_repository_save(0, 1, 0, 0, 0, NULL);
  block_repository_save(0, 1, 0, 0, 0, NULL);
  block_repository_save(0, 0, 0, 0, 0, NULL);
  block_repository_save(0, 0, 0, 0, 0, NULL);
  block_repository_save(0, 0, 0, 0, 0, NULL);
  GArray *document_ids = block_controller_get_document_ids();
  guint length = document_ids->len;
  g_free(document_ids);
  assert(length == 2);
  g_print("block_controller_get_document_ids_test2 OK.\n");
}

int main(void)
{
  open_database("test.db"); 
  g_print("BLOCK CONTROLLER TEST:\n");
  block_controller_get_document_ids_test1();
  block_controller_get_document_ids_test2();
  close_database("test.db");
  return 0;
}