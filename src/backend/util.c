#include <time.h>
#include "./util.h"

gint64 get_current_timestamp()
{
  time_t now = time(NULL);
  gint64 current_timestamp = (gint64) now;
  return current_timestamp;
}
