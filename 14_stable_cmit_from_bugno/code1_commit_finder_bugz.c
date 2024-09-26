#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>


// $ gcc code1_commit_finder_bugz.c -o run1_stable_finder_bugz.out
// $ Usage: ./run1_stable_finder_bugz.out <bugz_num>


#define checkForOtherUsersToo 1		// Leave uncommented to check for all users

//#define ManualProduct 1		// To select Product manually in 'nogui_find_stable_commit.py' OR ELSE Auto fetched from bugz

//#define debug_print 1

//#define MailDumpedData 1



int main_code_logic(int loop_number, int total_count, char *bugz_input, char *prev_bugz_num, char *prev_master_cmit_id);
void check_if_Ctrl_C_exception(int sys_ret_val_input, char *exception_num, int loop_number, int total_count);

time_t convertToDate(char *date_str);


bool override_MailDumpedData=false;


void main(int argc, char *argv[])
{
	char buf_fgets[500]={0}, bugz_num_inloop[10]={0};
	int i=0, Input_digitsOnly=1, totalBugs=0, loopCount=0;
	char prev_bugz_num[10]={0}, prev_master_cmit_id[45]={0};

	FILE *fp_bugzExtract=NULL;


	if(argc == 2);

	else if(argc == 3 && !strcmp(argv[2], "--maildata") ) {

		override_MailDumpedData = true;
	}
	else
	{
		printf("\n\t Usage: ./run1_stable_finder.out <bugz_num> OR <csv_file_name> {OPT: --maildata}\n\n");
		exit(1);
	}

	for(i=0; i< strlen(argv[1]); i++) {
		if ( !isdigit(argv[1][i]) ) {
			Input_digitsOnly = 0;
			break;
		}
	}

	printf("\n");
	system("rm dumped_data.txt 2> /dev/null");

#ifdef ManualProduct

	printf("\n 'ManualProduct' flag is set. DO NOT RUN 'run2_bugz_updater_batch.out' it will update wrong SYNC_REQ / RESOLVED on bugz. \n");
	printf("\n --------------------------- Press Enter to continue / Ctrl^C to Terminate ----------------------------- \n");
	getchar();

#endif

#ifdef MailDumpedData
	FILE *fp_dump_for_mail=fopen("dumped_data.txt", "w");
	fprintf(fp_dump_for_mail, "Subject: Script executed, 'dumped_data.txt' data as below \n\n");
	fclose(fp_dump_for_mail);
#else
	if(override_MailDumpedData == true) {

	FILE *fp_dump_for_mail=fopen("dumped_data.txt", "w");
	fprintf(fp_dump_for_mail, "Subject: Script executed, 'dumped_data.txt' data as below \n\n");
	fclose(fp_dump_for_mail);
	}
#endif

	if( Input_digitsOnly == 1 && ( strlen(argv[1])==5 || strlen(argv[1])==6 ) ) {	// Single Bug Passed
		printf(" Bugz Number passed, len 5 or 6 \n");

//		system("rm dumped_data.txt 2> /dev/null");
		strncpy(bugz_num_inloop, argv[1], 9);

		main_code_logic(1, 1, bugz_num_inloop, prev_bugz_num, prev_master_cmit_id);

		system("rm generated_details.txt 2> /dev/null");                // Delete unnecessary file
	}

	else if( strstr(argv[1], ".csv") != NULL ) {
		printf(" CSV File passed, Extracting Bugz Numbers... \n\n");

		fp_bugzExtract=fopen(argv[1], "r");
		if(fp_bugzExtract == NULL) {
			perror("fopen");
			exit(1);
		}

		if( fgets(buf_fgets, 490, fp_bugzExtract) != NULL) {
		        printf(" Ommiting first line -> %s  \n", buf_fgets); // went to the new-line
		}
		else {
			printf(" File is Empty. Exiting... \n\n");
		}

//		system("rm dumped_data.txt 2> /dev/null");

		while( fgets(buf_fgets, 490, fp_bugzExtract) != NULL ) totalBugs++;
		printf("\t Total bugs = %d \n", totalBugs);

		rewind(fp_bugzExtract);						// Go To Start of the file
		fgets(buf_fgets, 490, fp_bugzExtract);				// Ommit First Line again

		while( fgets(buf_fgets, 490, fp_bugzExtract) != NULL ) {

			loopCount++;
//			printf("%d - ", loopCount);

			for(i=0; i<strlen(buf_fgets); i++) {	//buf_fgets=> '131976,"normal","CGX.'

				if( !isdigit(buf_fgets[i]) ) {			// if not a digit
					bugz_num_inloop[i] = '\0';
					break;					// break out of for() NOT while()
				}
				else
					bugz_num_inloop[i] = buf_fgets[i];
			}

			if( strlen(bugz_num_inloop)==5 || strlen(bugz_num_inloop)==6 ) {
//				printf(" %s \n", bugz_num_inloop);

				main_code_logic(loopCount, totalBugs, bugz_num_inloop, prev_bugz_num, prev_master_cmit_id);
			}
			else {
				printf(" Data contains invalid Bugz Number '%s'. Exiting... \n\n", bugz_num_inloop);
				exit(1);
			}
		}

		system("rm generated_details.txt 2> /dev/null");                // Delete unnecessary file
	}
	else {
		printf(" Pass either .csv file or bugz number with len 5 or 6 \n");
		exit(1);
	}

	printf("\n");


#ifdef MailDumpedData							// Send Mail once done
	printf("\n********\n  Sending E-Mail...\n");
	system("sleep 2 && git send-email --to=skulkarni@mvista.com --confirm=never dumped_data.txt >> dumped_data.txt");
#else
	if(override_MailDumpedData == true) {

	printf("\n********\n  Sending E-Mail...\n");
	system("sleep 2 && git send-email --to=skulkarni@mvista.com --confirm=never dumped_data.txt >> dumped_data.txt");
	}
#endif

	exit(0);
}

