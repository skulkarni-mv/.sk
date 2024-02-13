#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main(int argc, char* argv[])
{
	int cnt=0;
	char buffer[500]={0}, arr_fscanf[100]={0};

	system("echo \" Remember to GIT RESET HARD to commit before copying files\" ");

	getchar();	
	
	FILE *fp = fopen("write_file_3", "r");
	if(fp == NULL) {
		perror("fopen");
		printf(" ISUUE IN READ - Err 01 \n\n");
	}	
	
	
	while( fscanf(fp, "%s", arr_fscanf) != EOF) {
		strcpy(buffer, "cp ");
		strcat(buffer, arr_fscanf);
		strcat(buffer, " ../../hostap/");
		strcat(buffer, arr_fscanf);

//		printf("%s\n", buffer);

		system(buffer);
		
		
	}
}
