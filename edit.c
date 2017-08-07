#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "edit.h"
#include "tags.h"
#include "types.h"

// Read and validiate the arguments
Status read_and_validate_edit_args(char *argv[], TagEdit *tagedit)
{
	strcpy(tagedit -> mp3_fname, argv[3]);

	printf("%s", tagedit -> mp3_fname);
	
	return e_success;
}

// Open the files
Status open_edit_files(TagEdit *tagedit)
{
	tagedit -> fptr_mp3 = fopen(tagedit -> mp3_fname, "r+");

	if(tagedit -> fptr_mp3 == NULL)
	{
		perror("fopen");
		fprintf(stderr,"ERROR: unable to open the file %s\n",tagedit -> mp3_fname);

		return e_failure;
	}
	return e_success;
}

// Calculate the size of file tags
Status get_edit_size_tag(FILE *fptr)
{
	int idx;
	unsigned char ch = '\0';
	int size = 0;

	for (idx = 0; idx < sizeof(int); idx++)
	{
		fread(&ch ,sizeof(char), 1, fptr);

		size = size|ch;

		if ((sizeof(int) - 1) != idx)
		{
			size <<= (sizeof(char) * 8);
		}
	}
	return size;
}

// Get tags to be editted
int get_tags_to_edit(char *argv[])
{
	int idx;

	char *array[6] = {"-album", "-singer", "-song", "-year", "-tracknum", "-composer"};

	for (idx = 0; idx < 6; idx++)
	{
		if (strncmp(argv[2], array[idx], strlen(array[idx])) == 0)
		{
			break;
		}
	}

	return idx+1;
}

// Get the options for tags to be editted
Status get_options(char *argv[], int argc, TagEdit *tagedit)
{
	int edit_option;

	// get options for command line arguments
	if (argc == 4)
	{
		edit_option = get_tags_to_edit(argv);
		printf("%d", edit_option);
	}

	else
	{
		printf("Enter the edit option:\n1.Album\n2.Singer\n3.Song\n4.Year\n5.Tracknum\n6.Composer\nEnter the option:  ");
		scanf("%d", &edit_option);
	}

	switch (edit_option)
	{
		case 1:
			
			// Copy tag name 
			strcpy(tagedit->edit_tags, "TALB");
			break;
		case 2:
			// Copy tag name 
			strcpy(tagedit->edit_tags, "TPE1");
			break;
		case 3:
			// Copy tag name 
			strcpy(tagedit->edit_tags, "TIT2");
			break;
		case 4:
			// Copy tag name 
			strcpy(tagedit->edit_tags, "TYER");
			break;
		case 5:
			// Copy tag name 
			strcpy(tagedit->edit_tags, "TRCK");
			break;
		case 6:
			// Copy tag name 
			strcpy(tagedit->edit_tags, "TCOM");
			break;
	
		default:
			printf("Invalid option\n");
	}
	return e_success;
}

// Copy the tags from file
Status copy_tag_names(TagEdit *tagedit)
{
	int size, idx, length, count = 0, count1;
	char *ptr, buffer[50], temp[3], nul = '\0';

	size = get_edit_size_tag(tagedit -> fptr_mp3);

	fread(temp, 1, 2, tagedit -> fptr_mp3);

	// Read the new tags 
	printf("Enter the new data: ");
	scanf("\n%[^\n]", buffer);
	

	length = strlen(buffer);
	count1 = size - length - 1;

	// Check for new data length with the old one
	if (length + 1 == size)
	{
		// Write null 
		fwrite(&nul, 1, 1, tagedit->fptr_mp3);

		// write the new data
		fwrite(&buffer, 1, strlen(buffer), tagedit->fptr_mp3);
	}
	
	// If new data length less than old one
	if (length + 1 < size)
	{
		
		// Write null
		fwrite(&nul, 1, 1, tagedit->fptr_mp3);

		// Write the new data
		fwrite(&buffer, 1, strlen(buffer), tagedit->fptr_mp3);
		for (idx = 0; idx < count1; idx++)
		{	
			// Write null for the remaining space
			fwrite(&nul, 1, 1, tagedit->fptr_mp3);
		}	
	}

	// Error message
	if (length + 1 > size)
		printf("Error! New data size cannot be greater than old data size\n");
}

// Edit tags main function
Status edit_tags(TagEdit *tagedit, char *argv[], int argc)
{
	
	int byte_read, idx;
	char buffer[4], ch;


	// Fuction call to read and validate arguments
	if (argc == 4)
		read_and_validate_edit_args(argv, tagedit);

	// Function call to open files
	open_edit_files(tagedit);
	
	// Function call to get options for editting
	if (get_options(argv, argc, tagedit) != e_success)
		printf("Error in get_options\n");

	while(feof(tagedit -> fptr_mp3)==0)
	{
		fread(&ch ,sizeof(char) ,1 ,tagedit->fptr_mp3);

		// Get the tag name
		if(ch == tagedit->edit_tags[0])
		{
			buffer[0] = ch;

			for(idx = 1; idx < 4; idx++)
			{
				fread(&ch ,sizeof(char) ,1 ,tagedit->fptr_mp3);
				buffer[idx] = ch;

			}
			buffer[idx] = '\0';
			
			if (strcmp(buffer, tagedit->edit_tags) == 0)
			{
				// Function call to copy tags from file
				copy_tag_names(tagedit);
				break;
			}
		}
	}
	
	// Close file 
	fclose(tagedit->fptr_mp3);
	return e_success;
}



			




	

