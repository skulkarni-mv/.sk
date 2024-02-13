#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Output cmd: /bin/smake
// gcc ~/.sk/smake_build.c -o ~/.sk/smake; sudo mv ~/.sk/smake /bin/

void main(int argc, char *argv[])
{

	char buffer[500]={0};

	if(argc > 1)
	{
		strcpy(buffer, "tsk \"");
		strcat(buffer, "make");
	
		for(int i=1; i < argc; i++)		// smake build   -> argc=2, argv[0]=smake / argv[1]=build
		{					// i=1 / i<2 OK  -> "make " + argv[1]/build
			strcat(buffer, " ");
			strcat(buffer, argv[i]);	// i=2 / i<2 NOK -> EXIT
		}

		strcat(buffer, "\"");

		//printf("Running: _%s_ \n\n", buffer);

		system(buffer);
	}
	else
	{
		printf("\n\t Invalid. Usage \"smake <argument> (ex- smake build / smake clean)\" \n");
		return;
	}

}

