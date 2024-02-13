#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
	FILE *fp;
	char buff[1000]={0};

	fp=fopen("patch.patch","r");
	FILE* fw=fopen("new.patch","w");

	while(fgets(buff, 999, fp) != NULL)
	{

		//printf("%s", buff);

		
		if(buff[0] != '\n' && buff[0] != '\0') //buff[0] = ' ';
		{
			strcpy(buff, buff+1);
		}
		fprintf(fw, "%s", buff);

	}

}

