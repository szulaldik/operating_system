#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//it takes 2 char pointer(string) and returns their combinations
char *strcon(const char *s1, const char*s2) 
{
	char *pd = malloc (strlen(s1) + strlen(s2) + 1); 
	if (!pd) 
	{ 
	printf("cannot allocate memory!\n"); 
	exit(EXIT_FAILURE); 
	} 
	strcpy(pd, s1); 
	strcat(pd, s2); 
	return pd;
}
int main(int argc, char *argv[]){
	
	//opens a file with mode 'a' with file pointer fptr
 	FILE * fptr;
	fptr = fopen(argv[0],"a");
	if(fptr==NULL)
	{
		printf("file error\n");
		exit(1);
	}
	else{ 
		//take the time
		time_t tm;
		time(&tm);
		char * time = ctime(&tm);
		
		// concatane "Current Date/Time = " and the time. And do not take '/n'
    		char * timeStr = strcon("Current Date/Time = ", time);
    		timeStr=strtok(timeStr,"\n");
    		
    		//print time pid and ppid to the file and close it
    		fprintf(fptr,timeStr);
    		fprintf(fptr," pid:%d ppid:%d\n",getpid(),getppid());
		fclose(fptr);
		free(timeStr);
		return 0;
	}

}
