#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void main(int argc, char **argv)
{

	char buf_fscanf[20]={0}, buf_fgets[500]={0}, buf_cmd[50]={0}, buffer[100]={0};

	if(argc!=2)
	{
		printf("\n\t Invalid. Usage: ./a.out <file> \n\n");
		return;
	}

	FILE *fp=fopen("/home/shubham/.sk/06_parseCommitIDs2Revert_w27_Dec19/gitOneline.txt", "r");

	if(fp)
	{
//		fgets(buf_fgets,490,fp);	printf(" ommiting first line ->  _%s  \n", buf_fgets); // went to the new-line
	

		FILE *fw=fopen("/home/shubham/.sk/06_parseCommitIDs2Revert_w27_Dec19/extracted_commits.sh", "w");

		while( fscanf(fp, "%s", buf_fscanf) != EOF)
		{
			printf("commit ID =  %s \n", buf_fscanf);		
			fgets(buf_fgets,490,fp);	// GOTO next Line

			fprintf(fw, "git revert %s --no-edit && \", buf_fscanf);
		}

		fclose(fw);
		fclose(fp);		

	}
	else
		perror("fopen");
}
