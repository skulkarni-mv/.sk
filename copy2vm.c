#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void main(int argc, char *argv[])
{
	char buffer[500]={0};
	char t_now[10]={0};
	char *space_found = NULL;

	if(argc<2 || argc>3)
	{
		printf("\n    Invalid. Usage:$ c2v <file_name_in_pwd> <OPT:'n' to keep same 'name'>\n");
		return;
	}

		FILE *fp;
	if( (fp=fopen(argv[1], "r")) == NULL)        // if file NOT present
	{
		printf("\n    File Not Present. \n");
		return;
	}

	  time_t t = time(NULL);
	  struct tm tm = *localtime(&t);

	sprintf(t_now, "%2d-%2d", tm.tm_hour, tm.tm_min);

	while((space_found=strchr(t_now,' ')))	// space found is NOT NULL
		*space_found = '0';		// replace space with '0'
		
	strcpy(buffer, "scp ");
	strcat(buffer, argv[1]);
	strcat(buffer, " qauser@10.80.200.8:/home/qauser/");	

	if(strrchr(argv[1],'/') != NULL)			// ENTIRE PATH is sent
	// Adding time stamp to the file to avoid overwrites
        	strcat(buffer, strrchr(argv[1],'/')+1); // get only file name at last from the entire path \
                                                        //if +1 NOT used -> scp /home/shubham/hi qauser@10.80.200.8:/home/qauser//hi_20-05

	else						// ONLY FILE NAME is sent for cp2vm. 
		strcat(buffer, argv[1]);
	
	if(argc==3 && argv[2][0]=='n');			// Added by SK on 2022-09-29 for keeping same name
	else 
	{
		strcat(buffer, "_");
		strcat(buffer, t_now);
	}

	printf("Command Issued : \n");
	sleep(1);
	printf(" %s\n", buffer);

	system(buffer);
}
