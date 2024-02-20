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
	char buffer_scp_cmd[500]={0};

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

/////////////

	const char ips_cnt=3;

	int pid_arr[4]={0};	//  [4] == [ ips_cnt+1 ]  used as mainproc_pid stored at arr[0] 
	static int chproc_num;  // OR int chproc_num=0;  -> initialise to 0 is important
	int first_ret=0;
	char pid_str[8]={0};
	char buffer_pid_killer[100]={0};


	pid_arr[0] = getpid();

	if( (chproc_num++, pid_arr[1]=fork()) && (chproc_num++, pid_arr[2]=fork()) && (chproc_num++, pid_arr[3]=fork()) )
	{
		chproc_num=0;		// Parent

//#ifdef debug
		printf("in pid=%d parent, childproc_num=%d\n", pid_arr[0], chproc_num);
		for(int j=1; j<=ips_cnt; j++)
		{
			printf("\t\t pid_arr[%d]=%d \n", j, pid_arr[j]);
		}
		printf("\n");
//#endif
		first_ret=wait(0);

		for(int j=1; j<=ips_cnt; j++)
		{
			if(pid_arr[j]==first_ret)	// Keep this PID
			{
			printf("\t\t\t wait returned -> first_ret=%d pid OR pid_arr[%d]=%d \n", first_ret, j, pid_arr[j]);
			}
			else				// Kill other PIDs
			{
				sprintf(pid_str, "%d", pid_arr[j]);
				strcpy(buffer_pid_killer, "kill -9 ");	strcat(buffer_pid_killer, pid_str);

				printf(" \t\t buffer = %s \n", buffer_pid_killer);
				system(buffer_pid_killer);
			}
		}

		for(int j=1; j<=ips_cnt-1; j++)		// '-1' was done as already once wait(0) used to capture first_ret
		{
			printf("\t\t\t wait returned -> %d \n", wait(0) );
		}
		printf("inside parent at end.. \n");
	}
	else
	{
		switch (chproc_num)
		{
			case 1: system("ping -c 1 172.31.84.172   > /dev/null"); break;
			case 2: system("ping -c 1 192.168.50.134  > /dev/null"); break;
			case 3: system("ping -c 1 10.80.10.3 	  > /dev/null"); break;
		}
	}



/////////////

/*
"sshpass -f /home/shubham/.sk/.localpass.txt scp /home/shubham/.sk/send-THIS2Lap.txt shubham@xyz:/home/shubham/.sk/send-ReceivedFromPC.txt"
	
	printf("\n Use: send-cat / send-view (GEDIT)  to view the content sent from Laptop");
	printf("\n Use: rec-cat  / rec-view  (GEDIT)  to view the content received from PC");

	printf("\n"); */
}
