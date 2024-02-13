#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if(argc!=2) {
	//			argv[0] argv[1]
		printf(" Usage: ./a.out <patch_file to edit> \n\n");
		return 1;
	}

	FILE *fp_r, *fp_w;
	char buffer[500]; 

	fp_r = fopen(argv[1], "r");	
	if(!fp_r)				// error opening file
	{	perror("fopen");
		return 1;
	}

	fp_w = fopen("created-file.patch", "w");	
	
	printf(" OK \n");

	fseek(fp_r, 1, SEEK_CUR);

	//printf(" ");	fflush(stdout);

	while( fgets(buffer, 490, fp_r) != NULL) { printf("%s", buffer); //printf(" ommiting first line ->  _%s_", buffer);	
	fprintf(fp_w, "%s", buffer);

	fseek(fp_r, 1, SEEK_CUR);
	}

}
