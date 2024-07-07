#pragma once

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ** get_filenames(const char *directory, int *file_count);