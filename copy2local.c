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

	if(argc!=2)
	{
		printf("\n    Invalid. Usage:$ c2l <file_PATH_in_vm> \n");
		return;
	}
          time_t t = time(NULL);
          struct tm tm = *localtime(&t);
 
        sprintf(t_now, "%2d-%2d", tm.tm_hour, tm.tm_min);

        while((space_found=strchr(t_now,' ')))  // space found is NOT NULL
                *space_found = '0';             // replace space with '0'


	strcpy(buffer, "scp qauser@10.80.200.8:");
	strcat(buffer, argv[1]);
	strcat(buffer, " ./");

	if(strrchr(argv[1],'/') != NULL)
        // Adding time stamp to the file to avoid overwrites
		strcat(buffer, strrchr(argv[1],'/')+1);		// get only file name at last from the entire path \
								//if +1 NOT used -> scp qauser@10.80.200.8:/home/qauser/hi .//hi_20-05
	else{
		printf("\n Invalid. Enter full path of file in VM \n");
		return;
	}	

        strcat(buffer, "_");
        strcat(buffer, t_now);

	printf("Command Issued : \n");
	sleep(1);
	printf(" %s\n", buffer);

	system(buffer);
}
