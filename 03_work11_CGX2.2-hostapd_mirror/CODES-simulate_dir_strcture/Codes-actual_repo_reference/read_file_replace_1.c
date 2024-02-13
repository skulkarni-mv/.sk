#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char *argv[])
{
	char buffer[9999]={0}, buff2[9999]={0};
	char ch, arr_fscanf[100]={0}, arr_tempbuf[1000]={0};
	int no_of_dashes = 0;
	int print_mode = 0, i_strlen=0;

	printf(" Usage: ./read.o <patch_full_address> \n\n");

	if(argc != 2)
		return;


	FILE *fp = fopen(argv[1], "r");
	if(fp==NULL) {
		perror("fopen");
		return;
	}

	FILE *fw = fopen("write_file_1", "w");

	while( (ch=fgetc(fp)) != -1)
	{

		
		if (print_mode == 1) {
//			printf("%c", ch);
			fputc(ch, fw);

			i_strlen = strlen(buffer);
			buffer[i_strlen] = ch;

			if ( strstr(buffer, " changed, ") != NULL) {		// "diff --git"
				print_mode = 0;
				fclose(fw);
				break;
			}
		}

		if(ch == '-') {
			no_of_dashes++;

			if(no_of_dashes == 3) {
				//printf("_-_");
				print_mode = 1;
			}
		}
		else
		{
			no_of_dashes = 0;
		}				
	}
	
	fclose(fp);

	fp = fopen("write_file_1", "r");
	if(fp == NULL) {
		perror("fopen");
		printf(" ISUUE IN SECOND READ - Err 001 \n\n");
	}
	
	fw = fopen("write_file_2", "w");
	buffer[0]='\0';

	while( fscanf(fp, "%s", arr_fscanf) != EOF) {

//		printf("%s\n", arr_fscanf);
		strcat(buffer, arr_fscanf);		
		strcat(buffer, " ");		

//		fprintf(fw ,"%s ", arr_fscanf);

		fgets(arr_tempbuf, 999, fp);		
	}

	if(strstr(buffer, ".../") != NULL) {
		printf(" Manual Intervention Needed \n");
	}
//	printf("\n %s\n", buffer);
	fprintf(fw, "%s", buffer);
	fclose(fw);	

	fp = fopen("write_file_2", "r");
	if(fp == NULL) {
		perror("fopen");
		printf(" ISUUE IN THIRD READ - Err 002 \n\n");
	}
	
	fw = fopen("write_file_3", "w");
	buffer[0]='\0';	

	while( fscanf(fp, "%s", arr_fscanf) != EOF) {

		if( strstr(arr_fscanf, ".../") == NULL)
		{
			strcat(buffer, arr_fscanf);
			strcat(buffer, " ");
		}
	}
	rewind(fp);

	while( fscanf(fp, "%s", arr_fscanf) != EOF) {

		if( strstr(arr_fscanf, ".../") != NULL)
		{
			strcat(buffer, arr_fscanf);
			strcat(buffer, " ");
		}
	}
	fprintf(fw, "%s", "echo \"");
	fprintf(fw, "%s", buffer);
	fprintf(fw, "%s", "\" > write_file_3");
	fclose(fw);	

	system("xclip -selection primary write_file_3");

	printf(" Copied the files, do shift+insert and CHANGE ACCORDINGLY \n");
}

