#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// Output cmd: /bin/loop
// gcc ~/.sk/loop_cmd_sk.c -o ~/.sk/loop; sudo mv ~/.sk/loop /bin/

int max_min_limit = 120;		// set time in minute, after these many minutes, while loop will break

void format_time_string(time_t t_sec, int add_sec, char * t_str);

void main(int argc, char *argv[])
{
	int sleep_time = 5; 	// default sleep time
	int max_sec_limit;

	char t_now[10]={0}, t_orig[10]={0}, t_diff[10]={0}, t_str_temp[10]={0};
	time_t t_start, t_end, t_temp;
	char print_msg_start[200]={0};

	char *cmd_buffer = argv[1];	// Added on 11/10/22 fot incorporating 'cnt' custom command

	if(argc < 3)
	{
		printf("\n\t Invalid. Usage \"loop\" <cmd_in_loop> <sec_value_of_delay> <OPT:max_looping_time_minutes_DEFALT:120min>\n");
		return;
	}

	if(argc == 4)
	{
		max_min_limit = atoi(argv[3]);     // Convert string into int
	}
	max_sec_limit = 60 * max_min_limit;

        t_start = time(NULL);

	format_time_string(t_start,0+19800, t_orig);
	format_time_string(t_start, 19800, t_now);

	strcpy(print_msg_start, " Start Time: 	for VM: ");
	strcat(print_msg_start, t_orig);
	strcat(print_msg_start, " \t for Docker: ");
	strcat(print_msg_start, t_now);			// Add \n explicitely

	/**************************************************************************************************/

	if(strcmp(argv[1], "cnt") == 0)
		cmd_buffer = "ls | wc -l";	// Added on 11/10/22
	
	sleep_time = atoi(argv[2]);	// Convert string into int
	while(1)
	{
		t_temp = time(NULL);
		format_time_string(t_temp,0+19800, t_str_temp);
		printf(" %s \n", t_str_temp);

		system(cmd_buffer);
		sleep(sleep_time);

		printf("\n");

		if(t_temp - t_start >= max_sec_limit)
			break;
	}

	/**************************************************************************************************/

        t_end = time(NULL);
		 
	format_time_string(t_end,0+19800, t_orig);
	format_time_string(t_end, 19800, t_now);


	printf("%s \n", print_msg_start);
	printf("Finish Time:	for VM: %s \t for Docker: %s \n", t_orig, t_now);


	 int sec = t_end-t_start;
	 int hh = sec/3600;
	 int mm = (sec - hh*3600)/60;
	 int ss = sec - hh*3600 - mm*60;

	sprintf(t_diff,"%2d:%2d:%2d", hh, mm, ss);

	char * space_found=NULL;
	while((space_found=strchr(t_diff,' ')))	// space found is NOT NULL
		*space_found = '0';		// replace space with '0'

	printf("Execution Time:         %s  \n", t_diff);
}

void format_time_string(time_t t_sec, int add_sec, char * t_str)
{
	char *space_found = NULL;
	struct tm tm;
	
	t_sec = t_sec + add_sec;

	tm = *localtime(&t_sec);

	sprintf(t_str, "%2d:%2d:%2d", tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	while((space_found=strchr(t_str,' ')))	// space found is NOT NULL
		*space_found = '0';		// replace space with '0'
	
}
