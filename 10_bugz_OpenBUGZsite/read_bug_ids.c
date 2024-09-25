#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//	Reference: Dir 04_work4_CosKernel_WEBPAGE_Aug09/


void create_python_code(void);
void open_tabs_browser(int);

void main(int argc, char **argv)
{
	create_python_code();
}


void create_python_code(void)
{
	char buffer[500]={0}, buf_fscanf[20]={0};
	int i=1;
	
	FILE *fpy=fopen("/home/shubham/.sk/10_bugz_OpenBUGZsite/code_web.py", "w");
	
	fprintf(fpy, "#!/usr/bin/python3\n\n");
	fprintf(fpy, "import time\n");
	fprintf(fpy, "import webbrowser\n");
	fprintf(fpy, "\n");
	

	FILE *fp=fopen("/home/shubham/.sk/10_bugz_OpenBUGZsite/bugz_numbers.txt", "r");
	if(!fp)
	{
		perror("fopen");
		exit(1);
	}
	
	while( fscanf(fp, "%s", buf_fscanf) != EOF)
	{
		strcpy(buffer, "webbrowser.open('");

		if(i != 1)
		{
			strcat(buffer, "http://bugz.mvista.com/show_bug.cgi?id=");
		
			strcat(buffer, buf_fscanf);	// buffer = CVE-xxxx-xxxx
			strcat(buffer, "')\n");

			fprintf(fpy, "%s", buffer);		

				fprintf(fpy, "time.sleep(0.1)\n");
		}
		else
		{
			strcat(buffer, "https://www.google.com/");	
			strcat(buffer, "')\n");

			fprintf(fpy, "%s", buffer);		

				fprintf(fpy, "time.sleep(0.5)\n");


			strcpy(buffer, "webbrowser.open('");
			strcat(buffer, "http://bugz.mvista.com/show_bug.cgi?id=");
			strcat(buffer, buf_fscanf);	// buffer = CVE-xxxx-xxxx
			strcat(buffer, "')\n");

			fprintf(fpy, "%s", buffer);	// Otherwise, first entry is getting omitted
				fprintf(fpy, "time.sleep(0.1)\n");
		}
		
		i++;
	}

	fclose(fpy);
	fclose(fp);


	open_tabs_browser(i);	
}

void open_tabs_browser(int i)
{
	system("chmod +x code_web.py");

	//system("python /home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/code_web.py")


	printf("\n\n\n");
	printf(" ******* OPENING NEW WINDOW IS SUGGESTED as multiple ( %d ) tabs will open... ******** \n\n\n", i);

	printf(" Run following command now (Copied to Clipboard  Shift+Insert  ): \n\n");
//	printf("\t python3 /home/shubham/.sk/10_bugz_OpenBUGZsite/code_web.py \n\n");
	printf("\t");   fflush(stdout);
	system("cat /home/shubham/.sk/10_bugz_OpenBUGZsite/.copy_cmd");
	printf("\n\n");

	system("xclip -selection primary /home/shubham/.sk/10_bugz_OpenBUGZsite/.copy_cmd");
	//system("xclip -selection clipboard /home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/.copy_cmd");
	
	fflush(stdout);

	sleep(3);

	system("gedit /home/shubham/.sk/10_bugz_OpenBUGZsite/code_web.py");


}
