#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


// $ gcc code2_bugz_updater_batch.c -o run2_bugz_updater_batch.out
// $ Usage: ./run2_bugz_updater_batch.out <dumped_data_file_generated_from_code1>


#define bugz_comment_dry_run 1

//#define update_bugz_confirm 1

//#define debug_print 1


/*
TODO: clear char arrays with memset() in loop & cmd_from_flags_to_python
Add whiteboard for skulkarni 2.6 bugs, don't update these bugs in next run
don't update sync req bugs of CGX till fix in stable // CHECK THIS if same whiteboard like 2.6 can be added
Don't touch bugs with multiple commits as a fix
*/


int format_comment(char chr_bugz_uname, char chr_cmnt_chk_files, char chr_bugz_status_chng_to);


char bugz_num[10]={0}, stable_cgit_link[150]={0}, stable_version[15]={0}, stable_github[100]={0}, ubuntu_cve_link[50]={0}, master_github[100]={0};
int loop_counter=0;

char bugz_product[15]={0}, bugz_prio[10]={0}, bugz_status[15]={0}, bugz_assignee[30]={0}, bugz_date_report[40]={0};
char flag_update_bug_y_n[2]={0}, flag_date_ok_nok[5]={0}, flag_date_difference[5]={0}, flag_take_on_name[5]={0}, flag_reason_of_N[9]={0};
char mv_gitcgx_link[150]={0}, cmd_from_flags_to_python[200]={0};

bool override_update_bugz_confirm=false;

void decode_flags_create_command()
{

	memset(cmd_from_flags_to_python, '\0', 200);

	if( strcmp(flag_update_bug_y_n, "Y") == 0 ) {		// match found, i.e. "y" flag found - Update Bugz

		if( strcmp(bugz_product, "CGX2.6") == 0 ) {	// Can't update SYNC_REQ or RESO as Manual Patch. No need of 'flag_date_ok_nok'.

			if( strcmp(bugz_status, "NEW")==0 || strcmp(bugz_status, "ASSIGNED")==0 || strcmp(bugz_status, "REOPENED")==0 ) {

				if( strcmp(flag_take_on_name, "namY") == 0 ) {
										// Uncomment 2 lines to check for CGX2.6 / else Ignore completely
/*uncomment*/				strcat(cmd_from_flags_to_python, "_uname=Yes_");

/*uncomment*/				strcat(cmd_from_flags_to_python, "_cmnt=chkCodeFilesIfAffected_");	// Only Update if on your name
				}

				if( strstr(mv_gitcgx_link, "https://gitcgx.mvista.com/cgit/CGX") != 0) {	// found fix -> Rare in CGX2.6

					strcat(cmd_from_flags_to_python, "_patch=already_");
				}
//				else {	strcat(cmd_from_flags_to_python, "xxx_patchx=xmanual_xxx");} // don't update if not on your name
			}

			if( strlen(cmd_from_flags_to_python) == 0 ) {				// both if() fails
				strcat(cmd_from_flags_to_python, "_final=NoUpdate_");	// don't update which are not your name
			}
#ifdef debug_print
			printf("\n\n---%s---\n", cmd_from_flags_to_python);
#endif
		}


		else if( strcmp(bugz_product, "CGX3.1")==0 || strcmp(bugz_product, "CGX4.0")==0 || strcmp(bugz_product, "CGX5.0")==0 ) {

			if( strcmp(bugz_status, "NEW")==0 || strcmp(bugz_status, "ASSIGNED")==0 || strcmp(bugz_status, "REOPENED")==0 ) {
												// don't take for 'SYNC_REQ' & 'IN_REVIEW'
				if( strcmp(flag_take_on_name, "namY")==0 ) {

					strcat(cmd_from_flags_to_python, "_uname=Yes_");
				}
			}

			if( strstr(mv_gitcgx_link, "https://gitcgx.mvista.com/cgit/CGX") != 0) {	// found fix

				strcat(cmd_from_flags_to_python, "_patch=already_");			// Not doing date check 'dOK' / 'dNOK'
			}
			else {		// if NO FIX found in gitcgx

				if( strcmp(bugz_status, "SYNC_REQ")==0 || strcmp(bugz_status, "IN_REVIEW")==0 ) {	// In_Rev also-> No Change
					strcat(cmd_from_flags_to_python, "_final=NoUpdate_");
				}
				else {
					strcat(cmd_from_flags_to_python, "_patch=sync_req_");
				}
			}
#ifdef debug_print
			printf("\n\n---%s---\n", cmd_from_flags_to_python);
#endif
		}

//---------------------------------------------------------------------------------------

/*			"_uname=Yes_"				-> Take bugz on your name
			"_cmnt=chkCodeFilesIfAffected_"		-> Add comment to check if the affected files are compiled
			"_patch=already_"			-> Change bugz status to RESOLVED FIXED
			_patch=sync_req_			-> Change bugz status to SYNC_REQ
			"_final=NoUpdate_"			-> Do Not touch/update the bug
*/

		if( strstr(cmd_from_flags_to_python, "_final=NoUpdate_") != 0 ) {			// NOT Found string
			printf("\n\t NOT UPDATING Bug: %s. Reason: '_final=NoUpdate_'.\n", bugz_num);
		}
		else {

			char chr_bugz_uname=0, chr_cmnt_chk_files=0, chr_bugz_status_chng_to=0;

			if(strstr(cmd_from_flags_to_python, "_uname=Yes_") != 0 )
				chr_bugz_uname='Y';
			if(strstr(cmd_from_flags_to_python, "_cmnt=chkCodeFilesIfAffected_") != 0 )
				chr_cmnt_chk_files='Y';

			if(strstr(cmd_from_flags_to_python, "_patch=already_") != 0 )
				chr_bugz_status_chng_to='R';
			else if(strstr(cmd_from_flags_to_python, "_patch=sync_req_") != 0 )
				chr_bugz_status_chng_to='S';

			if( chr_bugz_uname==0 && chr_cmnt_chk_files==0 && chr_bugz_status_chng_to==0 ) ;
			else

#ifdef bugz_comment_dry_run
				format_comment(chr_bugz_uname, chr_cmnt_chk_files, chr_bugz_status_chng_to);
#endif
		}

	} // "if( strcmp(flag_update_bug_y_n, "Y") == 0 ) {" ENDs
	else {
		printf("\n\n\t NOT UPDATING Bug: %s. Reason: %s.\n", bugz_num, flag_reason_of_N);
	}

}

