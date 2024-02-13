#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char *argv[])
{
	int i_start_patch=0, i_end_patch=0, i_patch_in_progress;
	char str_below_9999[5]={0}, str_above_9999[6]={0};
	char buffer[500]={0};


	printf(" Usage: ./a.out <start_patch_no> <end_patch_no> \n");

	if(argc!=3)
	return;


	i_start_patch = atoi(argv[1]);
	  i_end_patch = atoi(argv[2]);


	printf("%d start, %d end \n\n", i_start_patch, i_end_patch);


	if(i_start_patch > 0 && i_start_patch <= 9999 && i_end_patch <=9999 && i_end_patch > i_start_patch)
	{

		for(i_patch_in_progress=i_start_patch; i_patch_in_progress<=i_end_patch; i_patch_in_progress++)		
		{
			sprintf(str_below_9999, "%4d", i_patch_in_progress);

			if(str_below_9999[0]==' ') str_below_9999[0] = '0';
			if(str_below_9999[1]==' ') str_below_9999[1] = '0';
			if(str_below_9999[2]==' ') str_below_9999[2] = '0';

			//printf("_%s_ \n", str_below_9999);
			
			strcpy(buffer, "git am ../zsk_all_patches-18333_main_branch/");
			strcat(buffer, str_below_9999);
			strcat(buffer, "-*");
			strcat(buffer, " && ");
			strcat(buffer, "git push -u origin master");
			
//			printf("buffer is _%s_ \n", buffer);
		
			if(system(buffer) != 0) break;

			printf("\n_ <- applied number %d (loop number %d/%d)_\n\n", i_patch_in_progress, i_patch_in_progress-i_start_patch+1, \
													 i_end_patch-i_start_patch+1);
		}
		printf("\n\t i_patch_in_progress (not applied) = %d \n", i_patch_in_progress);
	}
	else if(i_start_patch >= 9999 && i_start_patch<=18333 && i_end_patch <=18333 && i_end_patch > i_start_patch)
	{

		for(i_patch_in_progress=i_start_patch; i_patch_in_progress<=i_end_patch; i_patch_in_progress++)	
		{
			sprintf(str_above_9999, "%5d", i_patch_in_progress);
		
			if(str_above_9999[0]==' ') str_above_9999[0] = '0';
			if(str_above_9999[1]==' ') str_above_9999[1] = '0';
			if(str_above_9999[2]==' ') str_above_9999[2] = '0';
			if(str_above_9999[3]==' ') str_above_9999[3] = '0';

			//printf("_%s_ \n", str_above_9999);

			strcpy(buffer, "git am ../zsk_all_patches-18333_main_branch/");
			strcat(buffer, str_above_9999);
			strcat(buffer, "-*");
			strcat(buffer, " && ");
			strcat(buffer, "git push -u origin master");
	
//			printf("buffer is _%s_ \n", buffer);			

			if(system(buffer) != 0) break;

			printf("\n_ <- applied number %d (loop number %d/%d)_\n\n", i_patch_in_progress, i_patch_in_progress-i_start_patch+1, \
													 i_end_patch-i_start_patch+1);

		}
		printf("\n\t i_patch_in_progress (not applied) = %d \n", i_patch_in_progress);
	}


}

