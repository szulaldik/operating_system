#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

// it takes a char pointer return its int value if it is a number
// otherwhise it prints an error message and returns -1
int strtoint( char* charnums)
{
 int number=0;
 int index=0;
 while(charnums[index])
 {
    if(('0'<=charnums[index]) && (charnums[index]<='9'))
    {

    if(!number)
        number= ( (int) charnums[index]) - 48;
    else
    {
        number = (number *= 10) + ((int) charnums[index] - 48);
    }
    index++;
         }
         else
         {
            number=-1;
            printf("Invalid command while expecting a number \n");
            break;
         }
 }
 return number;
}
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
// it takes the input line as a string array (double char pointer)
//if the line is like "writef -f filename" writef.c program is executed
//otherwise it prints an error message
void writef(char** command)
{
	
	if(strcmp(command[2],"-f")==0)
	{
		char * fileName[2];
		fileName[0] = command[3];
		fileName[1] = NULL;	
		int f;
		f= fork();
		int ev;
		if(f==0)//child proccess
		{
			ev=execve("writef",fileName,NULL);
			perror("error");	
		}
		else// parent proccess
		{
			wait(&ev);	
		}
	}
	else
	{
		puts("Invalid command");
		return;		
	}
}
// controls the command after execx -t part entered by the user and calls 
//the necessary functions
int execute_commands(char** command){

	//the number after -t is assigned to time
	int time=strtoint(command[0]);
	//if it is bash it prints a warning message
	if(strcmp(command[1],"bash")==0)
	{
		printf("bash can not be executed more than 1 time\n");	
		return 1;	
	}
	//if it is cat controls if there is an other word (a filename) after cat
	else if(strcmp(command[1],"cat")==0)
	{
		//if there is a word after cat 
		//it concatenate "/bin/cat " and file name
		//and calls is from system
		//it prints the contents of the file in a for loop 
		//if the file is exist
		if(command[2]!=NULL)
		{
			for(int i =0;i<time;i++){
			
				system(strcon("/bin/cat ",command[2]));
				printf("cat has been executed\n");
			}
		}
		else
		{
			//if there is nothing after cat
			//it calls the cat from system
			system("/bin/cat");
		}
		return 1;	
	}
	//if it is ls calls the ls from system 'time' time in a for loop
	else if(strcmp(command[1],"ls")==0)
	{
		//printf("ls secildi\n");
		for(int i =0;i<time;i++){
		
			system("/bin/ls");
		}

		return 1;	
	}
	else if(strcmp(command[1],"clear")==0)
	{
		system("/bin/clear");
		return 1;	
	}
	//if it is writef calls the writef function with all commands
	//'time' time in a for loop
	else if(strcmp(command[1],"writef")==0)
	{

		for(int i =0;i<time;i++){
		
			writef(command);
		}
		return 1;	
	}
	//if it is echo it prints the words after echo
	//'time' time in a for loop
	else if(strcmp(command[1],"echo")==0)
	{
	
		for(int i =0;i<time;i++){
			for(int j =2;command[j]!=NULL;j++)
			{
			printf("%s ",command[j]);
			}
			printf("\n");
		}
		return 1;	
	}
	else if(strcmp(command[1],"exit")==0)
	{
		exit(0);	
	}
	else
	{
		puts("Invalid Command");
	}
	return 0;
}
int main(int argc, char *argv[])
{
	execute_commands(argv);
}

