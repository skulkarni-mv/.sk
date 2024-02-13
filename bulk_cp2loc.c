#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Output cmd: bulkcp2
//gcc ~/.sk/bulk_cp2loc.c -o ~/.sk/bulkcp2; sudo mv ~/.sk/bulkcp2 /bin/

int time_sec_per_file=12;	// take 12 sec for each file copy

//-------------------------------------------------------------------------------- For Timer
FILE* fp_time=NULL;
char t_now[10]={0}, t_orig[10]={0}, t_diff[10]={0};
time_t t_start, t_end;

void start_timer_sk(char *argv_ptr);
void format_time_string(time_t t_sec, int add_sec, char * t_str);
void stop_timer_sk(void);
//-------------------------------------------------------------------------------- For Timer


void issue_copy_cmd(char *file_path, char *file_name, char *argv1, char *ETA_time, char print_on_screen)
{
	char buffer[200]={0}, print_line[300]={0}, redir_op[200]={0};
	unsigned int i=0, diff=0;

	strcpy(buffer, "scp qauser@10.80.200.8:");
	strcat(buffer, file_path);
	strcat(buffer,"/");
	strcat(buffer, file_name);

	strcat(buffer, " ");

	strcat(buffer, argv1);
	strcat(buffer,"/");		// "scp qauser@10.80.200.8:"+"/home/qauser"+"/"+".sk/example.txt"+" "+"/home/shubham/example_dir"+"/"
//----------------------------------------------------------------------------

	if(print_on_screen)
	{
		strcpy(print_line, "\nIssued Command : ");
		strcat(print_line, buffer);

		for(i=0; print_line[i]; i++);	

		diff = 145+1 - strlen(ETA_time);

		if(i>=diff)	diff=diff+150;		// for longer command

		while(i<diff)				// To print ETA at exact place for each file
			print_line[i++] = ' ';
	
		strcat(print_line, ETA_time);	
		printf("%s", print_line);
		printf("\n");
		
		system(buffer);		//system("scp qauser@10.80.200.8:/home/qauser/.sk/example.txt /home/shubham/example_dir/");
		//printf("\n");
	}
	else
	{
		strcpy(redir_op, buffer);
		strcat(redir_op, " > ~/.sk/tmp_redir_op.txt");
		system(redir_op);      //system("scp qauser@10.80.200.8:/home/qauser/.sk/example.txt .../example_dir/ > ~/.sk/tmp_redir_op.txt");
	}
}

void get_ETA_mm_ss(int ETA_int_sec, char *ETA_str_ptr)
{
	char mm_ss[10]={0};
	char *space_found = NULL;
	int sec=0, min=0;

	min = ETA_int_sec / 60;
	sec = ETA_int_sec % 60;

	sprintf(mm_ss, "%2d:%2d", min, sec);

        while((space_found=strchr(mm_ss,' ')))  // space found is NOT NULL
                *space_found = '0';  

	strcpy(ETA_str_ptr, "Total ETA ");
	strcat(ETA_str_ptr, mm_ss);

}