int main_code_logic(int loop_number, int total_count, char *bugz_input, char *prev_bugz_num, char *prev_master_cmit_id)
{
	char bugz_num[10]={0}, cve_id[15]={0}, master_cmit_id[45]={0}, stable_cmit_hash[45]={0}, buffer[500]={0}, buffer_fp_read[200]={0};
	char *ptr_pos=NULL, char_tmp_read=EOF, stable_branch_vers[60]={0};
	char prod_of_bugz[10]={0}, prio_of_bugz[10]={0}, status_of_bugz[15]={0}, assignee_of_bugz[30]={0}, date_reported_of_bugz[40]={0};
	int i=0, sys_ret_val;

	FILE *fp_read_patch_dets = NULL;


	strcpy(bugz_num, bugz_input);

	system("rm generated_details 2> /dev/null");
//	system("rm dumped_data.txt 2> /dev/null");		// Do not delete in this code. Delete from cmd line while starting, if looping

	printf("\n");
	printf("_ %d/%d __", loop_number, total_count);
	for(i=1; i<=23; i++)
		printf("______");
	printf("\n\n");

	printf(" bugz_num: %s ...", bugz_num); fflush(stdout);

	strcpy(buffer, "python3 fetch_cve_summary.py ");
	strcat(buffer, bugz_num);
	strcat(buffer, " 1> generated_details.txt");
	sys_ret_val = system(buffer);

	check_if_Ctrl_C_exception(sys_ret_val, "F1", loop_number, total_count);

	printf(" Fetched following CVE summary: \n");
	printf("\t\t\t\t");	fflush(stdout);
	system("head -1 generated_details.txt");
//	printf("\n");

	fp_read_patch_dets = fopen("generated_details.txt", "r");
	if(fp_read_patch_dets == NULL) {
		perror("fopen");
		printf("\t ERROR: Unable to open 'generated_details.txt'. Exiting... \n\n");
		return(-1);
	}

//	CVE Summary
	if( fscanf(fp_read_patch_dets, "%s", buffer_fp_read ) == EOF) {
		printf("\t ERROR: File 'generated_details.txt' looks EMPTY. Exiting... \n\n");
		return(-1);
	}

	if( strstr(buffer_fp_read, "CVE") == 0) {		// 'CVE' word not found in the 'buffer_fp_read'
		printf("\t ERROR: 'CVE' id NOT found in 'buffer_fp_read'. Exiting... \n\n");
		return(-1);
	}

	if(strchr(buffer_fp_read, ',') != NULL)	{		// if ',' presents e.g. 131394 (CVE-2023-6356, CVE-2023-6535, CVE-2023-6536)
		*strchr(buffer_fp_read, ',') = '\0';		// replace with '\0'
	}

	strcpy(cve_id, buffer_fp_read);
	fgets(buffer, 499, fp_read_patch_dets);				// first line is read completly, i.e. CVE summary is done


//	Product Details
        if( fgets(buffer_fp_read, 9, fp_read_patch_dets) == NULL) {		// As prod_of_bugz[] size is 10, so read 9 chars only MAX
                printf("\t ERROR: Product info in 'generated_details.txt' Not Found. Exiting... \n\n");
                return(-1);
        }

	if( strlen(buffer_fp_read) > 2 ) {					// for Local Laptop, last char coming out to be '\n', so replaced
		if( buffer_fp_read[strlen(buffer_fp_read)-1] == '\n') {
			buffer_fp_read[strlen(buffer_fp_read)-1] = '\0';
		}
	}

	strcpy(prod_of_bugz, buffer_fp_read);

	if( strchr(prod_of_bugz, ' ') != NULL )				// if space found eg- 'CGX 4.0', will not be there in 'CentOS'
		strcpy( strchr(prod_of_bugz, ' '), strchr(prod_of_bugz, ' ')+1 );	// Remove Space Inbetween

	if( prod_of_bugz[strlen(prod_of_bugz)-1] == '\n' )
		prod_of_bugz[strlen(prod_of_bugz)-1] = '\0';

	printf("\t\t\t\t\t ");     fflush(stdout);

#ifdef ManualProduct
	strcpy(prod_of_bugz, "-");
#endif

	printf("Product: ");			fflush(stdout);

//	Priority
        if( fgets(buffer_fp_read, 9, fp_read_patch_dets) == NULL) {		// As prio_of_bugz[] size is 10, so read 9 chars only MAX
                printf("\t ERROR: Priority info in 'generated_details.txt' Not Found. Exiting... \n\n");
                return(-1);
        }
	strcpy(prio_of_bugz, buffer_fp_read);
	if( prio_of_bugz[strlen(prio_of_bugz)-1] == '\n' )
		prio_of_bugz[strlen(prio_of_bugz)-1] = '\0';

//	printf("\t%s", prio_of_bugz);     fflush(stdout);

//	Status
        if( fgets(buffer_fp_read, 14, fp_read_patch_dets) == NULL) {		// As status_of_bugz[] size is 15, so read 14 chars only MAX
                printf("\t ERROR: Status info in 'generated_details.txt' Not Found. Exiting... \n\n");
                return(-1);
        }
	strcpy(status_of_bugz, buffer_fp_read);
	if( status_of_bugz[strlen(status_of_bugz)-1] == '\n' )
		status_of_bugz[strlen(status_of_bugz)-1] = '\0';

//	printf("\t%s", status_of_bugz);     fflush(stdout);

//	Assignee
        if( fgets(buffer_fp_read, 29, fp_read_patch_dets) == NULL) {		// As assignee_of_bugz[] size is 30, so read 29 chars only MAX
                printf("\t ERROR: Assignee info in 'generated_details.txt' Not Found. Exiting... \n\n");
                return(-1);
        }
	strcpy(assignee_of_bugz, buffer_fp_read);
	if( assignee_of_bugz[strlen(assignee_of_bugz)-1] == '\n' )
		assignee_of_bugz[strlen(assignee_of_bugz)-1] = '\0';

//	printf(" %s", assignee_of_bugz);     fflush(stdout);


//	Reported Date
        if( fgets(buffer_fp_read, 39, fp_read_patch_dets) == NULL) {	    // As date_reported_of_bugz[] size is 40, so read 39 chars only MAX
                printf("\t ERROR: Assignee info in 'generated_details.txt' Not Found. Exiting... \n\n");
                return(-1);
        }
	strcpy(date_reported_of_bugz, buffer_fp_read);

	if( date_reported_of_bugz[strlen(date_reported_of_bugz)-1] == '\n' )
		date_reported_of_bugz[strlen(date_reported_of_bugz)-1] = '\0';
	if( date_reported_of_bugz[strlen(date_reported_of_bugz)-1] == ' ' )
		date_reported_of_bugz[strlen(date_reported_of_bugz)-1] = '\0';

	for(i=0; i<strlen(date_reported_of_bugz); i++)
		if(date_reported_of_bugz[i] == ' ') {
			date_reported_of_bugz[i]='\0';
			break;
		}

//	printf(" %s", date_reported_of_bugz);     fflush(stdout);

	printf("%7s %8s %9s %11s %11s", prod_of_bugz, prio_of_bugz, status_of_bugz, assignee_of_bugz, date_reported_of_bugz);	fflush(stdout);

//---------------

	fclose(fp_read_patch_dets);
	printf("\n\n");

#ifdef debug_print
	printf("Fetched CVE id as : %s \n\n", cve_id);
#endif

	strcpy(buffer, "python3 check_rejected_cve_json.py ");		// Check if CVE is Invalid
	strcat(buffer, cve_id);
	sys_ret_val = system(buffer);

	check_if_Ctrl_C_exception(sys_ret_val, "R1", loop_number, total_count);

	if( sys_ret_val != 0) {
		return(-1);
	}

	system("rm generated_details 2> /dev/null");

	strcpy(buffer, "python3 ubuntu_fetch_redirectOP.py ");	// writes into 'generated_details.txt' if successful
	strcat(buffer, cve_id);
	sys_ret_val = system(buffer);

	check_if_Ctrl_C_exception(sys_ret_val, "U1", loop_number, total_count);
	if( sys_ret_val != 0) {

		sleep(3);
		sys_ret_val = system(buffer);

		check_if_Ctrl_C_exception(sys_ret_val, "U2", loop_number, total_count);
		if( sys_ret_val != 0) {

			sleep(5);
			sys_ret_val = system(buffer);

			check_if_Ctrl_C_exception(sys_ret_val, "U3", loop_number, total_count);
			if( sys_ret_val != 0) {		// Check 3 times, sometimes not able to fetch
				return(-1);			// Still no master commit found on ubuntu page
			}
		}
	}

	printf("\n");

	fp_read_patch_dets = fopen("generated_details.txt", "r");
	if(fp_read_patch_dets == NULL) {
		perror("fopen");
		printf("\t ERROR: Unable to open 'generated_details.txt'. Exiting... \n\n");
		return(-1);
	}


	while(fscanf(fp_read_patch_dets, "%s", buffer_fp_read ) != EOF)    // contains Master commit/s from Ubuntu 'https://git.kernel.org/linus/'
	{
		ptr_pos=strstr(buffer_fp_read, "linus");
		if( ptr_pos == 0) {					// 'linus' word not found in the 'buffer_fp_read'
			printf("\t ERROR: 'master commit' id NOT found in 'buffer_fp_read'. Exiting... \n\n");
			return(-1);
		}

		if(*(ptr_pos+6)=='-' || *(ptr_pos+7)=='\0') {		// No Fix, only 'Fixed By' - Eg: /ubuntu.com/security/CVE-2024-0564
			printf("\n No Valid fix found from Ubuntu Security Page. \n\n");
			return(-1);
		}

		strncpy(master_cmit_id, ptr_pos+6, 44);

#ifdef debug_print
		printf("\n-----------------------\n");
		printf("Fetched master commit id as : %s \n\n", master_cmit_id);
#endif

		if(strlen(master_cmit_id) != 40)			// Not in Appropriate Format - Eg: /ubuntu.com/security/CVE-2023-52604
		{
			printf("\t\t\t\txxxxxxxxxxx Commit ID found on Ubuntu Security Page is NOT Appropriate. CHECK MANUALLY. xxxxxxxxxx\n\n");
			return(-1);
		}


		if( (strcmp(bugz_num, prev_bugz_num)==0) && (strcmp(master_cmit_id, prev_master_cmit_id)==0) )	// To avoid repeatations
		{
	printf("\n Found same data as of prev: bugz _%s_&_%s_ commit _%s_&_%s_ \n", bugz_num,prev_bugz_num, master_cmit_id,prev_master_cmit_id);
			strcpy(prev_bugz_num, bugz_num);
			strcpy(prev_master_cmit_id, master_cmit_id);
			continue;
		}

//		Copy Present Value to the Previous Data
		strcpy(prev_bugz_num, bugz_num);
		strcpy(prev_master_cmit_id, master_cmit_id);


		FILE *fp=fopen("dumped_data.txt", "a+");
		fprintf(fp, "Bug:%s ", bugz_num);
		fclose(fp);

		strcpy(buffer, "python3 nogui_find_stable_commit.py ");	// writes into 'dumped_data.txt' if successful
		strcat(buffer, master_cmit_id);
		strcat(buffer, " ");
		strcat(buffer, prod_of_bugz);
		sys_ret_val = system(buffer);

		if(sys_ret_val == 0) {			//- ----------- python3 nogui_find_stable_commit.py <master_cmit> ---------------

			fp=fopen("dumped_data.txt", "r+");
			fseek(fp, -1, SEEK_END);

			char_tmp_read=fgetc(fp);
#ifdef debug_print
			printf("_%c_ is the last char of response/commit from .py file\n", char_tmp_read);
#endif

//			for(i=0; i<45; i++)				// max size allocated is 'char stable_cmit_hash[45]={0}', so i<45
//				stable_cmit_hash[i]='\0';

			memset(stable_cmit_hash, '\0', 45);

			if(char_tmp_read != EOF) {

				if(char_tmp_read!='-' && char_tmp_read!='x') {	// '-' means no fix found. 'x' means wrong product
										// '-'/'x' written into file by nogui_find_stable_commit.py

					fseek(fp, -40, SEEK_END);					// as commit length is 40 and its at
					fscanf(fp,"%s", stable_cmit_hash);				// at the end of line. Hence SEEK_END-40

					fseek(fp, -60, SEEK_END);					// thus 60 is size for stable_branch_vers
					fscanf(fp, "%s", stable_branch_vers);				// -> _/?h=linux-5.10.y&id=3..._

					if(strlen(stable_branch_vers) != 0) {
						if( strstr(stable_branch_vers, "linux-") != 0) {	// means found the string
							strcpy(stable_branch_vers, strstr(stable_branch_vers, "linux-") );  // -> _linux-5.10.y&_
							strcpy(stable_branch_vers, stable_branch_vers + strlen("linux-") ); // -> _5.10.y&id=3._

							for(i=0; i<10; i++) {
								if(stable_branch_vers[i] == '&')
									break;
							}

							for(; i<59; i++)			// 59, as 60 is allotted size in declaration
								stable_branch_vers[i]='\0';
						}
					}

#ifdef debug_print
					printf("_%s=>%ld_\n"  , stable_cmit_hash, strlen(stable_cmit_hash) );
					printf("_%s=>%ld_\n\n", stable_branch_vers, strlen(stable_branch_vers) );
#endif
/*				  fseek(fp, 0, SEEK_END);

*/				} // END of "if(char_tmp_read!='-' && char_tmp_read!='x') {"
/*
				else {						// if no stable fix, replace data with ' ' spaces
					fseek(fp, -13, SEEK_END);		// _Bug:123456  -_
					fprintf(fp, "            ");
				}
*/			} // END of "if(char_tmp_read != EOF) {"

/*uncommented*/		fseek(fp, 0, SEEK_END);

			if(strlen(stable_cmit_hash) == 40)
			{
								// 4.19-> 2 extra scpaces / 5.4-> 3 spaces / 5.10-> 2 spaces / adjust accordingly
				if(strlen(stable_branch_vers) %2)		// ODD Length [Eg. 5.4.y => 5] -> 1extra space for Visuals
					fprintf(fp, "   ");			// as '.py' file do not add any after cmit link
				else
					fprintf(fp, "  ");			// 2 spaces for EVEN Length [Eg. 5.10.y => 6]

				fprintf(fp, " https://github.com/gregkh/linux/commit/%s ", stable_cmit_hash);
				fprintf(fp, "\t\t\t https://ubuntu.com/security/%s", cve_id);
				if(strlen(cve_id) == 13)
					fprintf(fp, " ");			// if CVE-1234-abcd instead of CVE-1234-ABCDE
				fprintf(fp, " https://github.com/torvalds/linux/commit/%s", master_cmit_id);

				fprintf(fp, " - ");
		  fprintf(fp, "%6s %7s %8s %10s %10s", prod_of_bugz, prio_of_bugz, status_of_bugz, assignee_of_bugz, date_reported_of_bugz);

#ifdef checkForOtherUsersToo
				if(1 == 1)					// Bypass Username Check
#else
				if( strcmp(assignee_of_bugz, "secbugz")==0 || strcmp(assignee_of_bugz, "skulkarni")==0 )
#endif
				{
					i=1;

					if     ( strcmp(status_of_bugz, "NEW")     ==0) i=0;		// chk 'code2*.c', filter added there too
					else if( strcmp(status_of_bugz, "ASSIGNED")==0) i=0;
					else if( strcmp(status_of_bugz, "REOPENED")==0) i=0;
					else if( strcmp(status_of_bugz, "SYNC_REQ")==0) i=0;
					else
						fprintf(fp, "%17s", " N 'status'");	// Should not update bug as bugz 'Status' not appropriate

					if(i==0)		// Stored for Next Check
					{

						setenv("TZ", "PST8PDT", 1);		// -> To get diff between BugOpenDate & PresentDate(PST)
						tzset();
						time_t t = time(NULL);
						struct tm *local_time = localtime(&t);
						char date_string[12];
						strftime(date_string, sizeof(date_string), "%Y-%m-%d", local_time);
						//	printf("Current date in PST: %s\n", date_string);

						time_t date1 = convertToDate(date_reported_of_bugz);
						time_t date2 = convertToDate(date_string);

						double difference = difftime(date2, date1);

						// Convert seconds to days (1 day = 24 hours * 60 minutes * 60 seconds)
						int diff_days = difference / (60 * 60 * 24);
						//	printf(" diff_days is: %d", diff_days);

						if(diff_days < 1) {
							fprintf(fp, "%17s", " N 'date'");	// Should not update as bug opened on same day
							i=1;
						}
						else if(diff_days >= 1)
							fprintf(fp, " Y dOK  %4d", diff_days);
						else
							fprintf(fp, " Y dNOK %4d", diff_days);

						if(i==0) {
							if( strcmp(assignee_of_bugz, "secbugz")==0 || strcmp(assignee_of_bugz, "skulkarni")==0 )
								fprintf(fp, " namY");
							else
								fprintf(fp, " namN");
						}

					} // if(i==0) ENDS

				} // USERNAME_CHECK OR BYPASS
				else {
					fprintf(fp, "%17s", " N 'user'");		// Should not update bug as another 'User' has the bug
				}

//				fprintf(fp, "\n");	// Will be handled in 'mvista_find_gitcgx_commit.py'
				fclose(fp);

		                strcpy(buffer, "python3 mvista_find_gitcgx_commit.py "); // writes into 'dumped_data.txt' if successful
				strcat(buffer, master_cmit_id);
		                strcat(buffer, " ");
		                strcat(buffer, prod_of_bugz);
		                sys_ret_val = system(buffer);

				check_if_Ctrl_C_exception(sys_ret_val, "M1", loop_number, total_count);

			} // if(strlen(stable_cmit_hash) == 40) ENDS
			else {
				fprintf(fp, "\n");
				fclose(fp);
			}

		} // if(sys_ret_val == 0) ENDS

		check_if_Ctrl_C_exception(sys_ret_val, "S1", loop_number, total_count);

	} // End of while() loop

	FILE *fp=fopen("dumped_data.txt", "a+");
	fprintf(fp, "\n");
	fclose(fp);

	fclose(fp_read_patch_dets);
	printf(".....\n");

}

void check_if_Ctrl_C_exception(int sys_ret_val_input, char *exception_num, int loop_number, int total_count)
{
	if(sys_ret_val_input == 2) {
		printf("\n\t Unhandled Exception #%s in loop %d/%d. system() function returned '%d'. Exiting... \n\n", \
					exception_num, loop_number, total_count, sys_ret_val_input);

		system("rm generated_details.txt 2> /dev/null");		// Delete unnecessary file
		exit(1);
	}
}

// Function to convert date string (yyyy-mm-dd) to time_t
time_t convertToDate(char *date_str) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    sscanf(date_str, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
    tm.tm_year -= 1900;  // Year is years since 1900
    tm.tm_mon--;         // Month is 0-11, so adjust for 0-based index

    return mktime(&tm);
}
