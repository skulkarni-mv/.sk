#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Output cmd: /bin/bbb
// gcc ~/.sk/bb_build_sk.c -o ~/.sk/bbb; sudo mv ~/.sk/bbb /bin/

void main(int argc, char *argv[])
{

        char buffer[500]={0};

        if(argc > 1)
        {
                strcpy(buffer, "tsk \"");
                strcat(buffer, "bitbake");

                for(int i=1; i < argc; i++)             // bbb core-image-minimal -> argc=2, argv[0]=bbb / argv[1]=core-image-minimal
                {                                       // i=1 / i<2 OK  	  -> "bitbake " + argv[1]/core-image-minimal
                        strcat(buffer, " ");
                        strcat(buffer, argv[i]);        // i=2 / i<2 NOK -> EXIT
                }

                strcat(buffer, "\"");

                //printf("Running: _%s_ \n\n", buffer);

                system(buffer);
        }
        else
        {
                printf("\n\t Invalid. Usage \"bbb <argument> (ex- bbb core-image-minimal / bbb -s)\" \n");
                return;
        }

}