void main(int argc, char *argv[])
{

	char buffer[500]={0}, buffer_fp_read[200]={0};
	char *ptr_pos=NULL, char_tmp_read=EOF, prev_bugz_inloop[10]={0};
	int i=0, j=0;

	FILE *fp_read=NULL, *fp_write=NULL;

	if     (argc == 2);

	else if(argc == 3 && !strcmp(argv[2], "--upbugz") ) {

		override_update_bugz_confirm = true;
	}
	else
	{
		printf("\n\t Usage: ./run2_bugz_updater.out <'dumped_data'_file_path> {OPT: --upbugz}\n\n");
		exit(1);
	}


#ifdef update_bugz_confirm
	printf("\n ----------- Current settings in the code says, UPDATE the BUGs on bugz site. Are you sure ?? ---------- \n");

	printf("\n --------------------------- Press Enter to continue / Ctrl^C to Terminate ----------------------------- \n");
	getchar();
#else

      if(override_update_bugz_confirm == true) {

	printf("\n ----------- Command option passed: '--upbugz', UPDATE the BUGs on bugz site. Are you sure ?? ---------- \n");

	printf("\n --------------------------- Press Enter to continue / Ctrl^C to Terminate ----------------------------- \n");
	getchar();
      }
#endif


	fp_read=fopen(argv[1], "r");
	if(!fp_read) {
		perror("fopen");
		exit(1);
	}

	fp_write=fopen("weblinks_bugz_generated.py", "w");	// Create a new file with bugz site & gitcgx search links side by side

	fprintf(fp_write, "#!/usr/bin/python3\n\n");
	fprintf(fp_write, "import time\n");
	fprintf(fp_write, "import webbrowser\n\n");
	fclose(fp_write);

	system("chmod u+x weblinks_bugz_generated.py");
									// seems like fscanf() can not read '\n'

	while( fscanf(fp_read, "%s", buffer_fp_read) != EOF )
	{

		if( strstr(buffer_fp_read, "Bug") != 0)
		{
			strcpy(bugz_num, buffer_fp_read+4);					// "Bug:abcdef", 'a'->[4]

//			printf("%s  \t", bugz_num);	fflush(stdout);				// '-' ASCII 45, '\n', 10

			fgetc(fp_read);								// read 2 spaces after Bug:abcdef
			fgetc(fp_read);								// will be space ' ' ASCII 32

			char_tmp_read = fgetc(fp_read);						// check if '-' OR 'h' of https://
			if( char_tmp_read!=EOF && ( char_tmp_read=='-' || char_tmp_read=='x') ) {

				char_tmp_read = fgetc(fp_read);					// will be '\n'
				continue;						// SKIP THE LOOP
			}

			else if(char_tmp_read != EOF && char_tmp_read=='h')			// 'https://' stable commit link
			{
				if(strcmp(prev_bugz_inloop, bugz_num) != 0) {			// another '\n' printed if different bugz

					printf("\n %3d ", ++loop_counter);			// PRINT LOOP NUMBER
					for(j=0; j<145; j++) {
						printf("_");
					}
					printf("\n");
				}
//#ifdef debug_print
				printf("\n Bug:%6s \t", bugz_num);	fflush(stdout);			// '-' ASCII 45, '\n' is 10
//#endif

				fseek(fp_read, -1, SEEK_CUR);					// go back as '-'/'h' is already read
				if( fscanf(fp_read, "%s", stable_cgit_link) != EOF)
				{
#ifdef debug_print
					printf("%s", stable_cgit_link);	fflush(stdout);
#endif
					if( strstr(stable_cgit_link, "https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git") == 0) {
						printf("\n ************ Are you sure this is stable cgit link? ************ \n");
						continue;
					}

					ptr_pos=strstr(stable_cgit_link, "/commit/?h=");
					if( ptr_pos != NULL) {					// string search successful

						i=strlen("/commit/?h=");
						strncpy(stable_version, ptr_pos+i, 14);		// => _linux-4.19.y&i_ OR _linux-5.4.y&id_

						for(j=0; j<=strlen(stable_version); j++)
							if(stable_version[j]=='&')		// find '&' and make it '\0'
								stable_version[j]='\0';
#ifdef debug_print
						printf("\n \t\t\t\t\t\t\t\t    %s", stable_version);
#endif
					}
					else {
						printf("\n ********** Unable to find stable version from stable cgit ******** \n");
						continue;
					}

				  if( fscanf(fp_read, "%s", stable_github) != EOF)
				  {
#ifdef debug_print
					printf("\n \t\t\t\t\t\t\t\t    %s", stable_github);	fflush(stdout);
#endif
					if( strstr(stable_github, "https://github.com/gregkh/linux/commit/") == 0) {
						printf("\n ************ Are you sure this is stable github link? ************ \n");
						continue;
					}

				    if(fscanf(fp_read, "%s", ubuntu_cve_link) != EOF) {
#ifdef debug_print
					printf("\n \t\t\t\t\t\t\t\t    %s", ubuntu_cve_link);	fflush(stdout);
#endif
					if( strstr(ubuntu_cve_link, "https://ubuntu.com/security/") == 0) {
						printf("\n ************ Are you sure this is ubuntu security page link? ************ \n");
						continue;
					}

				      if(fscanf(fp_read, "%s", master_github) != EOF) {
#ifdef debug_print
					printf("\n \t\t\t\t\t\t\t\t    %s", master_github);	fflush(stdout);
#endif
					if( strstr(master_github, "https://github.com/torvalds/linux/commit/") == 0) {
						printf("\n ************ Are you sure this is master github link? ************ \n");
						continue;
					}


				        if(fscanf(fp_read, "%s", bugz_product) != EOF) {		// should be '-'
#ifdef debug_print
					  printf("\n \t\t\t\t\t\t\t\t    '%s'", bugz_product);	fflush(stdout);
#endif
					  if( strcmp(bugz_product, "-") != 0 ) {			// NOT matches with "-"
						printf("\n ************ Wrong Data formatting detected. Expceted '-' before bugz details ** \n");
						continue;
					  }

				        if(fscanf(fp_read, "%s", bugz_product) != EOF) {
#ifdef debug_print
					  printf("\n \t\t\t\t\t\t\t\t    %s", bugz_product);	fflush(stdout);
#endif
					  if( strstr(bugz_product, "CGX2.6") == 0 && strstr(bugz_product, "CGX3.1") == 0 && \
					      strstr(bugz_product, "CGX4.0") == 0 && strstr(bugz_product, "CGX5.0") == 0  ) { // string not found

						printf("\n ************ Only CGX Products 2.6/3.1/4.0/5.0 are supported currently ****** \n");
						continue;
					  }

				        if(fscanf(fp_read, "%s", bugz_prio) != EOF) {
#ifdef debug_print
					  printf("\n \t\t\t\t\t\t\t\t    %s", bugz_prio);	fflush(stdout);
#endif
					  if( strstr(bugz_prio, "P") == 0 && strstr(bugz_prio, "Backlog") == 0 ) {

						printf("\n ************ Bugz Priority should be P1/P2/P3/P4/P5/Backlog ****** \n");
						continue;
					  }

				        if(fscanf(fp_read, "%s", bugz_status) != EOF) {
#ifdef debug_print
					  printf("\n \t\t\t\t\t\t\t\t    %s", bugz_status);	fflush(stdout);
#endif
					  if( strstr(bugz_status, "NEW") == 0      && strstr(bugz_status, "ASSIGNED") == 0 &&	// chk 'code1*.c'
					      strstr(bugz_status, "REOPENED") == 0 && strstr(bugz_status, "SYNC_REQ") == 0 ) {  // 0=str not found

						printf("\n ************ Bugz Status should be New/Assigned/Reopened/Sync_Req ****** \n");
						continue;
					  }

				        if(fscanf(fp_read, "%s", bugz_assignee) != EOF) {
#ifdef debug_print
					  printf("\n \t\t\t\t\t\t\t\t    %s", bugz_assignee);	fflush(stdout);
#endif
					  if( strstr(bugz_assignee, "-") != 0 ) {		// groups like 'cge7-kernel-gatekeepers'. Rare.

						printf("\n ************ Bugz Assignee id contains '-' (looks like a group). Invalid Id **** \n");
						continue;
					  }

				        if(fscanf(fp_read, "%s", bugz_date_report) != EOF) {
#ifdef debug_print
					  printf("\n \t\t\t\t\t\t\t\t    %s", bugz_date_report);	fflush(stdout);
#endif
					  if( strlen(bugz_date_report) != 10 ) {		// Date length should be 10

						printf("\n ************ Bugz Reported/Open Date is Invalid **** \n");
						continue;
					  }

				        if(fscanf(fp_read, "%s", flag_update_bug_y_n) != EOF) {
#ifdef debug_print
					    printf("\n \t\t\t\t\t\t\t\t    %s", flag_update_bug_y_n);				fflush(stdout);
#endif
						if( strcmp(flag_update_bug_y_n, "Y") == 0 ) {		// if "Y", then scan further flags

						        if(fscanf(fp_read, "%s", flag_date_ok_nok) != EOF) {
#ifdef debug_print
								printf("\n \t\t\t\t\t\t\t\t    %s", flag_date_ok_nok);		fflush(stdout);
#endif
							}
						        if(fscanf(fp_read, "%s", flag_date_difference) != EOF) {
#ifdef debug_print
								printf("\n \t\t\t\t\t\t\t\t    %s", flag_date_difference);	fflush(stdout);
#endif
							}
						        if(fscanf(fp_read, "%s", flag_take_on_name) != EOF) {
#ifdef debug_print
								printf("\n \t\t\t\t\t\t\t\t    %s", flag_take_on_name);		fflush(stdout);
#endif
							}
						}
						else {				// if flag_update_bug_y_n is "N", read reason for 'No Updation'
							if( fscanf(fp_read, "%s", flag_reason_of_N) != EOF) {
#ifdef debug_print
								printf("\n \t\t\t\t\t\t\t\t    %s", flag_reason_of_N);		fflush(stdout);
#endif
							}
						}

				        if(fscanf(fp_read, "%s", mv_gitcgx_link) != EOF) {
#ifdef debug_print
					printf("\n \t      %s", mv_gitcgx_link);		fflush(stdout);
#endif
						if( strstr(mv_gitcgx_link, "NA_gitcgx") != 0 );					// FOUND string

						else if( strstr(mv_gitcgx_link, "https://gitcgx.mvista.com/cgit/CGX") == 0) {	// NOT found
							printf("\n ************ Are you sure this is correct gitcgx commit link? ********* \n");
							continue;
						}

					decode_flags_create_command();

					}	// mv_gitcgx_link
					}	// flag_update_bug_y_n

					}	// bugz_date_report
					}	// bugz_assignee
					}	// bugz_status
					}	// bugz_prio
					}	// bugz_product
					}	// '-' before bugz_product

				      }
				    }
				  }
				}
				else {
					printf(" ERROR 1\n");
				}

			} // else if(char_tmp_read != EOF && char_tmp_read=='h') ends
//			printf("\n_%s-%s_\n", prev_bugz_inloop, bugz_num);
			printf("\n");

//			if(strcmp(prev_bugz_inloop, bugz_num) != 0)	printf("\n");		// another '\n' printed if different bugz
			strcpy(prev_bugz_inloop, bugz_num);

		} // if(strstr(buffer_fp_read, "Bug") != 0) ends

	} // while() ends

	printf(".....\n\n");
}



