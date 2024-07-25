#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Usage: ./run_check_rejected_cves.out <csv_file_path>
// $ gcc read_CVE_id.c -o ./run_check_rejected_cves.out

int check_cveorg_rejected_CVE(unsigned int cve_count_passed);

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


	FILE *fp=fopen(argv[1], "r");
	if(fp)
	{
		printf(" Extracting CVE ids from the csv file...\n\n");
		fgets(buf_fgets,490,fp);	printf(" Ommiting first line -> %s  \n", buf_fgets); // went to the new-line

		FILE *fw=fopen("CVE_nums_extracted.txt", "w");		// Write Extracted CVE numbers into the file

		while( fgets(buf_fgets,490,fp) != NULL)
		{
			for(char i=0; i<strlen(buf_fgets); i++)
			{
				buf_bugID[i] = buf_fgets[i];
				if(buf_fgets[i] == ',') {
					buf_bugID[i]='\0';
					break;
				}
			}

			CVE_Start=strstr(buf_fgets, "CVE");	//CVE_Start =  CVE-2016-3709 libxml: Incorrect server side include OR

			if(CVE_Start)
			{
				for(int i=0; i<20; i++)
				{
					     if( *(CVE_Start + i) == ' ' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == ',' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == '.' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == ':' )	*(CVE_Start+i) = '\0';
				}


				if( strcmp(CVE_Start, CVE_prev) != 0) {		// Different CVEs

					fprintf(fw, "%s %s\n", buf_bugID, CVE_Start);
					count++;
					printf(" Bug_No = %s  :  CVE_ID = %s\n", buf_bugID, CVE_Start);
				}

				strcpy(CVE_prev, CVE_Start);

			}
//			printf(" Bug_No = %s  :  CVE_ID = %s\n", buf_bugID, CVE_Start);
		}

		fclose(fw);
		fclose(fp);
		printf("\n\t Found total %u valid CVE numbers. \n\n", count);

		if (count > 0) {
			check_cveorg_rejected_CVE(count);
		}
	}
	else
		perror("fopen");
}


int check_cveorg_rejected_CVE(unsigned int cve_count_passed)
{

	char buffer[500]={0}, buf_fscanf_CVE[20]={0}, buf_fscanf_BugID[10]={0}, rej_cve_stored[1000]={0};
	int  i=0, count_yes_rejected=0, sys_ret_val=0;
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

	while( fscanf(fp_cve_bugz, "%s", buf_fscanf_BugID) != EOF) {

		i++;
		printf(" %3d/%d - Bug ID: %s ", i, cve_count_passed, buf_fscanf_BugID);
		fflush(stdout);

		if( fscanf(fp_cve_bugz, "%s", buf_fscanf_CVE) != EOF) {

			strcpy(buffer, "python3 check_rejected_cve_json.py ");		// Check if CVE is Invalid
			strcat(buffer, buf_fscanf_CVE);

//			sleep(1);
			sys_ret_val = system(buffer);				// run "python3 check_rejected_cve_json.py CVE-abcd-1234"

			if(sys_ret_val == 0) {
				;
			}
			else if(sys_ret_val == 256) {
				count_yes_rejected++;

				if(strlen(rej_cve_stored) <= 950) {
					strcat(rej_cve_stored, buf_fscanf_BugID);
					strcat(rej_cve_stored, " - ");
					strcat(rej_cve_stored, buf_fscanf_CVE);
					strcat(rej_cve_stored, " \n");
				}
				else {
					strcat(rej_cve_stored, "** NOT ENOUGH SPACE, OVERFLOW DETECTED **\n")
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

	printf(" Found total %d CVEs as REJECTED out of %d CVEs \n\n", count_yes_rejected, cve_count_passed);

	printf("_\n%s_\n\n", rej_cve_stored);
}
