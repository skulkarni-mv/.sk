#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


bool check_input_length(char *input, int correct_length, char *input_name_label);
void print_bug_num_stable_commit_id(char *bugz_num, char *stable_commit_id);


void main(int argc, char *argv[])
{

	char bugz_num[10]={0},  stable_commit_id[50]={0}, i = 0, choice_y_n;
	char buffer[500]={0}, buffer_fp_read[200]={0}, buffer_temp_tag[100]={0};
	char script_ip_revision_r[5]={0}, script_ip_patches_n[5]={0};

	FILE *fp_read_patch_dets = NULL;


	printf("\n\t Usage: ./automate_run.out <bugz_num> <stable_commit_id> \n");

	if (argc == 1)		// Passed only program_binary
	{
		printf("\n");
		printf(" Enter bugz_number : ");
		scanf("%s", bugz_num);

		if( check_input_length(bugz_num, 6, "bugz_num") != true)
			exit(1);

		printf(" Enter upstream commit from STABLE to fix CVE : ");
		scanf("%s", stable_commit_id);

		if(check_input_length(stable_commit_id, 40, "stable_commit_id") != true)
			exit(1);

		print_bug_num_stable_commit_id(bugz_num, stable_commit_id);
	}
	else if(argc == 3)	// Passed program_binary , bug_num , commit_id
	{
		strcpy(bugz_num, argv[1]);
		if( check_input_length(bugz_num, 6, "bugz_num") != true)
		        exit(1);

		strcpy(stable_commit_id, argv[2]);
		if(check_input_length(stable_commit_id, 40, "stable_commit_id") != true)
		        exit(1);

		print_bug_num_stable_commit_id(bugz_num, stable_commit_id);
	}
	else
	{
		printf("\n");
		exit(1);	
	}

	printf(" ------------------------------------------------------------------------------------ \n");
	printf("  Assuming the gregkh/linux is cloned at : \" ../gregkh-linux/4.19-linux/linux/ \"  \n");
	printf("  And branch checked out to the following: \" linux-4.19.y \" \n\n");
	printf(" ------------- Check if the repo details defined as above are satisfied ------------- \n");
	printf(" ------------------ Press Enter to continue / Ctrl^C to Terminate ------------------- \n");

	getchar();	// Uncomment once dev_done - Fix me

	system("rm automate_generated_details.txt");

	strcpy(buffer, "cd ../gregkh-linux/4.19-linux/linux/ && ");
	strcat(buffer, "git format-patch -1 ");
	strcat(buffer, stable_commit_id);
	strcat(buffer, " 1> ../../../linux-mvista-2.6/automate_generated_details.txt");	// 1> - redirected if no error i.e. stdout only

	system(buffer);

	printf("\n Generated Patch : <if blank means unable to create patch> \n");
	printf("\t\t\t\t");	fflush(stdout);
	system("cat automate_generated_details.txt");
	printf("\n");

	fp_read_patch_dets = fopen("automate_generated_details.txt", "r");
	if(fp_read_patch_dets == NULL) {
		perror("fopen");
		printf("\t ERROR: Unable to open 'automate_generated_details.txt'. Exiting... \n\n");
		exit(1);
	}

	if( fscanf(fp_read_patch_dets, "%s", buffer_fp_read) == EOF) {
		printf("\t ERROR: Unable to read 'generated patch file name' from 'automate_generated_details.txt'. Exiting... \n\n");
		exit(1);
	}
	fclose(fp_read_patch_dets);
	
	printf(" ---------- MOVING & APPLYING generated patch into this working directory ----------- \n\n");

	strcpy(buffer, "mv ../gregkh-linux/4.19-linux/linux/");
	strcat(buffer, buffer_fp_read);
	strcat(buffer, " .");

	if( system(buffer) != 0) {	// Unable to move the patch		// system(buffer)== 0 success, failure returned 256 as %d
		printf("\n ERROR: cmd '%s' FAILED. Exiting...\n\n", buffer);
		exit(1);
	}

	strcpy(buffer, "git am ");
	strcat(buffer, buffer_fp_read);

	if( system(buffer) != 0) {	// system(buffer)== 0 success, failure returned 32768 as %d
		printf("\n ERROR: cmd '%s' FAILED. Exiting...\n\n", buffer);
		exit(1);
	}

	strcpy(buffer, "rm ");
	strcat(buffer, buffer_fp_read);
	system(buffer);

	printf("\n");
	printf(" ----------- Generating 'git describe --tag' from gregkh/linux directory ------------ \n\n");

        system("rm automate_generated_details.txt");

        strcpy(buffer, "cd ../gregkh-linux/4.19-linux/linux/ && ");
        strcat(buffer, "git describe --tag ");
        strcat(buffer, stable_commit_id);
        strcat(buffer, " 1> ../../../linux-mvista-2.6/automate_generated_details.txt");    // 1> - redirected if no error i.e. stdout only

        system(buffer);

        printf("\n Generated git describe tag : <if blank means unable to generate> \n");
	printf("\t\t\t\t");	fflush(stdout);
        system("cat automate_generated_details.txt");
        printf("\n");

        fp_read_patch_dets = fopen("automate_generated_details.txt", "r");
        if(fp_read_patch_dets == NULL) {
                perror("fopen");
                printf("\t ERROR: Unable to open 'automate_generated_details.txt'. Exiting... \n\n");
                exit(1);
        }   

        if( fscanf(fp_read_patch_dets, "%s", buffer_fp_read) == EOF) {
                printf("\t ERROR: Unable to read 'generated git describe tag' from 'automate_generated_details.txt'. Exiting... \n\n");
                exit(1);
        }   
	fclose(fp_read_patch_dets);

	strcpy(buffer_temp_tag, buffer_fp_read);		// eg- buffer_temp_tag = v4.19.292-58-g44f69c9

	for(i=0; buffer_temp_tag[i]; i++)
		if(buffer_temp_tag[i] == 'g')			// break at 'g'
			break;

	strcpy(buffer_temp_tag, buffer_temp_tag+i+1);		// store in new var temp_tag as buffer_temp_tag used further

	if(stable_commit_id != strstr(stable_commit_id, buffer_temp_tag))
	{
		printf("\t commit from tag  = %s \n", buffer_temp_tag);
		printf("\t Stable commit id = %s \n", stable_commit_id);
		printf("\n\t Not Matching as per logic. Exiting...\n\n");
		exit(1);
	}

	printf(" %s: 'commit id from tag' MATCHES with 'stable_commit_id' \n\n", buffer_fp_read);
	strcpy(buffer_temp_tag, buffer_fp_read);		// store back to 'buffer_temp_tag' for further use


	printf(" ------ Fetching CVE summary from bugzilla using bugz_num (automate_python.py) ------\n\n");

	system("rm automate_generated_details.txt");
	
	strcpy(buffer, "python3 automate_python.py ");
	strcat(buffer, bugz_num);
	strcat(buffer, " 1> automate_generated_details.txt");
	system(buffer);
	//system("echo \"CVE-2023-3772 kernel: DUMMY\" > automate_generated_details.txt"); // DUMMY // Fix Me

	printf(" bugz_num: %s , Fetched following CVE summary: \n", bugz_num);
	printf("\t\t\t\t");	fflush(stdout);
	system("cat automate_generated_details.txt");
	printf("\n");
	
        fp_read_patch_dets = fopen("automate_generated_details.txt", "r");
        if(fp_read_patch_dets == NULL) {
                perror("fopen");
                printf("\t ERROR: Unable to open 'automate_generated_details.txt'. Exiting... \n\n");
                exit(1);
        }   

        if( fgets(buffer_fp_read, 199, fp_read_patch_dets) == NULL) {
                printf("\t ERROR: File 'automate_generated_details.txt' looks EMPTY. Exiting... \n\n");
                exit(1);
        }   
	fclose(fp_read_patch_dets);

	if( strstr(buffer_fp_read, "CVE") == 0) {	// 'CVE' word not found in the 'buffer_fp_read'
                printf("\t ERROR: 'CVE' id NOT found in 'buffer_fp_read'. Exiting... \n\n");
		exit(1);
	}
	

        printf(" --------------------- Adding mvista header in the git commit ----------------------- \n\n");

	strcpy(buffer, "git commit-mvauto --amend ");
	strcat(buffer, " --bugz ");		strcat(buffer, bugz_num);
	strcat(buffer, " --source ");		strcat(buffer, "https://git.kernel.org/");
	strcat(buffer, " --type ");		strcat(buffer, "\"Security Fix\"");
	strcat(buffer, " --disposition ");	strcat(buffer, "\"Backport from \"");		strcat(buffer, buffer_temp_tag);
	strcat(buffer, " --changeid ");	strcat(buffer, stable_commit_id);
	strcat(buffer, " --no-edit");		// Keep EDIT OPTION to check manually if the log is correct

	if( system(buffer) != 0) {	// Not sure when it might return failure
		printf("\n ERROR: cmd '%s' FAILED. Exiting...\n\n", buffer);
		exit(1);
	}

	printf("\n");
	printf("\n\t\t Opening 'git show' in 3 seconds. Press 'Ctrl^C' to Terminate. Confirm if the header is correct \n");
	system("sleep 3 && git show");
	printf("\n\n");

	printf(" ------------- Do you want to proceed to create Tag & push Upstream ? --------------- \n\n");
	printf("\t\t Provide Input '-b Bug number', '-r Patch revision', '-n No of patches to push'  \n\n");
	printf(" ------------------ Press Enter to continue / Ctrl^C to Terminate ------------------- \n");
	getchar();

	printf("\t\t CONFIRM the details for Creating & Pushing the Tag \n\n");
	printf("\t Bugz number is: %s \n", bugz_num);
	printf("\t Assume Revision of Tag is '1'? [y/n] : ");
	scanf(" %c", &choice_y_n);
	
	if(choice_y_n != 'y' && choice_y_n != 'Y' && choice_y_n != 'n' && choice_y_n != 'N') {
		printf("\n\n UNRECOGNISED INPUT. Expected [y/n]. Exiting... \n\n");
		exit(1);
	}		
	else {
		if(choice_y_n == 'y' || choice_y_n == 'Y')
			strcpy(script_ip_revision_r, "1");
		else {
			printf("\t Enter the Revision of Tag {1,2,3,etc}: ");
			scanf("%s", script_ip_revision_r);
			
			if(strstr("1023456789", script_ip_revision_r) == 0) {
				printf("\n\n UNRECOGNISED INPUT. Expected {1,2,3,etc}. Exiting... \n\n");
				exit(1);
			}
		}
	}

	printf("\t Assume Number of Patches  '1'? [y/n] : ");
	scanf(" %c", &choice_y_n);
	
	if(choice_y_n != 'y' && choice_y_n != 'Y' && choice_y_n != 'n' && choice_y_n != 'N') {
		printf("\n\n UNRECOGNISED INPUT. Expected [y/n]. Exiting... \n\n");
		exit(1);
	}		
	else {
		if(choice_y_n == 'y' || choice_y_n == 'Y')
			strcpy(script_ip_patches_n, "1");
		else {
			printf("\t Enter the Number of Patches {1,2,etc}: ");
			scanf("%s", script_ip_patches_n);
			
			if(strstr("1023456789", script_ip_patches_n) == 0) {
				printf("\n\n UNRECOGNISED INPUT. Expected {1,2,etc}. Exiting... \n\n");
				exit(1);
			}
		}
	}
	
	printf("\n");

	strcpy(buffer, "python3 automate_send_pull_req.py -b ");
	strcat(buffer, bugz_num);
	strcat(buffer, " -r ");
	strcat(buffer, script_ip_revision_r);
	strcat(buffer, " -n ");
	strcat(buffer, script_ip_patches_n);
	strcat(buffer, " -v");
	
	printf(" Confirm if the generated COMMAND is as expected : \"%s\" \n\n\n", buffer);


	printf(" --------- Create & PUSH the Merge Request Tag (automate_send_pull_req.py) ---------- \n\n");
	printf("\t\t Using automate_send_pull_req.py to create & Push the tag \n\n");
	printf(" ------------------ Press Enter to continue / Ctrl^C to Terminate ------------------- \n");
	getchar();	// Uncomment once dev_done - Fix me
	getchar();	// Uncomment once dev_done - Fix me
	
	printf("\n\"\"\"\n");
	system(buffer);
	
	printf("\n\"\"\"\n");
}

void print_bug_num_stable_commit_id(char *bugz_num, char *stable_commit_id)
{
	printf("\n");
	printf("\t Entered bugz_number is : %s \n", bugz_num);
	printf("\t Entered stable_commit_id is : %s \n", stable_commit_id);
	printf("\n");
}

bool check_input_length(char *input, int correct_length, char *input_name_label)
{
	if(strlen(input) == correct_length)
		return true;

	if(strcmp(input_name_label, "bugz_num") == 0) {
		printf("\n\t 'bugz_num' entered is incorrect. strlen() returned = %ld. Should be 6. \n", strlen(input));
		return false;
	}
	if(strcmp(input_name_label, "stable_commit_id") == 0) {
		printf("\n\t 'stable_commit_id' #Hash entered is incorrect. strlen() returned = %ld. Should be 40. \n", strlen(input));
		return false;
	}
}