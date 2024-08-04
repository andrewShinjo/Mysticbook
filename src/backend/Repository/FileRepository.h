#pragma once

#include <stdio.h>
#include <stdlib.h>

int CreateFile(const char *Filepath);
int ReadFile(const char *Filepath, char *Buffer, size_t *BufferSize);
int UpdateFile(const char *Filepath, const char *Buffer, size_t BufferSize);
int DeleteFile(const char *Filepath);
