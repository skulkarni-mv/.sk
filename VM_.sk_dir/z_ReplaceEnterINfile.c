#include<stdio.h>

void main()
{
   	FILE *fp=fopen("/home/qauser/.sk/SKcp.txt","r+");
 
        if(fp)                  // check if file exists
        {
        	fseek(fp,-1,SEEK_END);          // Replacing Enter '\n' with space ' ' at EOF
                fputc('\0',fp);
                fclose(fp);
 	}
}

