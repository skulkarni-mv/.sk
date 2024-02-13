#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// cmd from code: /bin/sk
/* alias sk1='cat ~/.sk/pwd_sk.txt; newl'
   alias sk2='cat ~/.sk/pwd_sk2.txt; newl' - ~/.bashrc	*/

void main(int argc, char *argv[])
{
	char buffer[200] = {0}, file_name[]="/.sk/pwd_sk.txt", file_name2[]="/.sk/pwd_sk2.txt";
	FILE *fp, *file_chk;
	char typeOfCopy = 0;

	const char *env_home = getenv("HOME");

	/* Added by SK to store previous content too */
	strcpy(buffer, "cp "); 	strcat(buffer, env_home); strcat(buffer, file_name);	// "cp /home/qauser/.sk/pwd_sk /home/qauser/.sk/pwd_sk2"
	strcat(buffer," "); 	strcat(buffer, env_home); strcat(buffer, file_name2);
	system(buffer);
	// <-

	if(argc > 3) {
		printf(" Invalid Command: %d arguments Not Supported (Max 3 supported) \n\n", argc);
		goto help_printf;
	}

	else if(argc == 3 || (argc==2 && argv[1][0]=='-' && argv[1][1]=='h')) {		// 3 OK , 4/5/more NOT OK -> Error Message
		
		if(argv[1][0] == '-') {		// 2nd argc argument
		
		//.../pwd$ sk			 => Normal directory_path_copy	-d	| argc=1  argv[0]=sk
		//.../pwd$ sk filename_in_pwd	 => Normal file_copy_with_path	-p	| argc=2  argv[0]=sk	argv[1]=filename
		//.../pwd$ sk -f filename_in_pwd => Filename_Only copy		-f	| argc=3  argv[0]=sk	argv[1]=-f	argv[2]=filename
		//.../pwd$ sk -c tsk		 => Command Copy		-c	| argc=3  ragv[0]=sk	argv[1]=-c	argv[2]=command
		
			switch (argv[1][1]) {   		// argv[1][0]='-'  | argv[1][1]='h'/'c'/'f'
			
				case 'h': typeOfCopy = 'h';	goto help_printf;		// HELP

				case 'c': typeOfCopy = 'c';
	
						strcpy(buffer, env_home);       strcat(buffer, file_name);
						fp=fopen(buffer, "w");        	// write Command_Input into filestream

						fputs(argv[2], fp); 
						fputs("\n", fp);
						fclose(fp);

						break;

				case 'f': typeOfCopy = 'f';
				
						if((file_chk=fopen(argv[2], "r")) != NULL) {	// argv[2] holds filename
							
							strcpy(buffer, env_home);	strcat(buffer, file_name);
							fp=fopen(buffer, "w");		// write Filename_Input into filestream

							fputs(argv[2], fp);
							fputs("\n", fp);

							fclose(fp);
							fclose(file_chk);
						}
						else {
						      printf("\n\t Provided file not exists in pwd. Unable to Copy File Name \n Exiting... \n");
							return;
						}

						break;

				default :
				 printf(" Invalid: \"-f\" OR \"-c\" Not provided with %d Arguments.\t Usage: \"sk -f/-c file/cmd\" \n\n", argc);
				return;
			}		
		}
		else
		{
		printf(" Invalid Command: \"-\" option not provided with %d arguments.\tUsage: \"sk -f filename\" OR \"sk -c command\"\n\n",argc);
			return;
		}
	}

	else if(argc == 1) {		//only directory path to be copied

		typeOfCopy = 'd';	// 'd' -> directory_path

		strcpy(buffer, "pwd > ");	strcat(buffer, env_home);		strcat(buffer, file_name);
		system(buffer);								//system("pwd > /home/qauser/.sk/pwd_sk");
	}

	else if(argc == 2 && argv[1][0] != '-')	// filename also to be copied
	{
		
		if((file_chk=fopen(argv[1], "r")) != NULL) {
		
			typeOfCopy = 'p';			// 'p' -> path_of_file (Copy File location with path)

			strcpy(buffer, "pwd > ");		strcat(buffer, env_home);	strcat(buffer, file_name);
			system(buffer);								//system("pwd > /home/qauser/.sk/pwd_sk");
	
			strcpy(buffer, env_home);		strcat(buffer, file_name);	// buffer=/home/qauser/.sk/pwd_sk

			fp=fopen(buffer, "r+");			//fp=fopen("/home/qauser/.sk/pwd_sk","r+");
			fseek(fp, -1, SEEK_END);
			fputc('/', fp);				// concatenate and replace EOF with '/' and inputFileName
			fputs(argv[1], fp);
			fputc('\n', fp);			// intensionally putting as below fput('replacing enter') is removing last char
			fclose(fp);

		}
		else {
			printf("\n\t No file exists in pwd with the provided name. Unable to copy File Path \n Exiting... \n");
			return;
		}
	}
	else {

		printf("\n\t Invalid Input OR Command Not Available \n");
		goto help_printf;
	}


//	Here, "file_name" file should have content which should be copied

//	printf("content of the file 'file_name' are : \n_");	fflush(stdout);
//	system("cat ~/.sk/pwd_sk.txt");
//	printf("_\n");

	strcpy(buffer, env_home);         strcat(buffer, file_name);		// buffer = "/home/qauser" + "/.sk/pwd_sk"
	
	fp=fopen(buffer,"r+");   						//fp=fopen("/home/qauser/.sk/pwd_sk","r+");   
	if(fp)					// check if file exists
	{
		fseek(fp,-1,SEEK_END);		// Replacing Enter '\n' with space ' ' at EOF
		fputc('\0',fp);			// SK CHANGED - added '\0' at end - checking if OK
		fclose(fp);
										//askubuntu.com/questions/705620/xclip-vs-xsel/898094#898094
										////system("xclip -selection clipboard /home/qauser/.sk/pwd_sk");/

		strcpy(buffer, "xclip -selection primary ");	strcat(buffer, env_home);		strcat(buffer, file_name);
		system(buffer);							//system("xclip -selection primary /home/qauser/.sk/pwd_sk");
	
										//system("xsel -p -t 1000 < /home/qauser/.sk/pwd_sk");
		
		printf("Copied: ");			fflush(stdout);	
		
		strcpy(buffer, "cat ");		strcat(buffer, env_home);	strcat(buffer, file_name);
		system(buffer);			printf("\n");			//system("cat /home/qauser/.sk/pwd_sk");	printf("\n");
		
										//system("rm /home/qauser/.sk/pwd_sk");	// so that no false data
										/*		
										//system("xclip -selection clipboard -o");
										system("xclip -selection primary -o");	
										printf("\n");	*/
	}
	else {
		printf("\n\t Unable to open file. This Serious Error. Check source code for location of this statement \n\n");
	}

	return;


help_printf:

	printf("\n  HELP: \"sk -h\"\t Usage: \n");
	printf("\t sk -> copy directory_path\n\t sk file -> copy file_path\n\t sk -f file -> copy file_name\n\t sk -c cmd  -> copy command\n");
	return;	
}
