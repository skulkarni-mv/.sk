#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// Output cmd: /bin/sendtoLap
// gcc ~/.sk/send_text_to_Lap.c -o ~/.sk/sendtoLAP; sudo mv ~/.sk/sendtoLAP /bin/


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

	int mainpid = getpid();	printf(" main proc pid = %d \n\n", mainpid);

	if(fork() == 0)	// newproc1 enters with ppid of mainproc	// without VPN on wifi
	{
		printf("proc1 pid=%d, ppid=%d \n", getpid(), getppid());
		if(system("ping -c 1 172.31.84.172 > /dev/null") == 0) // wifi is connected, so ping is working
		{
			printf("Lap connected to wifi IP: 172.31.84.172 \n\n");
			system("sshpass -f /home/shubham/.sk/.sk_gitignore/.localpass.txt scp /home/shubham/.sk/.sk_gitignore/send-THIS2Lap.txt   shubham@172.31.84.172:/home/shubham/.sk/.sk_gitignore/send-ReceivedFromPC.txt");

			printf("\n Use: send-cat / send-view (GEDIT)  to view the content sent from Laptop");
			printf("\n Use: rec-cat  / rec-view  (GEDIT)  to view the content received from PC");
		}

	}
									else	// mainproc enters here
									{
										if(fork() == 0)	// newproc2 enters with ppid of mainproc
										{
											printf("proc2 pid=%d, ppid=%d \n", getpid(), getppid());
										}
									}



/*	strcpy(buffer, "sshpass -f /home/shubham/.sk/.localpass.txt scp /home/shubham/.sk/send-THIS2Lap.txt shubham@172.31.84.172:/home/shubham/.sk/send-ReceivedFromPC.txt");

	system(buffer);
	
		strcpy(buffer, "sshpass -f /home/shubham/.sk/.localpass.txt scp /home/shubham/.sk/send-THIS2Lap.txt shubham@192.168.50.134:/home/shubham/.sk/send-ReceivedFromPC.txt");
		system(buffer);
	
	printf("\n Use: send-cat / send-view (GEDIT)  to view the content sent from Laptop");
	printf("\n Use: rec-cat  / rec-view  (GEDIT)  to view the content received from PC");

	printf("\n"); */
}
