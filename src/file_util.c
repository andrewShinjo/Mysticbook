#include "file_util.h"

void
get_files_in_directory(const char *directory, char **files, int *length)
{
	*length = 0;
	struct dirent *entry;
	DIR *dp;

	// Get the number of files in the directory.
	{
		dp = opendir(directory);

		if(dp == NULL)
		{
			perror("opendir");
			exit(EXIT_FAILURE);
		}	

		while((entry = readdir(dp)) != NULL)
		{
			(*length)++;	
		}
	
	}

	// Get the names of each file in the directory.
	{
		rewinddir(dp);
		
		files = (char **) malloc(*length * sizeof(char *));

		while((entry = readdir(dp)) != NULL)
		{
			char filename[1024];
		}
	}



}
