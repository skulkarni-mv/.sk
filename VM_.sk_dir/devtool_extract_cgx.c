#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Output cmd: /bin/sdevtool
// gcc ~/.sk/devtool_extract_cgx.c -o ~/.sk/sdevtool; sudo mv ~/.sk/sdevtool /bin/

void main(int argc, char **argv)
{
//	char name1[]="MACHINE=x86-generic-64 devtool modify --extract --no-same-dir ";
	char name1[]="devtool modify --extract --no-same-dir ";
//	char name2[]=" ~/packages_cgx2.4/cgx-";			// CGX2.4 packages
	char name2[]=" ~/packages/cgx-";
	char name3[]=" --branch cgx-";
	
	char total[150]={0}, buffer[200]={0};


	if(argc != 2)
	{	printf("Package not mentioned / Space given / argc!=2 \n");
		exit(0);
	}
	
//	printf("%s  %ld \n", argv[1], strlen(argv[1]));

	strcpy(total,name1);
	strcat(total,argv[1]);
	strcat(total,name2);
	strcat(total,argv[1]);
	strcat(total,name3);
	strcat(total,argv[1]);

	//printf("Running : %s\n\n",total);

	strcpy(buffer, "tsk \"");	strcat(buffer, total);	strcat(buffer, "\""); // Added by Shubham on 2022-09-29 for Buildtime Info
	
	system(buffer);			//system(total);
}
