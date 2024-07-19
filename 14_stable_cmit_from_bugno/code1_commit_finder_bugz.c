#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


// $ gcc code1_commit_finder_bugz.c -o run1_stable_finder_bugz.out
// $ Usage: ./run1_stable_finder_bugz.out <bugz_num>


//#define ManualProduct 1		// To select Product manually in 'nogui_find_stable_commit.py' OR ELSE Auto fetched from bugz
//#define debug_print 1


void main(int argc, char *argv[])
{
	char bugz_num[10]={0}, cve_id[15]={0}, master_cmit_id[45]={0}, stable_cmit_hash[45]={0}, buffer[500]={0}, buffer_fp_read[200]={0};
	char *ptr_pos=NULL, char_tmp_read=EOF, stable_branch_vers[60]={0};
	char prod_of_bugz[10]={0}, prio_of_bugz[10]={0}, status_of_bugz[15]={0}, assignee_of_bugz[30]={0}, date_reported_of_bugz[40]={0};
	int i=0;

	FILE *fp_read_patch_dets = NULL;

	printf("\n_________________________??????????????? Have you removed dumped_data.txt file ?????????????????_________");
	printf("_____________________________________________\n\n");

	if(argc != 2)
	{
		printf("\n\t Usage: ./run1_stable_finder.out <bugz_num> \n\n");
		exit(1);
	}

	strcpy(bugz_num, argv[1]);

	system("rm generated_details 2> /dev/null");
//	system("rm dumped_data.txt 2> /dev/null");		// Do not delete in this code. Delete from cmd line while starting, if looping

	printf(" bugz_num: %s ...", bugz_num); fflush(stdout);

	strcpy(buffer, "python3 fetch_cve_summary.py ");
	strcat(buffer, bugz_num);
	strcat(buffer, " 1> generated_details.txt");
	system(buffer);

	printf(" Fetched following CVE summary: \n");
	printf("\t\t\t\t");	fflush(stdout);
	system("head -1 generated_details.txt");
//	printf("\n");

	fp_read_patch_dets = fopen("generated_details.txt", "r");
	if(fp_read_patch_dets == NULL) {
		perror("fopen");
		printf("\t ERROR: Unable to open 'generated_details.txt'. Exiting... \n\n");
		exit(1);
	}

//	CVE Summary
	if( fscanf(fp_read_patch_dets, "%s", buffer_fp_read ) == EOF) {
		printf("\t ERROR: File 'generated_details.txt' looks EMPTY. Exiting... \n\n");
		exit(1);
	}

	if( strstr(buffer_fp_read, "CVE") == 0) {	// 'CVE' word not found in the 'buffer_fp_read'
		printf("\t ERROR: 'CVE' id NOT found in 'buffer_fp_read'. Exiting... \n\n");
		exit(1);
	}
	strcpy(cve_id, buffer_fp_read);
	fgets(buffer, 499, fp_read_patch_dets);				// first line is read completly, i.e. CVE summary is done


//	Product Details
        if( fgets(buffer_fp_read, 9, fp_read_patch_dets) == NULL) {		// As prod_of_bugz[] size is 10, so read 9 chars only MAX
                printf("\t ERROR: Product info in 'generated_details.txt' Not Found. Exiting... \n\n");
                exit(1);
        }
	strcpy(prod_of_bugz, buffer_fp_read);
	strcpy( strchr(prod_of_bugz, ' '), strchr(prod_of_bugz, ' ')+1 );	// Remove Space Inbetween

	if( prod_of_bugz[strlen(prod_of_bugz)-1] == '\n' )
		prod_of_bugz[strlen(prod_of_bugz)-1] = '\0';

	printf("\t\t\t\t\t ");     fflush(stdout);

#ifdef ManualProduct
	strcpy(prod_of_bugz, "-");
#endif
	printf("Product: %s", prod_of_bugz);	fflush(stdout);


//	Priority
        if( fgets(buffer_fp_read, 9, fp_read_patch_dets) == NULL) {		// As prio_of_bugz[] size is 10, so read 9 chars only MAX
                printf("\t ERROR: Priority info in 'generated_details.txt' Not Found. Exiting... \n\n");
                exit(1);
        }
	strcpy(prio_of_bugz, buffer_fp_read);
	if( prio_of_bugz[strlen(prio_of_bugz)-1] == '\n' )
		prio_of_bugz[strlen(prio_of_bugz)-1] = '\0';

	printf("\t%s", prio_of_bugz);     fflush(stdout);

//	Status
        if( fgets(buffer_fp_read, 14, fp_read_patch_dets) == NULL) {		// As status_of_bugz[] size is 15, so read 14 chars only MAX
                printf("\t ERROR: Status info in 'generated_details.txt' Not Found. Exiting... \n\n");
                exit(1);
        }
	strcpy(status_of_bugz, buffer_fp_read);
	if( status_of_bugz[strlen(status_of_bugz)-1] == '\n' )
		status_of_bugz[strlen(status_of_bugz)-1] = '\0';

	printf("\t%s", status_of_bugz);     fflush(stdout);

//	Assignee
        if( fgets(buffer_fp_read, 29, fp_read_patch_dets) == NULL) {		// As assignee_of_bugz[] size is 30, so read 29 chars only MAX
                printf("\t ERROR: Assignee info in 'generated_details.txt' Not Found. Exiting... \n\n");
                exit(1);
        }
	strcpy(assignee_of_bugz, buffer_fp_read);
	if( assignee_of_bugz[strlen(assignee_of_bugz)-1] == '\n' )
		assignee_of_bugz[strlen(assignee_of_bugz)-1] = '\0';

	printf("\t%s", assignee_of_bugz);     fflush(stdout);


//	Reported Date
        if( fgets(buffer_fp_read, 39, fp_read_patch_dets) == NULL) {	    // As date_reported_of_bugz[] size is 40, so read 39 chars only MAX
                printf("\t ERROR: Assignee info in 'generated_details.txt' Not Found. Exiting... \n\n");
                exit(1);
        }
	strcpy(date_reported_of_bugz, buffer_fp_read);

	if( date_reported_of_bugz[strlen(date_reported_of_bugz)-1] == '\n' )
		date_reported_of_bugz[strlen(date_reported_of_bugz)-1] = '\0';
	if( date_reported_of_bugz[strlen(date_reported_of_bugz)-1] == ' ' )
		date_reported_of_bugz[strlen(date_reported_of_bugz)-1] = '\0';

	printf("\t%s", date_reported_of_bugz);     fflush(stdout);

//---------------

	fclose(fp_read_patch_dets);
	printf("\n\n");

#ifdef debug_print
	printf("Fetched CVE id as : %s \n\n", cve_id);
#endif

	strcpy(buffer, "python3 check_rejected_cve_json.py ");		// Check if CVE is Invalid
	strcat(buffer, cve_id);

	if( system(buffer) != 0) {
		exit(1);
	}


	system("rm generated_details 2> /dev/null");
	strcpy(buffer, "python3 ubuntu_fetch_redirectOP.py ");	// writes into 'generated_details.txt' if successful
	strcat(buffer, cve_id);

	if( system(buffer) != 0) {
		sleep(1);
		if( system(buffer) != 0) {
			sleep(2);
			if( system(buffer) != 0) {		// Check 3 times, sometimes not able to fetch
				exit(1);			// Still no master commit fround on ubuntu page
			}
		}
	}

	printf("\n");

	fp_read_patch_dets = fopen("generated_details.txt", "r");
	if(fp_read_patch_dets == NULL) {
		perror("fopen");
		printf("\t ERROR: Unable to open 'generated_details.txt'. Exiting... \n\n");
		exit(1);
	}


	while(fscanf(fp_read_patch_dets, "%s", buffer_fp_read ) != EOF)	// contains Master commit/s from Ubuntu 'https://git.kernel.org/linus/'
	{
		ptr_pos=strstr(buffer_fp_read, "linus");
		if( ptr_pos == 0) {					// 'linus' word not found in the 'buffer_fp_read'
			printf("\t ERROR: 'commit' id NOT found in 'buffer_fp_read'. Exiting... \n\n");
			exit(1);
		}

		if(*(ptr_pos+6)=='-' || *(ptr_pos+7)=='\0') {		// No Fix, only 'Fixed By' - Eg: /ubuntu.com/security/CVE-2024-0564
			printf("No fix found from Ubuntu Security Page. \n\n");
			exit(1);
		}

		strncpy(master_cmit_id, ptr_pos+6, 44);

#ifdef debug_print
		printf("Fetched master commit id as : %s \n\n", master_cmit_id);
#endif

		if(strlen(master_cmit_id) != 40)			// Not in Appropriate Format - Eg: /ubuntu.com/security/CVE-2023-52604
		{
			printf("\t\t\t\txxxxxxxxxxx Commit ID found on Ubuntu Security Page is NOT Appropriate. CHECK MANUALLY. xxxxxxxxxx\n\n");
			exit(1);
		}


		strcpy(buffer, "python3 nogui_find_stable_commit.py ");	// writes into 'dumped_data.txt' if successful
		strcat(buffer, master_cmit_id);
		strcat(buffer, " ");
		strcat(buffer, prod_of_bugz);

		FILE *fp=fopen("dumped_data.txt", "a+");
		fprintf(fp, "Bug:%s ", bugz_num);
		fclose(fp);

		if(system(buffer) == 0) {			//- ----------- python3 nogui_find_stable_commit.py <master_cmit> ---------------

			fp=fopen("dumped_data.txt", "r+");
			fseek(fp, -1, SEEK_END);

			char_tmp_read=fgetc(fp);
#ifdef debug_print
			printf("_%c_ is the last char of response/commit from .py file\n", char_tmp_read);
#endif

			for(i=0; i<45; i++)				// max size allocated is 'char stable_cmit_hash[45]={0}', so i<45
				stable_cmit_hash[i]='\0';

			if(char_tmp_read != EOF) {
				if(char_tmp_read != '-') {	// '-' means no fix found. '-' written into file by nogui_find_stable_commit.py

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
				}
			}

			fseek(fp, 0, SEEK_END);


			if(strlen(stable_cmit_hash) == 40) {
								// 4.19-> 2 extra scpaces / 5.4-> 3 spaces / 5.10-> 2 spaces / adjust accordingly
				if(strlen(stable_branch_vers) %2)		// ODD Length [Eg. 5.4.y => 5] -> 1extra space for Visuals
					fprintf(fp, "   ");			// as '.py' file do not add any after cmit link
				else
					fprintf(fp, "  ");			// 2 spaces for EVEN Length [Eg. 5.10.y => 6]

				fprintf(fp, " https://github.com/gregkh/linux/commit/%s ", stable_cmit_hash);
				fprintf(fp, "\t\t\t https://ubuntu.com/security/%s", cve_id);
				if(strlen(cve_id) == 13)
					fprintf(fp, " ");	// if CVE-1234-abcd instead of CVE-1234-ABCDE
				fprintf(fp, " https://github.com/torvalds/linux/commit/%s", master_cmit_id);

		  fprintf(fp, " - %s %s %s %s %s", prod_of_bugz, prio_of_bugz, status_of_bugz, assignee_of_bugz, date_reported_of_bugz);

			}
			fprintf(fp, "\n");
			fclose(fp);
		}
	} // End of while() loop

	FILE *fp=fopen("dumped_data.txt", "a+");
	fprintf(fp, "\n");
	fclose(fp);

	fclose(fp_read_patch_dets);
	printf(".....\n");

}
