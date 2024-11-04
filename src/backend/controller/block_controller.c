#include "./block_controller.h"
#include "../service/block_service.h"

GArray* block_controller_get_document_ids()
{
  return block_service_get_document_ids();
}

int block_controller_indent_block(gint64 id)
{
  return block_service_indent_block(id);  
}
