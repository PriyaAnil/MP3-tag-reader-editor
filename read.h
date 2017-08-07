#ifndef  READ_H
#define READ_H

#include "types.h" // Contains user defined types

/* Structure to store information required for
* read tags from the file
* Info about output and intermediate data is
* also stored
*/

typedef struct _ReadInfo
{
	FILE *fptr_mp3;
	char mp3_fname[32];
	
	int album_name_size;
	char *album_name;

	int singer_name_size;
	char *singer_name;

	int song_name_size;
	char *song_name;

	int album_year_size;
	char *album_year;


	int track_number_size;
	char *track_number;

	int album_art_size;
	char *album_art_name;

	int composer_name_size;
	char *composer_name;

}TagRead;

// Check operation to be performed
OperationType check_operation(char ** argv);

// Read and validiate the arguments
Status read_and_validate_read_args(char *argv[],TagRead *tagread);

// Open the files
Status open_files(TagRead *tagread);

// Calculate the size of file tags
Status get_size_tag(FILE *fptr);

// Read information from file
Status print_tags(int size, char *ptr);

// Print the tags
Status readInfo(int *size, char **ptr, FILE *fptr);

// Read Album art
Status album_art(TagRead *tagread);

// Get the tags to read
Status gets_tags_for_read(char*buffer, TagRead *tagread);

// Get tags which does the actual reading info
Status get_tags(TagRead *tagread, char *argv[], int argc);

#endif
