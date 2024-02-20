#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


// Output cmd: /bin/sendtoLap
// gcc ~/.sk/send_text_to_Lap.c -o ~/.sk/sendtoLAP; sudo mv ~/.sk/sendtoLAP /bin/

//#define debug

void main(int argc, char *argv[])
{
	char buffer[500]={0};

	if(argc!=2)
	{
		printf("\n   Invalid. Usage:$ sendtoLap <content_to_be_sent OR sendtoLap (to share manually edited file)> \n");
		return;
	}

	if(strcmp(argv[0], argv[1]) != 0)	// check if used cmd is $ sendtoLap sendtoLap , if yes means file to be sent is edited manually
						// in this case, that edit file directly should be sent, not content passed on CLI
	{
		FILE *fw = fopen("/home/shubham/.sk/.sk_gitignore/send-THIS2Lap.txt", "w");
		fprintf(fw, "%s\n", argv[1]);
		fclose(fw);
	}
	else
	{
		printf("\n Received cmd: $ sendtoLap sendtoLap. Sending manually edited file ~/.sk/.sk_gitignore/send-THIS2Lap.txt \n");
	}

#ifdef debug
	int mainpid = getpid();	printf(" main proc pid = %d \n\n", mainpid);
#endif

/////////////


	int n, i=0;
	int p[5]={0};
	int first_ret=0;
	char kill_this_pid[6]={0};
	char buffer_pid[100]={0};

	if( (i++, p[0]=fork()) && (i++, p[1]=fork()) && (i++, p[2]=fork()) )
	{
		i=0;		// Parent

		printf("in parent, p[0]=%d, p[1]=%d, p[2]=%d \n", p[0], p[1], p[2]);
#ifdef debug
		printf("inside parent at start, pid=%d \n", getpid());
#endif
		first_ret=wait(0);

		for(int j=0; j<3; j++)
		{
			if(p[j]==first_ret)
			{
				printf("\t\t\t wait returned -> first return pid p[%d]=%d OR first_ret=%d \n", j, p[j], first_ret);
			}
			else
			{
#ifdef debug
				printf(" should be killed pid is p[%d]=%d \n", j, p[j]);
#endif
				sprintf(kill_this_pid, "%d", p[j]);
				strcpy(buffer_pid, "kill -9 ");
				strcat(buffer_pid, kill_this_pid);

				printf(" \t\t buffer = %s \n", buffer_pid);
				system(buffer_pid);

			}
		}

		printf("\t\t\t wait returned -> %d \n", wait(0) );
		printf("\t\t\t wait returned -> %d \n", wait(0) );	// -> -1
		printf("inside parent at end.. \n");
	}
	else
	{
		sleep(1);
		printf("\n");
		if(i==1)
		{
#ifdef debug
			printf("inside child1 at start, pid=%d, ppid=%d \n", getpid(), getppid() );
#endif
			system("ping -c 1 172.31.84.172 > /dev/null");
#ifdef debug
			printf("inside child1 at end.. \n");
#endif
		}
		else if(i==2)
		{
#ifdef debug
			printf("inside child2 at start, pid=%d, ppid=%d \n", getpid(), getppid() );
#endif
			system("ping -c 1 192.168.50.134 > /dev/null");
#ifdef debug
			printf("inside child2 at end.. \n");
#endif
		}
		else if(i==3)
		{
#ifdef debug
			printf("inside child3 at start, pid=%d, ppid=%d \n", getpid(), getppid() );
#endif
			system("ping -c 1 10.80.10.3 > /dev/null");
#ifdef debug
			printf("inside child3 at end.. \n");
#endif
		}
	}



/////////////

/*
"sshpass -f /home/shubham/.sk/.localpass.txt scp /home/shubham/.sk/send-THIS2Lap.txt shubham@xyz:/home/shubham/.sk/send-ReceivedFromPC.txt"
	
	printf("\n Use: send-cat / send-view (GEDIT)  to view the content sent from Laptop");
	printf("\n Use: rec-cat  / rec-view  (GEDIT)  to view the content received from PC");

	printf("\n"); */
}
