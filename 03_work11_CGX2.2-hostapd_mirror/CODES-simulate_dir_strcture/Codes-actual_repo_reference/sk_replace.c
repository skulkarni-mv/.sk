#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main(int argc, char* argv[])
{

	char buffer[500]={0};

	system("echo \" Remember to GIT RESET HARD to commit before copying files\" ");

	if(argc !=2)
		return;

	strcpy(buffer, "cp ");
	strcat(buffer, argv[1]);
	strcat(buffer, " ../../hostap/");
	strcat(buffer, argv[1]);

	printf("_%s_\n\n", buffer);

	printf(" press Enter to continue, ^C to exit - Remember to GIT RESET HARD to commit before copying files ");
	getchar();

	system(buffer);
}
