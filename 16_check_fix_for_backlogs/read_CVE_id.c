#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Usage: ./check_cve_fix.out <csv_file_path>
// $ gcc read_CVE_id.c -o check_cve_fix.out

int check_ubuntu_cve_link(unsigned int);

void main(int argc, char **argv)
{

	char buf_fgets[500]={0}, buf_bugID[10]={0};
	unsigned int count=0;

	if(argc!=2)
	{
		printf("\n\t Invalid. Usage: ./check_cve_fix.out <csv_file_full_path> \n\n");
		return;
	}


	FILE *fp=fopen(argv[1], "r");
	if(fp)
	{
		fgets(buf_fgets,490,fp);	printf(" ommiting first line ->\n%s  \n", buf_fgets); // went to the new-line

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

			char* CVE_Start=strstr(buf_fgets, "CVE");	//CVE_Start =  CVE-2016-3709 libxml: Incorrect server side include OR

			if(CVE_Start)
			{
				for(int i=0; i<20; i++)
				{
					     if( *(CVE_Start + i) == ' ' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == ',' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == '.' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == ':' )	*(CVE_Start+i) = '\0';
				}

				fprintf(fw, "%s %s\n", buf_bugID, CVE_Start);
				count++;
			}
			printf(" Bug_No = %s  :  CVE_ID = %s\n", buf_bugID, CVE_Start);
		}

		fclose(fw);
		fclose(fp);
		printf("\n\t Found total %u valid CVE numbers. \n\n", count);

		if (count > 0) {
			check_ubuntu_cve_link(count);
		}
	}
	else
		perror("fopen");
}


