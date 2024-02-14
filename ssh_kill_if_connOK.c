#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// Output cmd: ~/.sk/SSHkill
// gcc ~/.sk/ssh_kill_if_connOK.c -o ~/.sk/SSHkill

void main()
{
	char buffer[20]={0}, str_pid[10]={0};

	system("rm ~/.sk/.sk_gitignore/ssh_kill.txt");
	system("ps -e | grep SSHconTester > ~/.sk/.sk_gitignore/ssh_kill.txt");

	FILE *fp=fopen("/home/shubham/.sk/.sk_gitignore/ssh_kill.txt", "r");			// Main program ID will be at TOP, so killing that will STOP all
	if(fp != NULL)
	{
		fscanf(fp,"%s", str_pid);	
		fclose(fp);

		printf(" %s \n", str_pid);
		
		if(str_pid[0] == 0)	// NO PID to kill
		{
			printf("\n\n NO PID Found to be Killed. Exiting ... \n");

			// -> Added extra by sk on Sep 14 2022
			system("ps -e | grep -w ssh$");
			system("ps -e | grep -w ssh$ > ~/.sk/.sk_gitignore/ssh_kill.txt");
			fp=fopen("/home/shubham/.sk/.sk_gitignore/ssh_kill.txt", "r");
			
			if(fp)
			{
        	        	fscanf(fp,"%s", str_pid);
	        	        fclose(fp);
				printf("killing pid ->  %s automatically at last...\n", str_pid);
				
				if(str_pid[0] != 0)
				{
					strcpy(buffer, "kill -9 ");
		                        strcat(buffer, str_pid);
					system(buffer);
				}		
			} // <- Added Extra by sk
			
			exit(0);
		}
			
			strcpy(buffer, "kill -9 ");
			strcat(buffer, str_pid);

			printf("\t Killing pid %s ... \n", str_pid);
			system(buffer);
		
		printf("\n Checking for Another Connection ...");
		sleep(3);
		main();
	}
	else
		printf("\n fp = NULL. Unable to kill \n");

}
