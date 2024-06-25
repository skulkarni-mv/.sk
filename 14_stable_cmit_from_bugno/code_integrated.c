#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


// $ gcc code_integrated.c -o run_stable_finder.out
// $ Usage: ./run_stable_finder.out <bugz_num>

void main(int argc, char *argv[])
{
	char bugz_num[10]={0}, cve_id[15]={0}, cmit_id[45]={0}, buffer[500]={0}, buffer_fp_read[200]={0};
	char *ptr_pos=NULL;

	FILE *fp_read_patch_dets = NULL;

	printf("\n--------------------------------------------------------------------------------------\n");
	printf("\n\t Usage: ./run_stable_finder.out <bugz_num> \n\n");

	if(argc != 2)
	{
		exit(1);
	}

	strcpy(bugz_num, argv[1]);

	system("rm generated_details 2> /dev/null");

	strcpy(buffer, "python3 fetch_cve_summary.py ");
	strcat(buffer, bugz_num);
	strcat(buffer, " 1> generated_details.txt");
	system(buffer);

	printf(" bugz_num: %s , Fetched following CVE summary: \n", bugz_num);
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

//	printf("Fetched CVE id as : %s \n\n", cve_id);

	system("rm generated_details 2> /dev/null");
	strcpy(buffer, "python3 ubuntu_fetch_redirectOP.py ");		// > stored output in generated_details.txt
	strcat(buffer, cve_id);

	if( system(buffer) != 0) {
		sleep(1);
		if( system(buffer) != 0) {
			sleep(2);
			if( system(buffer) != 0) {		// Check 3 times, sometimes not able to fetch
				exit(1);
			}
		}
	}

	printf("\n\n");
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

	ptr_pos=strstr(buffer_fp_read, "linus");
	if( ptr_pos == 0) {	// 'CVE' word not found in the 'buffer_fp_read'
		printf("\t ERROR: 'commit' id NOT found in 'buffer_fp_read'. Exiting... \n\n");
		exit(1);
	}

	if(*(ptr_pos+6)=='-' || *(ptr_pos+7)=='\0') {		// No Fix, only 'Fixed By' - ubuntu.com/security/CVE-2024-0564
		printf("No fix found from Ubuntu Security Page. \n\n");
		exit(1);
	}

	strcpy(cmit_id, ptr_pos+6);
//	printf("Fetched commit id as : %s \n\n", cmit_id);

	strcpy(buffer, "python3 nogui_find_stable_commit.py ");
	strcat(buffer, cmit_id);
	system(buffer);

	printf("\n");

}
