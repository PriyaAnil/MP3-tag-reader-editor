#ifndef  EDIT_H
#define EDIT_H

#include "types.h" // Contains user definedtypes

/* Structure to store information required for
* read tags from the file
* Info about output and intermediate data is
* also stored
*/

typedef struct _EditInfo
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

	char edit_tags[5];
}TagEdit;

// Read and validiate the arguments
Status read_and_validate_edit_args(char *argv[], TagEdit *tagedit);

// Open the files
Status open_edit_files(TagEdit *tagedit);

// Calculate the size of file tags
Status get_edit_size_tag(FILE *fptr);

// Get tags to be editted
int get_tags_to_edit(char *argv[]);

// Get the options for tags to be editted
Status get_options(char *argv[], int argc, TagEdit *tagedit);

// Copy the tags from file
Status copy_tag_names(TagEdit *tagedit);

// Edit tags main function
Status edit_tags(TagEdit *tagedit, char *argv[], int argc);

#endif
