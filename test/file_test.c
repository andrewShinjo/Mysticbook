#include <assert.h>
#include "../src/backend/file_explorer.h"

int
main()
{
	FileExplorer fe;
	const char *directory = "../sample_data";
	int file_count;

	char ** filenames = get_filenames(directory, &file_count);
	
	for(int i = 0; i < file_count; i++)
	{
		printf("filename: %s\n", filenames[i]);
	}

	printf("File count: %d\n", file_count);

	return 0;
}
