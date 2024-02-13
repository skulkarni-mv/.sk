#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Output cmd: /bin/sendtoPC
// gcc ~/.sk/send_text_to_PC.c -o ~/.sk/sendtoPC; sudo mv ~/.sk/sendtoPC /bin/


void main(int argc, char *argv[])
{
	char buffer[500];

	if(argc!=2)
	{
		printf("\n   Invalid. Usage:$ sendtoPC <content_to_be_sent OR sendtoPC (to share manually edited file)> \n");
		return;
	}

	if(strcmp(argv[0], argv[1]) != 0)	// check if used cmd is $ sendtoPC sendtoPC , if yes means file to be sent is edited manually
						// in this case, that edit file directly should be sent, not content passed on CLI
	{
		FILE *fw = fopen("/home/shubham/.sk/send-THIS2PC.txt", "w");
		fprintf(fw, "%s\n", argv[1]);
		fclose(fw);
	}
	else
	{
		printf("\n Received cmd: $ sendtoPC sendtoPC. Sending manually edited file ~/.sk/send-THIS2PC.txt \n");
	}

	strcpy(buffer, "scp /home/shubham/.sk/send-THIS2PC.txt shubham@10.80.244.104:/home/shubham/.sk/send-ReceivedFromLaptop.txt");

	system(buffer);
	printf("\n Use: send-cat / send-view (GEDIT)  to view the content sent from Laptop");
	printf("\n Use: rec-cat  / rec-view  (GEDIT)  to view the content received from PC");

	printf("\n");
}
