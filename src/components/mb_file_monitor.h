#pragma once
#include<gio/gio.h>

GFileMonitor* mb_file_monitor_new(GFile *directory);
