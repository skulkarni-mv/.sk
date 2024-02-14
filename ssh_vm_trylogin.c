#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>


// Output cmd: ~/.sk/SSHconTester
// gcc ~/.sk/ssh_vm_trylogin.c -o ~/.sk/SSHconTester

void main()
{

	static char otp_once=100;		// When VM restarts, it refuses connection, so give 2 attaempts for refuse

	pid_t chl_id;
	int delay = 5;

	if( chl_id=fork() )	// Parent
	{

		otp_once--;	// reduce to 1 from 2 first time, then 0		

		sleep(1);			// So that Child can complete first
		printf("\r");			// Carriage Return
		for(int i=0; i<15; i++)
		{	
			printf("."); fflush(stdout);
			sleep(1);
		}

		system("ps -e | grep -w ssh$ > .sk_gitignore/ssh_ps.txt");	// grep 'ssh' as vm strats session in ssh process

		//sleep(1);
		
		FILE *fp=fopen("/home/shubham/.sk/.sk_gitignore/ssh_ps.txt","r");

		if(fp)			// File Read Successful
		{
			char buffer[20]={0}, str_kill[20]="kill -9 ";

			//printf(" cat cmd __");	fflush(stdout);
			system("cat /home/shubham/.sk/.sk_gitignore/ssh_ps.txt");

			if((fscanf(fp,"%s",buffer)) != EOF)
			{					// pid of ssh read successfully
			
				strcat(str_kill, buffer);
				printf("\r  found pid of SSH - %6s, %s . waiting for new connection in %d seconds \n", buffer, str_kill, delay);
			
				system(str_kill);
				//sleep(delay);
				for(int i=0; i<delay; i++)
				{
					printf("*");	fflush(stdout);
					sleep(1);
				}
					printf("\n");
				main();
			}
			else if(otp_once >= 0)
			{
				printf("\r Giving extra attempt, as otp_once = %d \n", otp_once);
				main();
			}
			else
			{
				printf("\n\t No PID for SSH found. Exiting\n\n");
				return;
			}

		}
		else
		{
			printf("\n\t FILE not found. Exiting\n\n");
			return;
		}

	}
	else			// Child
	{

		char *space_found=NULL;
		time_t t_sec;
		struct tm tm;

		t_sec = time(NULL);
		tm =  *localtime(&t_sec);
		
		printf("\n\r");				// Carriage Return
		printf("%2d:%2d:%2d", tm.tm_hour, tm.tm_min, tm.tm_sec);	fflush(stdout);
		
		printf(" child pid = %6d, opening new SSH connection: \n", getpid());		
		system("ssh qauser@10.80.200.8");
		exit(0);
	}


}
