#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Usage: ./run_check_rejected_cves.out <csv_file_path>
// $ gcc read_CVE_id.c -o ./run_check_rejected_cves.out

int check_cveorg_rejected_CVE(unsigned int cve_count_passed);
void check_if_Ctrl_C_exception(int sys_ret_val_input, char *exception_num, int loop_number, int total_count);

char* inp_filename;

void main(int argc, char **argv)
{

	char buf_fgets[500]={0}, buf_bugID[10]={0};
	unsigned int count=0;
	char* CVE_Start=NULL, CVE_prev[20]={0};

	if(argc!=2)
	{
		printf("\n\t Invalid. Usage: ./check_rejected_cves.out <csv_file_full_path> \n\n");
		return;
	}

	inp_filename = argv[1];

	printf("\n ** Sorting input data wrt Summary/CVE ID is suggested, as it will avoid analysing same CVE again for other products **\n\n");
	sleep(2);
	printf(" ----------------------------------------------------------- \n");


	FILE *fp=fopen(inp_filename, "r");
	if(fp)
	{
		printf(" Extracting CVE ids from the csv file...\n\n");
		fgets(buf_fgets,490,fp);	printf(" Ommiting first line -> %s  \n", buf_fgets); // went to the new-line

		FILE *fw=fopen("CVE_nums_extracted.txt", "w");		// Write Extracted CVE numbers into the file

		while( fgets(buf_fgets,490,fp) != NULL)
		{

			for(char i=0; i<strlen(buf_fgets); i++) {

				buf_bugID[i] = buf_fgets[i];
				if( (buf_fgets[i] == ',') || (buf_fgets[i] == ' ') ) {
					buf_bugID[i]='\0';
					break;
				}
			}

			CVE_Start=strstr(buf_fgets, "CVE");	//CVE_Start =  CVE-2016-3709 libxml: Incorrect server side include OR

			if(CVE_Start) {

				for(int i=0; i<20; i++) {

					     if( *(CVE_Start + i) == ' ' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == ',' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == '.' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == ':' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == '\n')	*(CVE_Start+i) = '\0';
				}


				if( strcmp(CVE_Start, CVE_prev) != 0) {		// Different CVEs

					fprintf(fw, "%s %s\n", buf_bugID, CVE_Start);
					count++;
					printf(" Bug_No = %s  :  CVE_Id = %s\n", buf_bugID, CVE_Start);
				}
				strcpy(CVE_prev, CVE_Start);
			}
//			printf(" Bug_No = %s  :  CVE_ID = %s\n", buf_bugID, CVE_Start);
		}

		fclose(fw);
		fclose(fp);
		printf("\n\t Found total %u valid CVE numbers (Eliminated Repeatative CVEs). \n\n", count);

		if (count > 0) {
			check_cveorg_rejected_CVE(count);
		}
	}
	else
		perror("fopen");
}


