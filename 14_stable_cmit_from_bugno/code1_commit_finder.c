#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


// $ gcc code1_commit_finder.c -o run1_stable_finder.out
// $ Usage: ./run1_stable_finder.out <bugz_num>


//#define debug_print 1

void main(int argc, char *argv[])
{
	char bugz_num[10]={0}, cve_id[15]={0}, master_cmit_id[45]={0}, stable_cmit_hash[45]={0}, buffer[500]={0}, buffer_fp_read[200]={0};
	char *ptr_pos=NULL, char_tmp_read=EOF;
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
	system("cat generated_details.txt");
	printf("\n");

	fp_read_patch_dets = fopen("generated_details.txt", "r");
	if(fp_read_patch_dets == NULL) {
		perror("fopen");
		printf("\t ERROR: Unable to open 'generated_details.txt'. Exiting... \n\n");
		exit(1);
	}

	if( fscanf(fp_read_patch_dets, "%s", buffer_fp_read ) == EOF) {
		printf("\t ERROR: File 'generated_details.txt' looks EMPTY. Exiting... \n\n");
		exit(1);
	}
	fclose(fp_read_patch_dets);

	if( strstr(buffer_fp_read, "CVE") == 0) {	// 'CVE' word not found in the 'buffer_fp_read'
		printf("\t ERROR: 'CVE' id NOT found in 'buffer_fp_read'. Exiting... \n\n");
		exit(1);
	}

	strcpy(cve_id, buffer_fp_read);

#ifdef debug_print
	printf("Fetched CVE id as : %s \n\n", cve_id);
#endif

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
/*
	if( fscanf(fp_read_patch_dets, "%s", buffer_fp_read ) == EOF) {
		printf("\t ERROR: File 'generated_details.txt' looks EMPTY. Exiting... \n\n");
		exit(1);
	}
	fclose(fp_read_patch_dets);

	fp_read_patch_dets = fopen("generated_details.txt", "r");	*/

	while(fscanf(fp_read_patch_dets, "%s", buffer_fp_read ) != EOF)	// contains Master commit/s from Ubuntu 'https://git.kernel.org/linus/'
	{
		ptr_pos=strstr(buffer_fp_read, "linus");
		if( ptr_pos == 0) {					// 'linus' word not found in the 'buffer_fp_read'
			printf("\t ERROR: 'commit' id NOT found in 'buffer_fp_read'. Exiting... \n\n");
			exit(1);
		}

		if(*(ptr_pos+6)=='-' || *(ptr_pos+7)=='\0') {		// No Fix, only 'Fixed By' - ubuntu.com/security/CVE-2024-0564
			printf("No fix found from Ubuntu Security Page. \n\n");
			exit(1);
		}

		strcpy(master_cmit_id, ptr_pos+6);

#ifdef debug_print
		printf("Fetched master commit id as : %s \n\n", master_cmit_id);
#endif

		strcpy(buffer, "python3 nogui_find_stable_commit.py ");	// writes into 'dumped_data.txt' if successful
		strcat(buffer, master_cmit_id);

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

					fseek(fp, -40, SEEK_END);
					fscanf(fp,"%s", stable_cmit_hash);
#ifdef debug_print
					printf("_%s=>%ld_\n\n", stable_cmit_hash,strlen(stable_cmit_hash));
#endif
				}
			}

			fseek(fp, 0, SEEK_END);


			if(strlen(stable_cmit_hash) == 40) {
								// 4.19-> 2 extra scpaces / 5.4-> 3 spaces / 5.10-> 2 spaces / adjust accordingly
				fprintf(fp, "   ");		// 1extra space, as '.py' do not add any after cmit link, 1 more-> visual in file
				fprintf(fp, " https://github.com/gregkh/linux/commit/%s ", stable_cmit_hash);
				fprintf(fp, "\t\t\t https://ubuntu.com/security/%s", cve_id);
				fprintf(fp, " https://github.com/torvalds/linux/commit/%s", master_cmit_id);
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
