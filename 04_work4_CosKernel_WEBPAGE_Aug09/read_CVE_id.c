#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

////// Make sure the CSV file is downloaded in the downloads folder 
//	Reference: https://stackoverflow.com/questions/4302027/how-to-open-a-url-in-python


void create_python_code(void);
void open_tabs_browser(int);

void main(int argc, char **argv)
{

	char buf_fscanf[20]={0}, buf_fgets[500]={0}, buf_cmd[50]={0}, buffer[100]={0};

	if(argc!=2)
	{
		printf("\n\t Invalid. Usage: ./a.out <csv_file_full_path> \n\n");


		printf("Press any key to jump to 'Create Python Code' or else Ctrl+C to terminate \n\n");
		getchar();
		create_python_code();
		return;
	}

/*	strcpy(buffer, "cp ");
	strcat(buffer, argv[1]);
	strcat(buffer, " /home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/BugList.csv");

	system(buffer);

	// system("cp /home/shubham/Downloads/bugs-2022-08-18.csv /home/shubham/.sk/work7_opensource/");
*/
	strcpy(buffer, "/home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/");
	strcat(buffer, argv[1]);

/*	FILE *fp=fopen("/home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/BugList.csv", "r");	*/
	FILE *fp=fopen(buffer, "r");

	if(fp)
	{
		fgets(buf_fgets,490,fp);	printf(" ommiting first line ->  _%s  \n", buf_fgets); // went to the new-line
	

		FILE *fw=fopen("/home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/CVE_nums_extracted.txt", "w");

		while( fscanf(fp, "%s", buf_fscanf) != EOF)
		{
			char* CVE_Start=strstr(buf_fgets, "CVE");	//CVE_Start =  CVE-2016-3709 libxml: Incorrect server side include OR

			if(CVE_Start)
			{
			
				/*
				if(*CVE_Start == 'C');	//{	printf(" OK ->   ");	fflush(stdout);	}
					else	CVE_Start++;

				if(*CVE_Start == 'C')   {	printf(" OKnow-> ");    fflush(stdout); }
		                        else
						printf(" ISSUE HERE \n");		*/

				for(int i=0; i<20; i++)
				{
					     if( *(CVE_Start + i) == ' ' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == ',' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == '.' )	*(CVE_Start+i) = '\0';
					else if( *(CVE_Start + i) == ':' )	*(CVE_Start+i) = '\0';
				}

				fprintf(fw, "%s\n", CVE_Start);
				
 			}

			printf("CVE_Start =  %s \n", CVE_Start);
		
			fgets(buf_fgets,490,fp);	// GOTO next Line
		}

		fclose(fw);
		fclose(fp);		

		create_python_code();

	}
	else
		perror("fopen");
}


void create_python_code(void)
{
	char buffer[500]={0}, buf_fscanf[20]={0};
	int i=1;
	
	FILE *fpy=fopen("/home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/code_web.py", "w");
	
	fprintf(fpy, "import time\n");
	fprintf(fpy, "import webbrowser\n");
	fprintf(fpy, "\n");
	

	FILE *fp=fopen("/home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/CVE_nums_extracted.txt", "r");

	while( fscanf(fp, "%s", buf_fscanf) != EOF)
	{
		strcpy(buffer, "webbrowser.open('");

		if(i != 1)
		{
			strcat(buffer, "https://access.redhat.com/security/cve/");
		
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

	printf(" Run following command now (Copied to Clipboard  Ctrl+Shift+v  ): \n\n");
	printf("\t python /home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/code_web.py \n\n");


	system("xclip -selection primary /home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/.copy_cmd");
	//system("xclip -selection clipboard /home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/.copy_cmd");
	
	fflush(stdout);

	sleep(3);

	system("gedit /home/shubham/.sk/04_work4_CosKernel_WEBPAGE_Aug09/code_web.py");


}
