#include<stdio.h>
#include<string.h>

// cmd from code : replaceEnter_sk

void main(int argc, char *argv[])
{
	if(argc !=2)
	{
		printf("Invalid. Usage: replaceEnter_sk <file_name_to_replace_enter> \n");
		return;
	}
	
	char last_char=0;

   	FILE *fp=fopen(argv[1],"r+");			// argv[1] = "home/shubham/.sk/bulkcp_dir_is.txt"
 
        if(fp)                  // check if file exists
        {
        	fseek(fp,-1,SEEK_END);          	// Replacing Enter '\n' with space ' ' at EOF
		last_char=fgetc(fp);

		if(last_char == '\n')
		{
        		fseek(fp,-1,SEEK_END);          // Replacing Enter '\n' with space ' ' at EOF
                	fputc('\0',fp);
		}
		else
			printf(" Enter NOT found (%c=%d Found) at the end of File \n", last_char, last_char);

                fclose(fp);
 	}
}

