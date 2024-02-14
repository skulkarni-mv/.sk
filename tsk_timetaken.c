#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// Output cmd: /bin/tsk
// gcc ~/.sk/tsk_timetaken.c -o ~/.sk/tsk; sudo mv ~/.sk/tsk /bin/

//#define VM			/// Comment for Local Machine, Uncomment for Virtual Machine

void format_time_string(time_t t_sec, int add_sec, char * t_str);

int main(int argc, char *argv[])
{
	char t_now[10]={0}, t_orig[10]={0}, t_diff[10]={0};
	time_t t_start, t_end;
	char buf_cmd_op[500]={0}, fp_cmd_op[500]={0};///Load data into String from File to avoid info curruption due to Overwrite by another "tsk"
	int return_val=0;

	if(argc != 2)
	{
		printf("\n No command passed. Usage: tsk <cmd_to_calc_time_of_execution>\n\n");
		return -1;
	}

	system("pwd > /home/shubham/.sk/.sk_gitignore/info_build.txt");		// Newly written Everytime

	FILE* fp=fopen("/home/shubham/.sk/.sk_gitignore/info_build.txt","r+");

	fseek(fp, -1, SEEK_END);				// Replace \n at the end
	fprintf(fp, "/	cmd: %s \n", argv[1]);			// Add \n Manually

           t_start = time(NULL);

	format_time_string(t_start,     0, t_orig);
        format_time_string(t_start, 19800, t_now);
 
	#ifdef VM
	fprintf(fp, " Start Time:       for VM: %s \t for Docker: %s \n", t_orig, t_now);
	#else
	fprintf(fp, " Start Time:    %s \n", t_orig);
	#endif	

	fseek(fp, 0, SEEK_SET);
	fgets(buf_cmd_op, 490, fp);			strcpy(fp_cmd_op, buf_cmd_op);		/// /home/shubham/ cmd: ls -> READS till '\n'
	fgets(buf_cmd_op, 490, fp);			strcat(fp_cmd_op, buf_cmd_op);		///  Start Time:  00:00:00 -> READS till '\n'

	fclose(fp);	/// Close file as Next Command "system(argv[1]);" may take time depending on the argv[1]

	printf("Running: %s -> %s \n", argv[1], buf_cmd_op); fflush(stdout);    //fflush added as if o/p redir used '>' not ptinting correct locn

/*****************************************************************************************************************/
	if( system(argv[1]) != 0) { return_val=1; printf("\n\t Error Occurred in 'system()' Execution \n\n"); fflush(stdout); }
/*****************************************************************************************************************/

           t_end = time(NULL);
		 
	format_time_string(t_end,     0, t_orig);
        format_time_string(t_end, 19800, t_now);

///   	fprintf(fp, "Finish Time:       for VM: %s \t for Docker: %s \n", t_orig, t_now);
	#ifdef VM
	sprintf(buf_cmd_op, "Finish Time:       for VM: %s \t for Docker: %s \n", t_orig, t_now);
	#else
	sprintf(buf_cmd_op, "Finish Time:    %s \n", t_orig);
	#endif

	strcat(fp_cmd_op, buf_cmd_op);	/// Finish Time:  00:00:00

	 int sec = t_end-t_start;
	 int hh = sec/3600;
	 int mm = (sec - hh*3600)/60;
	 int ss = sec - hh*3600 - mm*60;

	sprintf(t_diff,"%2d:%2d:%2d", hh, mm, ss);

	char * space_found=NULL;
	while((space_found=strchr(t_diff,' ')))	// space found is NOT NULL
		*space_found = '0';		// replace space with '0'

///	fprintf(fp, "Execution Time: %s  \n", t_diff);
	sprintf(buf_cmd_op, "Execution Time: %s  \n", t_diff);	strcat(fp_cmd_op, buf_cmd_op);	/// Execution Time:  00:00:00 

	
///	printf("\n%s\n", fp_cmd_op);				/// Print Data from file, Comment the String Printing
	
	fp=fopen("/home/shubham/.sk/.sk_gitignore/info_build.txt","w");	/// Reopen file in Write Mode to erase previous contents and load New Data
	fputs(fp_cmd_op, fp);					/// Write Data in the String into the file for retaining the build time in File
	fclose(fp);						/// Close the file to save the content in the Disk


	printf("\n");   fflush(stdout);
	system("cat /home/shubham/.sk/.sk_gitignore/info_build.txt");		// Print the contents of the File

	//return return_val;	
}								///->represents Changes on 07/09/2022 to avoid info overwrite due to another "tsk"

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


/*

	/// Previous void main() before change on 07/09/2022 -> kept for reference

void main(int argc, char *argv[])
{
	char t_now[10]={0}, t_orig[10]={0}, t_diff[10]={0};
	time_t t_start, t_end;

	if(argc != 2)
	{
		printf("\n No command passed. Usage: skbuild <cmd_to_calc_time_of_execution>\n\n");
		return;
	}

	system("pwd > /home/shubham/.sk/info_build.txt");		// Newly written Everytime

	FILE* fp=fopen("/home/shubham/.sk/info_build.txt","r+");

	fseek(fp, -1, SEEK_END);				// Replace \n at the end
	fprintf(fp, "/	cmd: %s \n", argv[1]);

           t_start = time(NULL);

	format_time_string(t_start,     0, t_orig);
 
	fprintf(fp, " Start Time:    %s \n", t_orig);

//*****************************************************************************************************************
	system(argv[1]);
//*****************************************************************************************************************

           t_end = time(NULL);
		 
	format_time_string(t_end,     0, t_orig);

	fprintf(fp, "Finish Time:    %s \n", t_orig);

	 int sec = t_end-t_start;
	 int hh = sec/3600;
	 int mm = (sec - hh*3600)/60;
	 int ss = sec - hh*3600 - mm*60;

	sprintf(t_diff,"%2d:%2d:%2d", hh, mm, ss);

	char * space_found=NULL;
	while((space_found=strchr(t_diff,' ')))	// space found is NOT NULL
		*space_found = '0';		// replace space with '0'

	fprintf(fp, "Execution Time: %s  \n", t_diff);
	fclose(fp);

	printf("\n");
	system("cat /home/shubham/.sk/info_build.txt");
	
}

*/
