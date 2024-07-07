#include "file_explorer.h"

char ** get_filenames(const char *directory_path, int *file_count)
{
    DIR *directory;
    struct dirent *entry;
    char **filenames = NULL;
    int count = 0;

    directory = opendir(directory_path);

    if(directory == NULL)
    {
        perror("opendir");
        return NULL;
    }

    while((entry = readdir(directory)) != NULL)
    {
        if(entry->d_type == DT_REG)
        {
            count++;
        }
    }

    filenames = (char **) malloc(count * sizeof(char *));

    if(filenames == NULL)
    {
        perror("malloc");
        closedir(directory);
        return NULL;
    }

    rewinddir(directory);

    int index = 0;

    while((entry = readdir(directory)) != NULL)
    {
        if(entry->d_type != DT_REG) 
        {
            continue;
        }

        filenames[index] = (char *) malloc(strlen(entry->d_name) + 1);

        if(filenames[index] == NULL)
        {
            perror("malloc");
            closedir(directory);
            
            for(int i = 0; i < index; i++)
            {
                free(filenames[i]);
            }

            free(filenames);
            return NULL;
        }

        strcpy(filenames[index], entry->d_name);
        index++;
    }

    closedir(directory);
    *file_count = count;

    return filenames;
}