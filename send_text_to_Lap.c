#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Output cmd: /bin/sendtoLap
// gcc ~/.sk/send_text_to_Lap.c -o ~/.sk/sendtoLAP; sudo mv ~/.sk/sendtoLAP /bin/


//#define debug

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

	ip_matrix[1].matx_chproc_num = 1;  strcpy(ip_matrix[1].matx_ip,"172.31.84.172");  strcpy(ip_matrix[1].matx_nameof_ip,"Office_WiFi");
	ip_matrix[2].matx_chproc_num = 2;  strcpy(ip_matrix[2].matx_ip,"192.168.50.160"); strcpy(ip_matrix[2].matx_nameof_ip,"Cisco_VPN");
	ip_matrix[3].matx_chproc_num = 3;  strcpy(ip_matrix[3].matx_ip,"10.80.10.30");    strcpy(ip_matrix[3].matx_nameof_ip,"NetExtender_VPN");
	ip_matrix[4].matx_chproc_num = 4;  strcpy(ip_matrix[4].matx_ip,"10.80.244.125");  strcpy(ip_matrix[4].matx_nameof_ip,"Office_Ethernet");

	if(argc!=2)
	{
		printf("\n   Invalid. Usage:$ sendtoLap <content_to_be_sent OR sendtoLap (to share manually edited file)> \n");
		return;
	}

	if(strcmp(argv[0], argv[1]) == 0)	// check if used cmd is $ sendtoLap sendtoLap , if yes means file to be sent is edited manually
						// in this case, that edit file directly should be sent, not content passed on CLI
	{
		printf("\n Received cmd: $ sendtoLap sendtoLap. Sending manually edited file ~/.sk/.sk_gitignore/send-THIS2Lap.txt \n");
	}
	else 					// else if(strcmp(argv[0], argv[1]) != 0) - Used cmd is $ sendtoLap NOTEQUAL to "sendtoLap" string
	{
		FILE *fw = fopen("/home/shubham/.sk/.sk_gitignore/send-THIS2Lap.txt", "w");
		fprintf(fw, "%s\n", argv[1]);
		fclose(fw);
	}


