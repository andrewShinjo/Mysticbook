#pragma once

#include <stdio.h>
#include <stdlib.h>

int Create_File(const char *Filepath);
int Read_File(const char *Filepath, char *Buffer, size_t *Buffer_Size);
int Update_File(const char *Filepath, const char *Buffer, size_t Buffer_Size);
int Delete_File(const char *Filepath);
