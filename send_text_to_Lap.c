#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


// Output cmd: /bin/sendtoLap
// gcc ~/.sk/send_text_to_Lap.c -o ~/.sk/sendtoLAP; sudo mv ~/.sk/sendtoLAP /bin/

#define debug

#define NUMBER_OF_IPs 4	// ALSO CHANGE MATRIX DATA & FORK CREATIONS

typedef struct ip_matrix_struct
{
	 int matx_chproc_num;
	char matx_ip[16];
	char matx_nameof_ip[20];
}struct_sk;

	const int ips_cnt = NUMBER_OF_IPs;			// Number of IP addresses to be checked = 4
	int         pid_arr[NUMBER_OF_IPs+1]={0};		//  [5] == [ ips_cnt+1 ]  used as mainproc_pid stored at arr[0] 
	struct_sk ip_matrix[NUMBER_OF_IPs+1]={0};		//  [5] -> donot use [0] to keep logic less complicated


void main(int argc, char *argv[])
{

	int first_ret_pid, ret_chproc_num, chproc_num=0;

	char pid_str[9]={0};			// to convert INT(pid) into STRING(pid)
	char buffer_pid_killer[100]={0};
	char buffer_scp_cmd[500]={0};

	//printf("size = %ld \n\n", sizeof(ip_matrix));

	ip_matrix[1].matx_chproc_num = 1;  strcpy(ip_matrix[1].matx_ip,"172.31.84.172");   strcpy(ip_matrix[1].matx_nameof_ip,"Laptop-WiFi");
	ip_matrix[2].matx_chproc_num = 2;  strcpy(ip_matrix[2].matx_ip,"192.168.50.134");  strcpy(ip_matrix[2].matx_nameof_ip,"Cisco_VPN");
	ip_matrix[3].matx_chproc_num = 3;  strcpy(ip_matrix[3].matx_ip,"10.80.10.3");	   strcpy(ip_matrix[3].matx_nameof_ip,"NetExtender_VPN");
	ip_matrix[4].matx_chproc_num = 4;  strcpy(ip_matrix[4].matx_ip,"111.111.111.111"); strcpy(ip_matrix[4].matx_nameof_ip,"Laptop_Ethernet");

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

	pid_arr[0] = getpid();

	if( (chproc_num++, pid_arr[1]=fork()) && (chproc_num++, pid_arr[2]=fork()) && \
	    (chproc_num++, pid_arr[3]=fork()) && (chproc_num++, pid_arr[4]=fork()) )
	{
		chproc_num=0;		// for Parent, set chproc_num i.e. child_process_number = 0

		printf("in pid=%d parent, childproc_num=%d\n", pid_arr[0], chproc_num);
#ifdef debug
		for(int j=1; j<=ips_cnt; j++)
		{
			printf("\t\t pid_arr[%d]=%d \n", j, pid_arr[j]);
		}
		printf("\n");
#endif
		first_ret_pid=wait(0);

		printf(" \t\t KEEP this pid=%d \n", first_ret_pid);
		for(int j=1; j<=ips_cnt; j++)
		{
			if(pid_arr[j] != first_ret_pid)	// KILL these PID
			{
				sprintf(pid_str, "%d", pid_arr[j]);
				strcpy(buffer_pid_killer, "kill -9 ");	strcat(buffer_pid_killer, pid_str);

				printf(" \t\t buffer = %s \n", buffer_pid_killer);
				system(buffer_pid_killer);
			}
			else
			{
				ret_chproc_num=j;
			}
		}

		for(int j=1; j<=ips_cnt-1; j++)		// '-1' was done as already once wait(0) used to capture first_ret_pid
		{
#ifdef debug
			printf("\t\t\t wait returned after killing process pid %d. \n", wait(0) );
#else
			wait(0);
#endif
		}

		printf(" keep child process number %d\n", ret_chproc_num);
		printf(" access the IP address available for ret_chproc_num=%d",ret_chproc_num);

		strcpy(buffer_scp_cmd, "sshpass -f /home/shubham/.sk/.localpass.txt scp /home/shubham/.sk/send-THIS2Lap.txt shubham@");
		strcat(buffer_scp_cmd, "<IP_adds>");
		strcat(buffer_scp_cmd, ":/home/shubham/.sk/send-ReceivedFromPC.txt");

		printf("\n %s \n", buffer_scp_cmd);

			printf("\n Use: send-cat / send-view (GEDIT)  to view the content sent from Laptop");
			printf("\n Use: rec-cat  / rec-view  (GEDIT)  to view the content received from PC");

			printf("\n");

#ifdef debug
		printf("inside parent at end.. \n");
#endif
	}
	else			      //		 10	   20   25   30     37
	{			      //		 10	   20   25   30     37
	sleep(1);		      //0123456789        0123456789012345678901234567 
		strcpy(buffer_scp_cmd, "ping -c 1 ");	//xyz.xyz.xyz.xyz > /dev/null
		//strcpy(buffer_scp_cmd+10, "xyz.xyz.xyz.xyz");
		strcpy(buffer_scp_cmd+25, " > /dev/null");
		//strncpy(buffer_scp_cmd+10, "011.113.114.117        ", 15);



	switch (chproc_num)
	{//-----------------------------------------------------// |
	case 1: strncpy(buffer_scp_cmd+10, "0.0.0.2                ", 15);  printf("_%s_\n", buffer_scp_cmd); system(buffer_scp_cmd);break;

	case 3: strncpy(buffer_scp_cmd+10, "192.168.50.134         ", 15);  printf("_%s_\n", buffer_scp_cmd); system(buffer_scp_cmd);break;

	case 2: strncpy(buffer_scp_cmd+10, "10.80.10.3             ", 15);  printf("_%s_\n", buffer_scp_cmd); system(buffer_scp_cmd);break;

	case 4: strncpy(buffer_scp_cmd+10, "111.111.111.111        ", 15);  printf("_%s_\n", buffer_scp_cmd); system(buffer_scp_cmd);break;

	}
	}



/////////////

/*
"sshpass -f /home/shubham/.sk/.localpass.txt scp /home/shubham/.sk/send-THIS2Lap.txt shubham@xyz:/home/shubham/.sk/send-ReceivedFromPC.txt"
	
	printf("\n Use: send-cat / send-view (GEDIT)  to view the content sent from Laptop");
	printf("\n Use: rec-cat  / rec-view  (GEDIT)  to view the content received from PC");

	printf("\n"); */
}
