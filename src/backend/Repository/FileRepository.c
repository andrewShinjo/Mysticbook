#include "FileRepository.h"

// PUBLIC

int CreateFile(const char *Filepath)
{
    FILE *File = fopen(Filepath, "w");

    if (File == NULL)
    {
        perror("Create_File: fopen failed.");
        return 1;
    }

    fclose(File);
    return 0;
}

int ReadFile (const char *Filepath, char *Buffer, size_t *BufferSize)
{
    FILE *File = fopen(Filepath, "r");

    if (!File)
    {
        perror("Read_File: file doesn't exist.");
        return 1;
    }

    fseek (File, 0, SEEK_END);
    *BufferSize = ftell(File);
    fseek (File, 0, SEEK_SET);

    Buffer = (char *) malloc(*BufferSize + 1);

    if (!Buffer)
    {
        perror("Read_File: failed to allocate memory.");
        fclose (File);
        return 1;
    }

    size_t Read_Size = fread (Buffer, 1, *BufferSize, File);

    if (Read_Size != *BufferSize)
    {
        perror("Read_File: error reading file.");
        free (Buffer);
        fclose (File);
        return 1;
    }

    Buffer[*BufferSize] = '\0';
    fclose (File);
    return 0;

}

int UpdateFile (const char *Filepath, const char *Buffer, size_t BufferSize)
{
    FILE *File = fopen(Filepath, "r");

    if (!File)
    {
        perror("Update_File: file doesn't exist.");
        return 1;
    } 

    size_t Write_Size = fwrite (Buffer, 1, BufferSize, File);

    if (Write_Size != BufferSize)
    {
        perror ("Update_File: error writing to file.");
        fclose (File);
        return 1;
    }

    fclose (File);
    return 0;
}

int DeleteFile (const char *Filepath)
{
    if (remove(Filepath) == 0)
    {
        return 0;
    }

    perror ("Delete_File: failed to delete file.");
    return 1;
}