int check_cveorg_rejected_CVE(unsigned int cve_count_passed)
{

	char buffer[500]={0}, buf_fscanf_CVE[20]={0}, buf_fscanf_BugID[10]={0}, cve_ids_rej_list[1000]={0};
	int  i=0, count_yes_rejected=0, sys_ret_val=0;
	char i_str[5]={0}, cve_count_passed_str[5]={0};
	FILE *fp_cve_bugz=NULL;


	printf("\n");
	printf(" ----------------------------------------------------------- \n");
	printf(" Do you want to check the CVE.ORG website for available fix? \n\n");
	printf(" ------ Press Enter to continue / Ctrl^C to Terminate ------ \n\n");
	getchar();


	fp_cve_bugz=fopen("CVE_nums_extracted.txt", "r");			// Read Extracted CVE numbers file
	if(fp_cve_bugz==NULL) {
		perror("fopen");
		exit(1);
	}

	while( fscanf(fp_cve_bugz, "%s", buf_fscanf_BugID) != EOF)
	{
		i++;
		printf(" %3d/%d - Bug ID: %s ", i, cve_count_passed, buf_fscanf_BugID);
		fflush(stdout);

		if( fscanf(fp_cve_bugz, "%s", buf_fscanf_CVE) != EOF) {

			strcpy(buffer, "python3 check_rejected_cve_json.py ");		// Check if CVE is Invalid
			strcat(buffer, buf_fscanf_CVE);

			sys_ret_val = system(buffer);				// run "python3 check_rejected_cve_json.py CVE-abcd-1234"

			check_if_Ctrl_C_exception(sys_ret_val, "1", i, cve_count_passed);

			if(sys_ret_val == 0) {					// 'check_rejected_cve_json.py' returns 0
				;
			}
			else if(sys_ret_val == 256) {				// CVE is rejected, 'check_rejected_cve_json.py' returns 1
				count_yes_rejected++;

				strcpy(buffer, "python3 check_rejected_cve.py ");		// Check if CVE is Invalid
				strcat(buffer, buf_fscanf_CVE);
				sys_ret_val = system(buffer);

				check_if_Ctrl_C_exception(sys_ret_val, "2", i, cve_count_passed);

				printf("\n------------------------------------------------------------\n\n");

				if(strlen(cve_ids_rej_list) <= 450) {

					sprintf(i_str, "%d", i);
					sprintf(cve_count_passed_str, "%d", cve_count_passed);

					strcat(cve_ids_rej_list, buf_fscanf_CVE);
/*					strcat(cve_ids_rej_list, "______loop:");	// for debug_change  (below also, search 'debug_change')
					strcat(cve_ids_rej_list, i_str);
					strcat(cve_ids_rej_list, "/");
					strcat(cve_ids_rej_list, cve_count_passed_str);
*/					strcat(cve_ids_rej_list, "\n");
				}
				else {
					strcat(cve_ids_rej_list, "** NOT ENOUGH SPACE, OVERFLOW DETECTED **\n");
				}
			}
			else {							// check if ^C returns 2 or not
				printf("\n\t Unhandled Exception #2. system() function returned '%d'. Exiting... \n\n", sys_ret_val);
				break;
			}
		}
//		printf("\n");
	}
	fclose(fp_cve_bugz);

	printf("_______________________________________________________________________________________________________________________\n\n");

	char single_cve[15]={0}, cve_id_len=0;
	char *head=NULL, *tail=NULL;

	head = cve_ids_rej_list;

	char buf_fgets[500]={0}, buf_bugID[10]={0}, *CVE_Start=NULL;

	FILE *fp=fopen(inp_filename, "r");			// Re-open input file once agian to fetch all bugs against rejected CVEs
	if(!fp) {
		perror("fopen");
		exit(1);
	}


	system("rm temp_comment_file.tmp 1> /dev/null 2>&1");
	FILE *fp_file_append=NULL;

	for(i=0; i<count_yes_rejected; i++)			// Will run for number CVE are Rejected from Input
	{
		head=strstr(head, "CVE-");
		tail=strchr(head, '\n');
		//tail=strchr(head, '_');				// if debug_change was done

		if(tail && head) {
			cve_id_len = tail-head;
			strncpy(single_cve, head, cve_id_len);
			single_cve[cve_id_len] = '\0';

//			printf("******** len=%d, id=_%s_\n", cve_id_len, single_cve);
		}
		head = head+1;


		fp_file_append=fopen("temp_comment_file.tmp", "a+");	// append data to file, Will be appended by python file as well

		rewind(fp);

		while( fgets(buf_fgets, 490, fp) != NULL)			// loop through the file
		{

			for(char j=0; j<strlen(buf_fgets); j++) {

				buf_bugID[j] = buf_fgets[j];
				if( (buf_fgets[j] == ',') || (buf_fgets[j] == ' ') ) {
					buf_bugID[j]='\0';
					break;
				}
			}

			CVE_Start=strstr(buf_fgets, "CVE");	//CVE_Start =  CVE-2016-3709 libxml: Incorrect server side include OR

			if(CVE_Start) {

				for(int i=0; i<20; i++) {

					     if( *(CVE_Start + i) == ' ' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == ',' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == '.' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == ':' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == '\n')	*(CVE_Start+i) = '\0';
				}

				if( strcmp(CVE_Start, single_cve) == 0 ) {

						fprintf(fp_file_append, "%s - %s ", buf_bugID, CVE_Start);
						fprintf(fp_file_append, "\t\t http://bugz.mvista.com/show_bug.cgi?id=%s \n", buf_bugID);
				}
			}

		} // while() ENDS
///////
		fclose(fp_file_append);

		printf("\n Please wait... generating Output data for Rejected CVEs -  %3d/%d ", i+1, count_yes_rejected);
		fflush(stdout);

		strcpy(buffer, "python3 check_rejected_cve.py ");		// Check if CVE is Invalid
		strcat(buffer, single_cve);
		strcat(buffer, " --optional RedirectOutput");
		sys_ret_val = system(buffer);

		check_if_Ctrl_C_exception(sys_ret_val, "3", 0, 0);

		fp_file_append=fopen("temp_comment_file.tmp", "a+");
		fprintf(fp_file_append, "\n----------------------------------------------------------\n\n");
		fclose(fp_file_append);

	} // for() ENDS
	fclose(fp);

	printf("\n_______________________________________________________________________________________________________________________\n\n");
	printf(" Found total %d CVEs as REJECTED out of %d CVEs: \n", count_yes_rejected, cve_count_passed);

	printf("%s\n", cve_ids_rej_list);
	printf("\n_______________________________________________________________________________________________________________________\n\n");
	printf(" ***** Following Bugs can be Marked Invalid which are against Rejected CVEs:\n\n\n");

	system("cat temp_comment_file.tmp");
}


void check_if_Ctrl_C_exception(int sys_ret_val_input, char *exception_num, int loop_number, int total_count)
{
	if(sys_ret_val_input == 2) {
		printf("\n\t Unhandled Exception #%s in loop %d/%d. system() function returned '%d'. Exiting... \n\n", \
					exception_num, loop_number, total_count, sys_ret_val_input);
		exit(1);
	}
}
