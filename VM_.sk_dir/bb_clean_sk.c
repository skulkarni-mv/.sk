#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Output cmd: /bin/bbc
// gcc ~/.sk/bb_clean_sk.c -o ~/.sk/bbc; sudo mv ~/.sk/bbc /bin/

void main(int argc, char *argv[])
{

        char buffer[500]={0};

        if(argc > 1)
        {
                strcpy(buffer, "tsk \"");
                strcat(buffer, "bitbake -c clean");

                for(int i=1; i < argc; i++)             // bbc go-runtime -> argc=2, argv[0]=bbc / argv[1]=go-runtime
                {                                       // i=1 / i<2 OK   -> "bitbake " + argv[1]/go-runtime
                        strcat(buffer, " ");
                        strcat(buffer, argv[i]);        // i=2 / i<2 NOK -> EXIT
                }

                strcat(buffer, "\"");

                //printf("Running: _%s_ \n\n", buffer);

                system(buffer);
        }
        else
        {
                printf("\n\t Invalid. Usage \"bbc <argument> (ex- bbc go-runtime / bbc all?)\" \n");
                return;
        }

}
