#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


// $ gcc code2_bugz_updater_batch.c -o run2_bugz_updater_batch.out
// $ Usage: ./run2_bugz_updater_batch.out <dumped_data_file_generated_from_code1>


#define update_bugz 1
#define update_bugz_confirm 1
//#define debug_print 1


int loop_counter=0;		// used in main() & format_comment() functions

int format_comment(char *bugz_num, char *stable_cgit_link, char* stable_version, char *stable_github, char *ubuntu_cve_link, char *master_github);

void main(int argc, char *argv[])
{
	char bugz_num[10]={0}, stable_cgit_link[150]={0}, stable_version[15]={0}, stable_github[100]={0}, ubuntu_cve_link[50]={0};
	char master_github[100]={0};
	char buffer[500]={0}, buffer_fp_read[200]={0};
	char *ptr_pos=NULL, char_tmp_read=EOF, prev_bugz_inloop[10]={0};
	int i=0, j=0;

	FILE *fp_read = NULL, *fp_write = NULL;

	if(argc != 2)
	{
		printf("\n\t Usage: ./run2_bugz_updater.out <'dumped_data'_file_path> \n\n");
		exit(1);
	}


#ifdef update_bugz_confirm
	printf("\n ----------- Current settings in the code says, UPDATE the BUGs on bugz site. Are you sure ?? ---------- \n");

	printf("\n --------------------------- Press Enter to continue / Ctrl^C to Terminate ----------------------------- \n");
	getchar();

#endif


	fp_read=fopen(argv[1], "r");
	if(!fp_read) {
		perror("fopen");
		exit(1);
	}

	fp_write=fopen("web_bugz_gitcgx_links.py", "w");	// Create a new file with bugz site & gitcgx search links side by side

	fprintf(fp_write, "#!/usr/bin/python3\n\n");
	fprintf(fp_write, "import time\n");
	fprintf(fp_write, "import webbrowser\n\n");
	fclose(fp_write);

	system("chmod u+x web_bugz_gitcgx_links.py");
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
			if(char_tmp_read!=EOF && char_tmp_read == '-') {

			//	printf("Bug:%s  \t x", bugz_num);	fflush(stdout);		// '-' ASCII 45, '\n', 10

				char_tmp_read = fgetc(fp_read);					// will be '\n'
				continue;					// SKIP THE LOOP
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
#ifdef debug_print
				printf("Bug:%s  \t", bugz_num);	fflush(stdout);			// '-' ASCII 45, '\n', 10
#endif

				fseek(fp_read, -1, SEEK_CUR);
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
					if( ptr_pos != NULL) {	// string search successful

						i=strlen("/commit/?h=");
						strncpy(stable_version, ptr_pos+i, 14);		// => _linux-4.19.y&i_

						for(j=0; j<=strlen(stable_version); j++)
							if(stable_version[j]=='&')
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

#ifdef update_bugz
					format_comment(bugz_num, stable_cgit_link, stable_version, stable_github, ubuntu_cve_link, master_github);
#endif
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


int format_comment(char *bugz_num, char *stable_cgit_link, char* stable_version, char *stable_github, char *ubuntu_cve_link, char *master_github)
{
	char buffer[500]={0}, cmd_buf[600]={0};
	FILE *fp_write=NULL;
	char cgx_vers_num[5]={0}, msd_branch[20]={0}, master_cmit_hash[45]={0};


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

//	printf("\"\n%s\n\"", buffer);


	strcpy(cmd_buf, "python3 update_bugz_comment.py ");
	strcat(cmd_buf, bugz_num);
	strcat(cmd_buf, " \"");
	strcat(cmd_buf, buffer);
	strcat(cmd_buf, "\"");

	printf("\n\"\"\"\n%s\n\"\"\"", cmd_buf);

#ifdef update_bugz_confirm
	system(cmd_buf);
//	printf("\n\n\t\t\t DONE. \n\n");
#endif


	/* Create a Python script to open bugz & gitcgx link side by side */

	fp_write=fopen("web_bugz_gitcgx_links.py", "a+");
	if(fp_write != NULL)
	{
		fprintf(fp_write, "#%3d\n", loop_counter);					// Loop Number i.e. how many bugs/bug count
		fprintf(fp_write, "webbrowser.open('http://bugz.mvista.com/show_bug.cgi?id=%s')\n", bugz_num);
		fprintf(fp_write, "time.sleep(0.5)\n");


		if( strstr(stable_version, "linux-4.14.y") != 0) {		// CGX2.4
			strcpy(cgx_vers_num, "2.4");
			strcpy(msd_branch, "mvl-4.14/msd.cgx");
		}
		else if( strstr(stable_version, "linux-4.19.y") != 0) {		// CGX2.6
			strcpy(cgx_vers_num, "2.6");
			strcpy(msd_branch, "mvl-4.19/msd.cgx");
		}
		else if( strstr(stable_version, "linux-5.4.y") != 0) {		// CGX3.1
			strcpy(cgx_vers_num, "3.1");
			strcpy(msd_branch, "mvl-5.4/msd.cgx");
		}
		else if( strstr(stable_version, "linux-5.10.y") != 0) {		// CGX4.0
			strcpy(cgx_vers_num, "4.0");
			strcpy(msd_branch, "mvl-5.10/msd.cgx");
		}
		else if( strstr(stable_version, "linux-6.6.y") != 0) {		// CGX5.0
			strcpy(cgx_vers_num, "5.0");
			strcpy(msd_branch, "mvl-6.6/msd.cgx");
		}

#ifdef debug_print
		printf("\n_-%s__CGX_%s___%s_\n",stable_version, cgx_vers_num, msd_branch);
#endif

		if( (strlen(cgx_vers_num)!=0) && (strlen(msd_branch)!=0) ) {

			if(strstr(master_github, "https://github.com/torvalds/linux/commit/") != 0) {

				strcpy(master_cmit_hash, master_github+strlen("https://github.com/torvalds/linux/commit/") );

				fprintf(fp_write, "webbrowser.open('");
//				fprintf(fp_write, "https://gitcgx.mvista.com/cgit/CGX%s", cgx_vers_num);	// https search will be very slow
				fprintf(fp_write, "file://gitcgx.mvista.com/cgit/CGX%s", cgx_vers_num);		// file:// used to open fast
				fprintf(fp_write, "/kernel/linux-mvista-%s.git/log/?h=%s&qt=grep&q=%s",cgx_vers_num,msd_branch,master_cmit_hash);
				fprintf(fp_write, "')\n");
				fprintf(fp_write, "time.sleep(0.5)\n\n");
			}
		}

		fclose(fp_write);

	}

	return 1;
}
