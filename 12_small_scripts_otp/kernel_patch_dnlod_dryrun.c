#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main(int argc, char *argv[])
{

printf(" Usage: skp1 <commit_id_only/commit_IDdnd->donotdelete_patch> \n\n");

if(argc!=2)
exit(1);

	char buffer[500]={0};
	char commit_id[50]={0};
	char do_not_del_flag=0;

	if(strlen(argv[1])!=40 && strlen(argv[1])!=43) {
		printf("\n Invalid commit id, strlen()=%ld\n\n", strlen(argv[1]));
		exit(1);
	}

	if(strlen(argv[1]) == 43)
		do_not_del_flag=1;

	strncpy(commit_id, argv[1], 40);

	strcpy(buffer, "wget https://github.com/gregkh/linux/commit/");
	strcat(buffer, commit_id);
	strcat(buffer, ".patch");

	system(buffer);

	strcpy(buffer, "patch -p1 < ");
	strcat(buffer, commit_id);
	strcat(buffer, ".patch");
	strcat(buffer, " --dry-run");

	system(buffer);

	if(do_not_del_flag == 1)
		exit(0);

	printf("\n\t Deleting the downloaded patch \n\n");

	strcpy(buffer, "rm ");
	strcat(buffer, commit_id);
	strcat(buffer, ".patch");

	system(buffer);
}