int check_ubuntu_cve_link(unsigned int cve_count_passed)
{
	char buffer[500]={0}, buf_fscanf_CVE[20]={0}, buf_fscanf_BugID[10]={0};
	unsigned int  i=0, count_yes_fix=0, count_update_bug=0;
	int sys_ret_val;
	FILE *fp_cve_bugz=NULL, *fp_link=NULL, *fp_old_prio=NULL, *fw_write_final=NULL, *fp_read_final=NULL;;
	char buf_git_fix_link[100]={0}, buf_old_prio[10]={0};

	printf("\n");
	printf(" ----------------------------------------------------------- \n");
	printf(" Do you want to check the ubuntu website for available fix ? \n\n");

	printf(" Make sure you have updated userid & pass for bugz in files: \n");
	printf(" ---------  '.mvista_userid.txt' & '.bugzpass.txt'  -------- \n\n");
	printf(" ------ Press Enter to continue / Ctrl^C to Terminate ------ \n\n");
	getchar();


	fp_cve_bugz=fopen("CVE_nums_extracted.txt", "r");			// Read Extracted CVE numbers file
	if(fp_cve_bugz==NULL) {
		perror("fopen");
		exit(1);
	}
	fw_write_final=fopen("CVE_link_bug_prio.txt", "w");

	while( fscanf(fp_cve_bugz, "%s", buf_fscanf_BugID) != EOF) {

		i++;
		printf(" Bug ID: %s \n", buf_fscanf_BugID);

		if( fscanf(fp_cve_bugz, "%s", buf_fscanf_CVE) != EOF) {

			strcpy(buffer, "python3 ubuntu-fetch.py ");	// "python3 ubuntu-fetch.py "
			strcat(buffer, buf_fscanf_CVE);			// "python3 ubuntu-fetch.py CVE-abcd-1234"

			printf("$ %38s   (loop: %d/%d & till now fix found for: %d)\n", buffer, i, cve_count_passed, count_yes_fix);

			system("rm gitlink_bugzprio.tmp 2> /dev/null");

			sleep(1);

			sys_ret_val = system(buffer);			// run "python3 ubuntu-fetch.py CVE-abcd-1234"
			if(sys_ret_val == 256)					// No fix OR CHARACTER LOADING ERROR
			{
				sleep(1);

				sys_ret_val = system(buffer);			// SECOND CHANCE
				if(sys_ret_val == 256)
				{
					sleep(2);				// THIRD CHANCE

					sys_ret_val = system(buffer);		// if no fix(256), will exit out
				}
			}

			if(sys_ret_val == 0) {				// OK RESPONSE from 'ubuntu-fetch.py '

				fp_link=fopen("gitlink_bugzprio.tmp", "r");
				if(!fp_link) {				// Unable to open the file
					perror("fopen");
					fclose(fp_link);
					break;
				}

				if( fscanf(fp_link, "%s", buf_git_fix_link) == EOF) {
					printf("\n\t Unable to read git fix link for the CVE from file 'git_fix_link.tmp' \n\n");
					system("rm gitlink_bugzprio.tmp 2> /dev/null");
					break;
				}
				else if(strlen(buf_git_fix_link) < 35) { 	// No Fix, only 'Fixed By' - ubuntu.com/security/CVE-2024-0564
					printf("\t\t\t\t\t Invalid Fix Link\n\n");
					fclose(fp_link);
					system("rm gitlink_bugzprio.tmp 2> /dev/null");
					continue;
				}

				fclose(fp_link);
				system("rm gitlink_bugzprio.tmp 2> /dev/null");

				count_yes_fix++;

				strcpy(buffer, "python3 get_prio_back.py ");	// "python3 get_prio_back.py "
				strcat(buffer, buf_fscanf_BugID);		// "python3 get_prio_back.py 142966"

				sys_ret_val = system(buffer);
				if(sys_ret_val == 0) {				// OK RESPONSE from 'get_prio_back.py'

					fp_old_prio=fopen("gitlink_bugzprio.tmp", "r");
					if(!fp_old_prio) {				// Unable to open the file
						perror("fopen");
						fclose(fp_old_prio);
						break;
					}

					if( fscanf(fp_old_prio, "%s", buf_old_prio) == EOF) {
						printf("\n\t Unable to read old priority for the bug from file 'bugz_old_prio.tmp' \n\n");
						system("rm gitlink_bugzprio.tmp 2> /dev/null");
						break;
					}
					fclose(fp_old_prio);
					system("rm gitlink_bugzprio.tmp 2> /dev/null");

				      fprintf(fw_write_final,"%6s %15s %70s %s\n",buf_fscanf_BugID,buf_fscanf_CVE,buf_git_fix_link,buf_old_prio);
					count_update_bug++;
				}
				else {
					printf("\n\t Unhandled Exception #1. system() function returned '%d'. Exiting... \n\n", sys_ret_val);
					break;
				}
			}
//			else if(sys_ret_val == 256);			// If no fix found

			else if(sys_ret_val == 2) {			// check if ^C returns 2 or not
				printf("\n\t Unhandled Exception #2. system() function returned '%d'. Exiting... \n\n", sys_ret_val);
				break;
			}
		}
		printf("\n");
	}
	fclose(fp_cve_bugz);
	fclose(fw_write_final);

	printf(" Found fix for %d CVEs out of %d CVEs \n\n", count_yes_fix, cve_count_passed);

	if(count_update_bug == 0)
		return -1;

	printf("\n");
	printf(" ------------- Generated Following Details ----------------- \n\n");
	system("cat CVE_link_bug_prio.txt");

	printf("\n");
	printf(" ----------------------------------------------------------- \n\n");
	printf(" Do you want to update bugz with above details ? \n\n");
	printf(" ------ Press Enter to continue / Ctrl^C to Terminate ------ \n\n");
	getchar();

	fp_read_final=fopen("CVE_link_bug_prio.txt", "r");
	if(fp_read_final == NULL) {
		perror("fopen");
		exit(1);
	}
	i=0;
	while(  fscanf(fp_read_final, "%s", buf_fscanf_BugID) != EOF)
	{
		i++;

		fscanf(fp_read_final, "%s", buf_fscanf_CVE);
		fscanf(fp_read_final, "%s", buf_git_fix_link);
		fscanf(fp_read_final, "%s", buf_old_prio);

		strcpy(buffer, "python3 backlog_prio_update.py ");	// "python3 backlog_prio_update.py "
		strcat(buffer, buf_fscanf_BugID);			// "python3 backlog_prio_update.py 144229"
		strcat(buffer, " ");					// "python3 backlog_prio_update.py 144229 "
		strcat(buffer, buf_fscanf_CVE);				// "python3 backlog_prio_update.py 144229 CVE-2024-26934"
		strcat(buffer, " ");					// "python3 backlog_prio_update.py 144229 CVE-2024-26934 "
		strcat(buffer, buf_git_fix_link);			// "python3 backlog_prio_update.py 144229 CVE-2024-26934 git_link"
		strcat(buffer, " ");					// "python3 backlog_prio_update.py 144229 CVE-2024-26934 git_link "
		strcat(buffer, buf_old_prio);				// "python3 backlog_prio_update.py 144229 CVE-2024-26934 git_link P2"

		//printf("\n%s\n", buffer);
		sys_ret_val = system(buffer);
		if(sys_ret_val == 0) {
		  printf("%d)\t Successfully updated bug: %s - http://bugz.mvista.com/show_bug.cgi?id=%s\n",i,buf_fscanf_BugID,buf_fscanf_BugID);
		}
		else
		  printf("%d)\t\t   Failed to update bug: %s - http://bugz.mvista.com/show_bug.cgi?id=%s\n",i,buf_fscanf_BugID,buf_fscanf_BugID);
	}

}
