#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_file_into_buffer(char **buffer, const char *path)
{
	int fsize;
	char *str;
	FILE *file;
	char ls[512] = "Reading ";

	strcat(ls, path);
	strcat(ls, " into buffer...");
	printf("%-45s", ls);

	file = fopen(path, "rb");
	fseek(file, 0, SEEK_END);
	fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	str = (char*)malloc(fsize + 1);
	fread(str, 1, fsize, file);
	fclose(file);
	str[fsize] = 0;

	*buffer = str;

	printf("\033[0;32m%5s\033[0m\n", "OK!");
	return 0;
}