int format_comment(char chr_bugz_uname, char chr_cmnt_chk_files, char chr_bugz_status_chng_to)
{
	char buffer[1000]={0}, cmd_buf[1000]={0}, bugz_flds_change_cmd[200]={0};
	FILE *fp_write=NULL;


	strcpy(buffer, "From AutoScript\n\n");
	strcat(buffer, "Fixed in Master by ");
	strcat(buffer, master_github);
	strcat(buffer, "\nRef: ");
	strcat(buffer, ubuntu_cve_link);
	strcat(buffer, "\n\nPatch available in stable kernel ");
	strcat(buffer, stable_version);
	strcat(buffer, " :\n");
	strcat(buffer, stable_cgit_link);
	strcat(buffer, "\n");
	strcat(buffer, stable_github);


	if(chr_bugz_uname == 'Y')
		strcat(bugz_flds_change_cmd, "_assignee=uname_");

	if(chr_cmnt_chk_files == 'Y') {

		strcat(buffer, "\n\nCheck if the affected files are getting compiled (Invalid bugz, if not)");
	}

	if(chr_bugz_status_chng_to == 'R') {

		strcat(buffer, "\n\nMerged: ");
		strcat(buffer, mv_gitcgx_link);
		strcat(buffer, "\n\nMarking Resolved Fixed. Update 'Depends on:' with MR of the above gitcgx commit");

		strcat(bugz_flds_change_cmd, "_status=Reso__whiteb=clear_");
	}
	else if(chr_bugz_status_chng_to == 'S') {

		strcat(buffer, "\n\nMarking as SYNC_REQ");

		strcat(bugz_flds_change_cmd, "_status=Sync_Req__whiteb=cgx_sync_");
	}

//	printf("\"\n%s\n\"", buffer);


	strcpy(cmd_buf, "python3 update_bugz_comment.py ");
	strcat(cmd_buf, bugz_num);
	strcat(cmd_buf, " \"");
	strcat(cmd_buf, buffer);
	strcat(cmd_buf, "\"");

	if( strlen(bugz_flds_change_cmd) != 0 ) {
		strcat(cmd_buf, " --optional ");		// 3 options here:  "_assignee=uname_" /
		strcat(cmd_buf, bugz_flds_change_cmd);		// 	"_status=Reso__whiteb=clear_" / "_status=Sync_Req__whiteb=cgx_sync_"
	}

	printf("\n\"\"\"\n%s\n\"\"\"", cmd_buf);

#ifdef update_bugz_confirm
	system(cmd_buf);
#else

      if(override_update_bugz_confirm == true) {

	system(cmd_buf);
      }
      else {

	printf("\n\n\t\t\t NOT UPDATING ON THE BUGZ AS PER THE SETTINGS IN THE CODE. \n");
      }
#endif


	/* Create a Python script to open bugz & gitcgx link side by side */

	fp_write=fopen("weblinks_bugz_generated.py", "a+");
	if(fp_write != NULL)
	{
		fprintf(fp_write, "#%3d\n", loop_counter);					// Loop Number i.e. how many bugs/bug count
		fprintf(fp_write, "webbrowser.open('http://bugz.mvista.com/show_bug.cgi?id=%s')\n", bugz_num);
		fprintf(fp_write, "time.sleep(0.5)\n\n");
		fclose(fp_write);
	}

	return 1;
}
