#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "tags.h"
#include "read.h"
#include "edit.h"

int main(int argc, char *argv[])
{
	int option;
	char ch;
	
	TagRead tagread;
	TagEdit tagedit;

	// Read the option for operation
	if (argc == 1)
	{
		printf("Select the option:\n1. Read\n2. Edit\nEnter option: ");
		scanf("%d", &option);
	}

	else
		option = check_operation(&argv[1]);

	switch (option)
	{
		case read:

			// Read the file name
			if (argc == 1)
			{
				printf("Enter the file name: ");
				scanf("%s", tagread.mp3_fname);
			}

			// Function call for reading the tags
			get_tags(&tagread, argv, argc);
			break;

		case edit:
			
			// Read the file name
			if (argc == 1)
			{
				printf("Enter the file name: ");
				scanf("%s", tagedit.mp3_fname);
			}
			
			// Function call for editting the tags
			edit_tags(&tagedit, argv, argc);
			break;

		default:
			printf("Invalid option\n");
	}

	// No error return e_success;
	return e_success;

}
