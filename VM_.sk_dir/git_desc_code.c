#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// OUTPUT cmd: /bin/gitdsk2 [Run gitdsk1 from .bashrc to take log first / Run gitdsk from .bashrc for pid to kill ("kl" from .bashrc) the process]
// gcc ~/.sk/git_desc_code.c -o ~/.sk/gitdsk2; sudo mv ~/.sk/gitdsk2 /bin/

#define max_len_fgets 1000

void main(int argc)
{
	char buf_fscanf[20]={0}, buf_fgets[max_len_fgets]={0}, buf_cmd[50]={0};

	if(argc != 2) 	// pass any argument along with executable a.out (argc==2) to avoid the Instructions - Expert Mode
	{	
		printf(" Starting in Non-Expert Mode... \n");
		printf("\t File 'git_log_of_this_dir.txt' contains : \n"); 
		system("cat /home/qauser/.sk/git_log_of_this_dir.txt");
		printf("\n");
		sleep(2);

		printf("\n\t Make sure you are in the dir where you have taken git log... \n");
		sleep(5);
	}

	FILE *fp=fopen("/home/qauser/.sk/git_log-oneline_file.txt", "r");

	if(fp)
	{
		int i =0;

			while( fscanf(fp, "%s", buf_fscanf) != EOF)
			{
				fgets(buf_fgets, max_len_fgets-10, fp);	//printf("  _%s_ .\n", buf_fgets); // went to the new-line
		
				strcpy(buf_cmd, "git describe --contains ");
				strcat(buf_cmd, buf_fscanf);

				//if(i>=170 && i<=185)
				{
					printf("\t\t %4d. %s \t\n", i, buf_fscanf);
					
					system(buf_cmd);
					printf("\n");
				}
				i++;

				//if(i>185)	break;
			}

		fclose(fp);
	}
	else
		printf("fp=NULL. File opening failed \n\n");
}