/////////////

	pid_arr[0] = getpid();

	if( (chproc_num++, pid_arr[1]=fork()) && (chproc_num++, pid_arr[2]=fork()) && \
	    (chproc_num++, pid_arr[3]=fork()) && (chproc_num++, pid_arr[4]=fork()) )
	{	// Will be executed once by Parent main() process
	
		chproc_num=0;		// for Parent, set chproc_num i.e. child_process_number = 0

		printf("fork() %d times. main() parent_pid=%d.\n", NUMBER_OF_IPs, pid_arr[0]);
#ifdef debug
		for(int j=1; j<=ips_cnt; j++)
		{
			printf("\t\t pid_arr[%d]=%d \n", j, pid_arr[j]);
		}
		printf("\n");
#endif
		first_ret_pid=wait(0);

//		printf(" \t\t KEEP this pid=%d \n", first_ret_pid);
		for(int j=1; j<=ips_cnt; j++)
		{
			if(pid_arr[j] != first_ret_pid)	// KILL these PIDs
			{
				sprintf(pid_str, "%d", pid_arr[j]);
				strcpy(buffer_pid_killer, "kill -9 ");	strcat(buffer_pid_killer, pid_str);

				printf(" \t\t buffer = %s [%s]\n", buffer_pid_killer, ip_matrix[j].matx_nameof_ip);
				system(buffer_pid_killer);
			}
			else
			{
				ret_chproc_num=j;
			}
		}
		printf("\n");
		printf(" \t\t KEEP THIS    pid=%d [%s]\n", first_ret_pid, ip_matrix[ret_chproc_num].matx_nameof_ip);
		printf("\n");

		for(int j=1; j<=ips_cnt-1; j++)		// '-1' was done as already once wait(0) used to capture first_ret_pid
		{
#ifdef debug
			printf("\t\t\t wait returned after killing process pid %d. \n", wait(0) );
#else
			wait(0);
#endif
		}
#ifdef debug
		printf("\n KEEP child process number %d\n", ret_chproc_num);
#endif

		strcpy(buffer_scp_cmd, "sshpass -v -f /home/shubham/.sk/.sk_gitignore/.localpass.txt ");
		strcat(buffer_scp_cmd, "scp /home/shubham/.sk/.sk_gitignore/send-THIS2Lap.txt shubham@");
		strcat(buffer_scp_cmd, ip_matrix[ret_chproc_num].matx_ip);
		strcat(buffer_scp_cmd, ":/home/shubham/.sk/.sk_gitignore/send-ReceivedFromPC.txt");

//		printf("\n %s \n", buffer_scp_cmd);
		system(buffer_scp_cmd);

			printf("\n Use: send-cat / send-view (GEDIT)  to view the content sent from Laptop");
			printf("\n Use: rec-cat  / rec-view  (GEDIT)  to view the content received from PC");

			printf("\n");

#ifdef debug
		printf("inside parent at end.. \n");
#endif
	}// if() for parent ends
	
	else // Will be Executed by 4xfork()ed Child Processes
	{			      //		 10	   20   25   30     37		// Reference for "strncpy(buffer_scp_cmd+..." command proper output for all possible IPs
				      //		 10	   20   25   30     37		// Reference for "strncpy(buffer_scp_cmd+..." command proper output for all possible IPs
				      //0123456789        0123456789012345678901234567	// Reference for "strncpy(buffer_scp_cmd+..." command proper output for all possible IPs
		strcpy(buffer_scp_cmd, "ping -c 1 ");	   //xyz.xyz.xyz.xyz > /dev/null
		strcpy(buffer_scp_cmd+25, " > /dev/null");

		char tmp_cp_buff[24]={0};

		switch (chproc_num)
		{
		case 1: 						strcpy(tmp_cp_buff, ip_matrix[1].matx_ip);	for(int j=strlen(tmp_cp_buff); j<23; j++){tmp_cp_buff[j]=' ';}
			strncpy(buffer_scp_cmd+10, tmp_cp_buff, 15);						//printf("_%s_\n", buffer_scp_cmd);

			if( system(buffer_scp_cmd) != 0) {printf("\n\t Please check the IP Matrix.\t FAILED \t to connect any IP \n\n\n");} break;

		case 2: 						strcpy(tmp_cp_buff, ip_matrix[2].matx_ip);	for(int j=strlen(tmp_cp_buff); j<23; j++){tmp_cp_buff[j]=' ';}
			strncpy(buffer_scp_cmd+10, tmp_cp_buff, 15);  					//printf("_%s_\n", buffer_scp_cmd);

			if( system(buffer_scp_cmd) != 0) {printf("\n\t Please check the IP Matrix.\t FAILED \t to connect any IP \n\n\n");} break;

		case 3: 						strcpy(tmp_cp_buff, ip_matrix[3].matx_ip);	for(int j=strlen(tmp_cp_buff); j<23; j++){tmp_cp_buff[j]=' ';}
			strncpy(buffer_scp_cmd+10, tmp_cp_buff, 15);  					//printf("_%s_\n", buffer_scp_cmd);

			if( system(buffer_scp_cmd) != 0) {printf("\n\t Please check the IP Matrix.\t FAILED \t to connect any IP \n\n\n");} break;

		case 4: 						strcpy(tmp_cp_buff, ip_matrix[4].matx_ip);	for(int j=strlen(tmp_cp_buff); j<23; j++){tmp_cp_buff[j]=' ';}
			strncpy(buffer_scp_cmd+10, tmp_cp_buff, 15);  					//printf("_%s_\n", buffer_scp_cmd);

			if( system(buffer_scp_cmd) != 0) {printf("\n\t Please check the IP Matrix.\t FAILED \t to connect any IP \n\n\n");} break;
		}
	}

}
