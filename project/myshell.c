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
	//controls if "-f" is written after writef 
	if(strcmp(command[1],"-f")==0)
	{
		//the arguments to be sent to writef are created
		//(the filename will be sent)
		char * fileName[2];
		fileName[0] = command[2];
		fileName[1] = NULL;	
		int f;
		f= fork();
		int ev;
		if(f==0)//child proccess
		{
			ev=execve("writef",fileName,NULL); 
			perror("error");
			exit(0);	
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
//the input line is seperated
//and its elements is assigned to the array given as a parameter
//the last element is NULL
void strSep(char* str, char** seperated)
{

   int i = 0;
   char * token = strtok(str, " ");
   while( token != NULL ) {
      seperated[i++]=token;
      token = strtok(NULL, " ");
   }
   seperated[i]=NULL;
}

// it takes the input line as a string array (double char pointer)
//if the line is like "execx -t command" execx.c program is executed
//otherwise it prints an error message
void execx(char** command)
{
	
	//controls if "-t" is written after execx 
	int time;
	if(strcmp(command[1],"-t")==0)
	{
		//the number after -t is assigned to time
		time = strtoint(command[2]);
		if(time==-1)
		{
			puts("Invalid command");
			return;
		}
		int size =10;
		char *args[size];
		int i = 2;
		int j=0;
		//the arguments to be sent to execx are created
		//the elements after -t are taken
		while(command[i]!= NULL)
		{
			args[j++] = command[i++];
		}
		args[j]=NULL;
		int f;
		f= fork();
		int ev;
		if(f==0)//child proccess
		{
			ev=execve("execx",args,NULL);
			perror("error");
			exit(0);
				
		}
		else// parent proccess
		{
			wait(&ev);
		
		}
	}
	else
	{
		printf("Invalid command\n");
		return;		
	}
}

// controls the command entered by the user and calls 
//the necessary functions
int execute_commands(char** command){

	//if it is bash calls the bash from system
	if(strcmp(command[0],"bash")==0)
	{
		system("/bin/bash");
		return 1;	
	}
	
	//if it is cat controls if there is an other word (a filename) after cat
	else if(strcmp(command[0],"cat")==0)
	{
		//if there is a word after cat 
		//it concatenate "/bin/cat " and file name
		//and calls is from system
		//it prints the contents of the file if the file is exist
		if(command[1]!=NULL)
		{
			char * catStr =strcon("/bin/cat ",command[1]);
			system(catStr);
			free(catStr);
		}
		else
		{
			//if there is nothing after cat
			//it calls the cat from system
			system("/bin/cat");
		}
		return 1;	
	}
	//if it is ls calls the ls from system
	else if(strcmp(command[0],"ls")==0)
	{
		system("/bin/ls");
		return 1;	
	}
	//if it is clear calls the clear from system
	else if(strcmp(command[0],"clear")==0)
	{
		system("/bin/clear");
		return 1;	
	}
	//if it is execx calls the execx function with all commands 
	else if(strcmp(command[0],"execx")==0)
	{
		execx(command);
		return 1;	
	}
	//if it is writef calls the writef function with all commands
	else if(strcmp(command[0],"writef")==0)
	{
		writef(command);
		return 1;	
	}
	//if it is echo it prints the words after echo
	//note: I wrote this command because ı could not be sure 
	//how the cat command should work
	else if(strcmp(command[0],"echo")==0)
	{
		for(int i =1;command[i]!=NULL;i++)
		{
		printf("%s ",command[i]);
		}
		printf("\n");
		return 1;	
	}
	//if it is exit it finishes the program 
	else if(strcmp(command[0],"exit")==0)
	{
		exit(0);	
	}
	//if it is none of the above it prints an error message
	else
	{
		puts("Invalid Command");
	}
	return 0;
}
int main()
{
	char inputString[1000], *Args[100];
	while (1) 
	{
		printf("myshell>>");
		gets(inputString);
		strSep(inputString,Args);
		execute_commands(Args);
	}
	return 0;
}

