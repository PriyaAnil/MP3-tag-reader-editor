#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "read.h"
#include "tags.h"
#include "types.h"

// Check opertaion to be performed
OperationType check_operation(char ** argv)
{
	if (strncmp(*argv,"-r",2) == 0)
	{
		return read;
	}
	if (strncmp(*argv,"-e",2 )== 0)
	{
		return edit;
	}
	else
		return e_unsupported;

}

// Read and validate the arguments
Status read_and_validate_read_args(char *argv[],TagRead *tagread)
{
	strcpy(tagread -> mp3_fname,  argv[2]);
	printf("%s", tagread->mp3_fname);

	return e_success;
}

// Open file in read mode
Status open_files(TagRead *tagread)
{
	tagread -> fptr_mp3 =fopen(tagread ->mp3_fname,"r" );

	// Sanity check
	if (tagread -> fptr_mp3 == NULL)
	{
		perror("fopen");
		fprintf(stderr,"ERROR: unable to open the file %s\n",tagread ->mp3_fname);

		return e_failure;
	}
	return e_success;
}

// Calculate the tag size
Status get_size_tag(FILE *fptr)
{
	int idx;
	unsigned char ch = '\0';
	int size = 0;

	for (idx = 0; idx < sizeof(int); idx++)
	{
		fread(&ch, sizeof(char), 1, fptr);
		size = size|ch;

		if ((sizeof(int) - 1)!= idx)
		{
			size <<= (sizeof(char) * 8);
		}
	}
	return size;
}

// Get album art tags
Status album_art(TagRead *tagread)
{
	char s1 = 0xff , s2 = 0xd8 ;
	char e1 = 0xff , e2 = 0xd9 ;
	char ch;
	int flag = 0;
	FILE *fp;

	// Open a file in write mode
	fp = fopen("pictures.jpg","w");

	// Sanity check
	if (fp == NULL)	
		printf("Error to open the file\n");
	
	while (feof(tagread->fptr_mp3) == 0)
	{
		fread(&ch, sizeof(char) , 1, tagread->fptr_mp3);
		
		// Check for ff d8
		while (ch == s1)
		{
			if (ch == s2)
			{
			
				// Write ff d8 to new file
				fwrite(&s1 , 1 ,1 ,fp);
				fwrite(&s2 , 1 , 1 , fp);

				while (1)
				{
					fread(&ch, sizeof(char) , 1 ,tagread->fptr_mp3);
					fwrite(&ch,1 ,1, fp);
					
					// Check for ff d9
					if (flag && ch == e2)
						break;
				
					if(ch == e1)
					
						flag = 1;
					else
						flag = 0;
				}
			}
		}
	}
}

// Read infomation from the file
Status readInfo(int *size, char **ptr, FILE *fptr)
{
	int idx;
	int tag_size;
	char flag_buff[2];

	// Get the size
	*size = get_size_tag(fptr);

	// Allocate memory  
	*ptr = (char*)malloc(*size * sizeof(char));

	// Neglect the flags
	fread(flag_buff , sizeof(char) , 2 ,fptr);
	
	// Read byte by byte from the file
	for (idx= 0; idx < (*size); idx++)
		fread(*ptr + idx, sizeof(char), 1, fptr);
	
	return e_success;
}

// Print the tags
Status print_tags(int size, char *ptr)
{
	int idx;

	for (idx = 0; idx < size; idx++)
	{
		if (ptr[idx] > 31 && ptr[idx] != 92 && ptr[idx] != 127)
			printf("%c", ptr[idx]);
	}
	printf("\n");
	
	return e_success;
}

// Get the tags to read information
Status gets_tags_for_read(char*buffer, TagRead *tagread)
{
	int idx;
	char *ptr_buff[7] ={ALBUM_NAME, ARTIST_NAME, TRACK_NAME,ALBUM_YEAR,TRACK_NO,COMPOSER_NAME,ALBUM_ART};

	for(idx=0; idx < 6; idx++)
	{

		if(strncmp(buffer, ptr_buff[idx], 4) == 0)
		{

	printf("\n*************************************\n\n");
			switch(idx)
			{
				case 0:

					// Function call for read information from the tags
					readInfo( &tagread->album_name_size, &tagread->album_name ,tagread-> fptr_mp3);
					
					// Function call to print inormation from the tags
					printf("ALBUM NAME  :");
					print_tags(tagread->album_name_size, tagread->album_name);
					break;

				case 1:

					// Function call for read information from the tags
					readInfo( &tagread->singer_name_size, &tagread->singer_name ,tagread-> fptr_mp3);
					
					// Function call to print inormation from the tags
					printf("SINGER NAME  :");
					print_tags(tagread->singer_name_size, tagread->singer_name);
					break;

				case 2:
					
					// Function call for read information from the tags
					readInfo( &tagread->song_name_size, &tagread->song_name ,tagread-> fptr_mp3);
					
					// Function call to print inormation from the tags
					printf("SONG NAME  :");
					print_tags(tagread->song_name_size, tagread->song_name);
					break;

				case 3:
					
					// Function call for read information from the tags
					readInfo( &tagread->album_year_size, &tagread->album_year ,tagread-> fptr_mp3);
					
					// Function call to print inormation from the tags
					printf("ALBUM YEAR  :");
					print_tags(tagread->album_year_size, tagread->album_year);
					break;

				case 4:
					
					// Function call for read information from the tags
					readInfo( &tagread->track_number_size, &tagread->track_number ,tagread-> fptr_mp3);
					
					// Function call to print inormation from the tags
					printf("TRACK NUMBER :");
					print_tags(tagread->track_number_size, tagread->track_number);
					break;

				case 5:
					
					// Function call for read information from the tags
					readInfo( &tagread->composer_name_size, &tagread->composer_name ,tagread-> fptr_mp3);
					
					// Function call to print inormation from the tags
					printf("COMPOSER NAME :");
					print_tags(tagread->composer_name_size, tagread->composer_name);
					break;

				case 6:

					// Function call for reading album art info
					album_art(tagread);
					break;
			}
		}
	}
	return e_success;
}

// Get tags main function
Status get_tags(TagRead *tagread, char *argv[], int argc)
{

	int byte_read, idx;
	char buffer[4];
	char ch;

	// Function call to read and validate arguments
	if (argc == 3)
		read_and_validate_read_args(argv, tagread);
	
	// Function call to open file
	open_files(tagread);

	// Check the tag name
	while(feof(tagread -> fptr_mp3)==0)
	{
		fread(&ch ,sizeof(char) ,1 ,tagread->fptr_mp3);

		// Check for T or A
		if(ch == 'T' || ch == 'A')
		{
			buffer[0] = ch;

			// Copy the remaining tag names characters
			for(idx = 1; idx < 4; idx++)
			{
				fread(&ch ,sizeof(char) ,1 ,tagread->fptr_mp3);
				buffer[idx] = ch;

			}
			buffer[idx] = '\0';

			// Function call to get the tags for reading info
			gets_tags_for_read(buffer, tagread);
		}
	}

	// Close file
	fclose(tagread->fptr_mp3);
	return e_success;
}