void main(int argc, char *argv[])
{	
	char buffer[500]={0}, file_dir_is[100]={0}, ETA_str[50]={0}, copying_files[50]={0};
	char last_char =0;
	int no_of_files_ls = 0, i, ETA_int_sec;
	FILE *fp_ls=NULL, *fp_dir=NULL;	


	if(argc!=2)
	{
		printf("\n    Invalid. Usage:$ bulkcp2 <new_folder_name_to_paste> \t ERROR:1\n\n");
		return;
	}


		start_timer_sk(argv[1]);	//******************************  For Timer ****************************//

	printf(" Initiating... Checking number of files to be copied... \n");
	system("rm /home/shubham/.sk/bulkcp_ls_is.txt");						     // Remove prev file to avoid conflict
	issue_copy_cmd("/home/qauser/.sk", "bulkcp_ls_is.txt",  "/home/shubham/.sk", "Calculating ETA", 1);	//"Calculating ETA" "01:25"

	fp_ls=fopen("/home/shubham/.sk/bulkcp_ls_is.txt", "r");
	if(fp_ls)
	{
		while(fgets(buffer, 100, fp_ls) != NULL)
			no_of_files_ls ++;	
	}
	else
	{
		printf("\n\t Not Found. File \"/home/shubham/.sk/bulkcp_ls_is.txt\" Unavailable / Copying from VM failed.\t ERROR:2\n");

		stop_timer_sk();	//*** For Timer ***//
		return;
	}

	if(no_of_files_ls == 0)		// No files to copy
	{
		printf("\n\t No files found to copy in \"/home/shubham/.sk/bulkcp_ls_is.txt\". no_of_files_ls = 0. \t");
		printf(" Copying \"bulkcp_dir_is.txt\" from \"/home/qauser/.sk\" \n");

		system("rm /home/shubham/.sk/bulkcp_dir_is.txt");					     // Remove prev file to avoid conflict
		issue_copy_cmd("/home/qauser/.sk", "bulkcp_dir_is.txt", "/home/shubham/.sk", "", 0);

		fp_dir=fopen("/home/shubham/.sk/bulkcp_dir_is.txt", "r");
		if(fp_dir)
		{
			printf("\t No Copyable files available in VM directory: ");	fflush(stdout);
			system("cat /home/shubham/.sk/bulkcp_dir_is.txt");

			fclose(fp_dir);
		}
		else
		     printf("\n\t Not Found. File \"/home/shubham/.sk/bulkcp_dir_is.txt\" Unavailable / Copying from VM failed.\t ERROR:3\n");
		
		fclose(fp_ls);

		stop_timer_sk();	//*** For Timer ***//
		return;			// return in both cases.
	}	

	/////////////////////////////////////////////////////////////////////////////////////
	
	no_of_files_ls++;		// one extra file bulkcp_dir_is

	ETA_int_sec = no_of_files_ls * time_sec_per_file;
	get_ETA_mm_ss(ETA_int_sec, ETA_str);

	sprintf(copying_files, "Copying %d more files     ", no_of_files_ls);
	strcat(copying_files, ETA_str);

	system("rm /home/shubham/.sk/bulkcp_dir_is.txt");						     // Remove prev file to avoid conflict
	issue_copy_cmd("/home/qauser/.sk", "bulkcp_dir_is.txt", "/home/shubham/.sk", copying_files, 1);

	fp_dir=fopen("/home/shubham/.sk/bulkcp_dir_is.txt", "r");
	if(fp_dir)
	{
		printf("\n Creating New Directory : %s \n", argv[1]);
		strcpy(buffer, "mkdir ");
		strcat(buffer, argv[1]);
		system(buffer);			// system("mkdir <copied_content>");
	
		//------------------------------------------------------------------//

		fscanf(fp_dir, "%s", file_dir_is);
		fclose(fp_dir);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		rewind(fp_ls);
		while(fgets(buffer, 100, fp_ls) != NULL)
		{	
			for(i=0;buffer[i];i++);				// buffer has '\n' at the end automatically
			buffer[i-1] = '\0';

			ETA_int_sec = ETA_int_sec - time_sec_per_file;
			get_ETA_mm_ss(ETA_int_sec, ETA_str);

			issue_copy_cmd(file_dir_is, buffer, argv[1], ETA_str, 1);
		}

		fclose(fp_ls);
	}
	else
		printf("\n	Not Found. File \"/home/shubham/.sk/bulkcp_dir_is.txt\" Unavailable / Copying from VM failed.\t ERROR:4\n");
		
	
	stop_timer_sk();	//******************************  For Timer ****************************//
}

//**************************************************************************************************************************************//


void start_timer_sk(char *argv_ptr)
{
	system("pwd > /home/shubham/.sk/info_build.txt");		// Newly written Everytime

	fp_time=fopen("/home/shubham/.sk/info_build.txt","r+");

	fseek(fp_time, -1, SEEK_END);					// Replace \n at the end
	fprintf(fp_time, "/	cmd: bulkcp2  + mkdir:%s \n", argv_ptr);

           t_start = time(NULL);

	format_time_string(t_start,     0, t_orig);
 
	fprintf(fp_time, " Start Time:    %s \n", t_orig);

}

void stop_timer_sk(void)
{
           t_end = time(NULL);
		 
	format_time_string(t_end,     0, t_orig);

	fprintf(fp_time, "Finish Time:    %s \n", t_orig);

	 int sec = t_end-t_start;
	 int hh = sec/3600;
	 int mm = (sec - hh*3600)/60;
	 int ss = sec - hh*3600 - mm*60;

	sprintf(t_diff,"%2d:%2d:%2d", hh, mm, ss);

	char * space_found=NULL;
	while((space_found=strchr(t_diff,' ')))	// space found is NOT NULL
		*space_found = '0';		// replace space with '0'

	fprintf(fp_time, "Execution Time: %s  \n", t_diff);
	fclose(fp_time);

	printf("\n");
	system("cat /home/shubham/.sk/info_build.txt");
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
