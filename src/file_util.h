#pragma once

#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>

void
get_files_in_directory(const char *directory, char **files, int *length);
