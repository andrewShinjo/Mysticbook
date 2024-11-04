#include "./block_controller.h"
#include "../service/block_service.h"

gint64 block_controller_create_document(gchar *content)
{
  return block_service_create_document(content);
}

GArray* block_controller_get_document_ids()
{
  return block_service_get_document_ids();
}

int block_controller_indent_block(gint64 id)
{
  return block_service_indent_block(id);  
}
