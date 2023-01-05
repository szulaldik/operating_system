// C program to implement one side of FIFO
// This side writes first, then reads
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>


int client_num=0;
char * fileList[10];
int file_num;

pthread_mutex_t lock;
pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_mutex_t lock3;
pthread_mutex_t lock4;
pthread_mutex_t lock5;

// it returns the length of a char array
int str_length(char *str)
{
	int length=0;
	while(str[length]!='\0')
	{
		length++;
	}
	return length;
}

//it copies the second parameter to the first.
//if the last elemant of fileName is /n 
//then the last elemant is deleted
void getFileName(char * fileName, char * command)
{
	strcpy(fileName,command);
	int length=str_length(fileName);
	if (fileName[length-1] == '\n')
        {
            fileName[length - 1] = '\0';
        }
       

}

//the input line is seperated
//and its elements is assigned to the array given as a parameter
//the last element is NULL
void str_sep(char* str, char** seperated)
{

   int i = 0;
   char * token = strtok(str, " ");
   while( token != NULL ) {
      seperated[i++]=token;
      token = strtok(NULL, " ");
      
   }
   seperated[i]=NULL;
}

//it returns the index of file if it is in
//fileList
int return_index_of_file (char * fileName)
{
	//it search the fileName parameter in fileList
	//if it finds, returns its index
	//if can not find returns -1;
	for(int i =0;i<10;i++)
	{
		if(fileList[i]!= NULL && strcmp(fileName,fileList[i])==0){
			return i;
		}
	}
	return -1;
}

//it takes data and writes it to file if file exists 
//it returns 1
//if file does not exist returns -1
int write_to_file (char * fileName, char *str)
{
	//the index of file is taken
	int idx = return_index_of_file(fileName);
	
	//if file exists in fileList
	if (idx != -1)
    	{
    		//file is opened 
		FILE *fptr = fopen(fileName,"a+");
	    	if(fptr==NULL)
		{
			printf("file error\n");
			exit(1);
		}
		//data is written to file
		fprintf(fptr,"%s\n",str);
		
		//file is closed
		fclose(fptr);
		return 1;
	}
	else
	{
		printf("File not found\n");
		return -1;
	}
}

//it takes file name as an parameter
//if file is in fileList then it is read returns 1
//if it is not then it returns -1
int read_file(char *file_name)
{
    //the index of file is taken
    int idx = return_index_of_file(file_name);
    char c;
    
    //if file exists in fileList
    if (idx != -1)
    {
    	//file is opened 
        FILE *fptr = fopen(file_name, "r");
        
        // file is read character by character untill the end of file
        while ((c = fgetc(fptr)) != EOF)
        {
            printf("%c", c);
        }
        
        //file is closed
        fclose(fptr);
        return 1;
    }
    else
    {
        printf("File not found\n");
        return -1;
    }
}

//it takes file name as an parameter
//if file is in fileList it returns 0 if it is not and
//file number is smaller than 10,
//the file is created on system and assigned to fileList
//and return 1, if file number is equal to 10
//it return 2
int create_file (char * fileName)
{
	//the index of file is taken
	int idx=return_index_of_file(fileName);
	
	//if file in fileList return 0
	if(idx!=-1)
	{
		printf("File is already created\n");
		return 0;
	}
	
	//if file does not exist in fileList and the number of file is smaller than 10,
	//than the created file is assigned to the first null index of fileList
	if(file_num<10)
	{
		//file is opened as append mode to create if it does not exist in system
		FILE *fptr = fopen(fileName,"a");
	    	if(fptr==NULL)
		{
			printf("file error\n");
			exit(1);
		}
		fclose(fptr);
        
	
	
		for(int i =0;i<10;i++)
		{
			//NULL index is find and the file name is assigned it.
			if(fileList[i]== NULL)
			{
				fileList[i]=fileName;
				file_num++;
				break;
			}
		}
		return 1;
	}
	printf("You can not create larger than 10 file\n");
	return 2;
	
}

//it takes file name as an parameter
//if file is in fileList then it is deleted and return 1
//if file is not in fileList then return -1
int delete_file (char * fileName)
{

	//the index of file is taken
	int idx=return_index_of_file(fileName);
	
	//if file exists in fileList
	if(idx!=-1)
	{
		//it is deleted from list,
		fileList[idx]=NULL;
		//also from system
		remove(fileName);
		return 1;
	}
	
	//if file does not exist in fileList return -1
	printf("File not found\n");
	return -1;
      
}


// it prints file names in fileList it is written as control
void print_file_names()
{
	for(int i =0;i<10;i++)
	{
		if(fileList[i]!= NULL){
			printf("file name of index %d is %s\n",i,fileList[i]);
		}
	}
}

// controls the command and calls 
//the necessary functions
int execute_commands(char** command){
	
	// this parameter keeps the situation of functions
	int response;
	
	//if command is exit than the client number is reduced 1
	if(strcmp(command[0],"exit\n")==0)
	{
		printf("command detected exit\n");
		client_num--;
		
		//if there is no more client than return -4 to finish all program
		if(client_num==0)
		{
			return -4;
		}
		
		//if there are still clients returns -3
		return -3;	
	}
	else if(command[1]!=NULL)
	{
	
		//the file name is taken from commands
		char fileName[80];
		getFileName(fileName,command[1]);
		
		//if command is create create_file() function is called
		if(strcmp(command[0],"create")==0)
		{
			printf("command detected create\n");
			response=create_file(fileName);
			
			//if file is already in fileList return 0
			//if it is created return 1
			//if it can not created due to file number return 2
			if(response == 0)
			{
				return 0;
			}else if(response == 1)
			{
				return 1;
			} 
			return 2;	
		}
		//if command is read read_file() function is called
		else if(strcmp(command[0],"read")==0)
		{
			printf("command detected read\n");
			response = read_file(fileName);
			
			//if file is read return 3
			//if file is not in fileList return -1
			if(response !=-1)
			{
				return 3;
			}
			return -1;	
		}
		//if command is write write_to_file() function is called
		else if(strcmp(command[0],"write")==0)
		{
			printf("command detected write\n");
			response = write_to_file(fileName,command[2]);		
			//if data is written to file return 4
			//if file is not in fileList return -1
			if(response !=-1)
			{
				return 4;
			}
			return -1;		
		}
		//if command is delete delete_file() function is called
		else if(strcmp(command[0],"delete")==0)
		{
			printf("command detected delete\n");
			response = delete_file(fileName);
			
			//if file is deleted return 5
			//if file is not in fileList return -1
			if(response !=-1)
			{
				return 5;
			}
			return -1;	
		}
		//if invalid command return -2
		else
		{
			puts("Invalid Command");
			return -2;
		}
	}
	//if invalid command return -2
	else
	{
		puts("Invalid Command");
		return -2;
	}
	
	
}


void *listenpipe1()
{
    pthread_mutex_lock(&lock1);//locking
    int fd;
    
    //if client id is 1 this pipe is listened:
    char * myfifo = "/tmp/file_manager_named_pipe1";
 
    mkfifo(myfifo, 0666);
 
    char text[80];
    char *Args[20];
    char response[80];
    while (1)
    {
	
        fd = open(myfifo, O_RDONLY);//named pipe is opened
        read(fd, text, sizeof(text));//blocking
        
        //Args is taken word by word
        str_sep(text,Args);
        
        //execute command function is called and 
        //the result is taken to decide the response
        
        int result= execute_commands(Args);
        if(result==-3||result==-4)
        {
        	strcpy(response,"Exitting");
        }
        else if(result==-2)
        {
        	strcpy(response,"Inavlid command");
        }
        else if(result==-1)
        {
        	strcpy(response,"File not found");
        }
        else if(result==0)
        {
        	strcpy(response,"File is already created");
        }
        else if(result==1)
        {
        	strcpy(response,"File is created");
        }
        else if(result==2)
        {
        	strcpy(response,"Cannot create larger than 10 file");
        }
        else if(result==3)
        {
        	strcpy(response,"File is read");
        }
        else if(result==4)
        {
        	strcpy(response,"Written to file");
        }
        else if(result==5)
        {
        	strcpy(response,"File is deleted");
        }
        
        //named pipe is opened
        fd = open(myfifo, O_WRONLY);
        //response is written
        write(fd, response, sizeof(response));
        close(fd);
        
        //if result is -4 which means there is no more client finish the program
        if(result==-4)
        {
        	pthread_mutex_unlock(&lock1);
        	exit(0);
        }
    }
   pthread_mutex_unlock(&lock1);//unlocking
   return NULL;
}
void *listenpipe2()
{
    pthread_mutex_lock(&lock2);
    int fd;
    
    //if client id is 2 this pipe is listened:
    char * myfifo = "/tmp/file_manager_named_pipe2";
 
    mkfifo(myfifo, 0666);
 
    char text[80];
    char *Args[20];
    char response[80];
    while (1)
    {
	
        fd = open(myfifo, O_RDONLY);//named pipe is opened
        read(fd, text, sizeof(text));//blocking
        
        //Args is taken word by word
        str_sep(text,Args);
        
        //execute command function is called and 
        //the result is taken to decide the response
        
        int result= execute_commands(Args);
        if(result==-3||result==-4)
        {
        	strcpy(response,"Exitting");
        }
        else if(result==-2)
        {
        	strcpy(response,"Inavlid command");
        }
        else if(result==-1)
        {
        	strcpy(response,"File not found");
        }
        else if(result==0)
        {
        	strcpy(response,"File is already created");
        }
        else if(result==1)
        {
        	strcpy(response,"File is created");
        }
        else if(result==2)
        {
        	strcpy(response,"Cannot create larger than 10 file");
        }
        else if(result==3)
        {
        	strcpy(response,"File is read");
        }
        else if(result==4)
        {
        	strcpy(response,"Written to file");
        }
        else if(result==5)
        {
        	strcpy(response,"File is deleted");
        }
        
        //named pipe is opened
        fd = open(myfifo, O_WRONLY);
        //response is written
        write(fd, response, sizeof(response));
        close(fd);
        
        //if result is -4 which means there is no more client finish the program
        if(result==-4)
        {
        	pthread_mutex_unlock(&lock2);
        	exit(0);
        }
    }
   pthread_mutex_unlock(&lock2);//unlocking
   return NULL;
}
void *listenpipe3()
{
    pthread_mutex_lock(&lock3);
    int fd;
    
    //if client id is 3 this pipe is listened:
    char * myfifo = "/tmp/file_manager_named_pipe3";
 
    mkfifo(myfifo, 0666);
 
    char text[80];
    char *Args[20];
    char response[80];
    while (1)
    {
	
        fd = open(myfifo, O_RDONLY);//named pipe is opened
        read(fd, text, sizeof(text));//blocking
        
        //Args is taken word by word
        str_sep(text,Args);
        
        //execute command function is called and 
        //the result is taken to decide the response
        
        int result= execute_commands(Args);
        if(result==-3||result==-4)
        {
        	strcpy(response,"Exitting");
        }
        else if(result==-2)
        {
        	strcpy(response,"Inavlid command");
        }
        else if(result==-1)
        {
        	strcpy(response,"File not found");
        }
        else if(result==0)
        {
        	strcpy(response,"File is already created");
        }
        else if(result==1)
        {
        	strcpy(response,"File is created");
        }
        else if(result==2)
        {
        	strcpy(response,"Cannot create larger than 10 file");
        }
        else if(result==3)
        {
        	strcpy(response,"File is read");
        }
        else if(result==4)
        {
        	strcpy(response,"Written to file");
        }
        else if(result==5)
        {
        	strcpy(response,"File is deleted");
        }
        
        //named pipe is opened
        fd = open(myfifo, O_WRONLY);
        //response is written
        write(fd, response, sizeof(response));
        close(fd);
        
        //if result is -4 which means there is no more client finish the program
        if(result==-4)
        {
        	pthread_mutex_unlock(&lock3);
        	exit(0);
        }
    }
   pthread_mutex_unlock(&lock3);//unlocking
   return NULL;
}
void *listenpipe4()
{
    pthread_mutex_lock(&lock4);
    int fd;
    
    //if client id is 4 this pipe is listened:
    char * myfifo = "/tmp/file_manager_named_pipe4";
 
    mkfifo(myfifo, 0666);
 
    char text[80];
    char *Args[20];
    char response[80];
    while (1)
    {
	
        fd = open(myfifo, O_RDONLY);//named pipe is opened
        read(fd, text, sizeof(text));//blocking
        
        //Args is taken word by word
        str_sep(text,Args);
        
        //execute command function is called and 
        //the result is taken to decide the response
        
        int result= execute_commands(Args);
        if(result==-3||result==-4)
        {
        	strcpy(response,"Exitting");
        }
        else if(result==-2)
        {
        	strcpy(response,"Inavlid command");
        }
        else if(result==-1)
        {
        	strcpy(response,"File not found");
        }
        else if(result==0)
        {
        	strcpy(response,"File is already created");
        }
        else if(result==1)
        {
        	strcpy(response,"File is created");
        }
        else if(result==2)
        {
        	strcpy(response,"Cannot create larger than 10 file");
        }
        else if(result==3)
        {
        	strcpy(response,"File is read");
        }
        else if(result==4)
        {
        	strcpy(response,"Written to file");
        }
        else if(result==5)
        {
        	strcpy(response,"File is deleted");
        }
        
        //named pipe is opened
        fd = open(myfifo, O_WRONLY);
        //response is written
        write(fd, response, sizeof(response));
        close(fd);
        
        //if result is -4 which means there is no more client finish the program
        if(result==-4)
        {
        	pthread_mutex_unlock(&lock4);
        	exit(0);
        }
    }
   pthread_mutex_unlock(&lock4);//unlocking
   return NULL;
}
void *listenpipe5()
{
    pthread_mutex_lock(&lock5);
    int fd;
    
    //if client id is 5 this pipe is listened:
    char * myfifo = "/tmp/file_manager_named_pipe5";
 
    mkfifo(myfifo, 0666);
 
    char text[80];
    char *Args[20];
    char response[80];
    while (1)
    {
	
        fd = open(myfifo, O_RDONLY);//named pipe is opened
        read(fd, text, sizeof(text));//blocking
        
        //Args is taken word by word
        str_sep(text,Args);
        
        //execute command function is called and 
        //the result is taken to decide the response
        
        int result= execute_commands(Args);
        if(result==-3||result==-4)
        {
        	strcpy(response,"Exitting");
        }
        else if(result==-2)
        {
        	strcpy(response,"Inavlid command");
        }
        else if(result==-1)
        {
        	strcpy(response,"File not found");
        }
        else if(result==0)
        {
        	strcpy(response,"File is already created");
        }
        else if(result==1)
        {
        	strcpy(response,"File is created");
        }
        else if(result==2)
        {
        	strcpy(response,"Cannot create larger than 10 file");
        }
        else if(result==3)
        {
        	strcpy(response,"File is read");
        }
        else if(result==4)
        {
        	strcpy(response,"Written to file");
        }
        else if(result==5)
        {
        	strcpy(response,"File is deleted");
        }
        
        //named pipe is opened
        fd = open(myfifo, O_WRONLY);
        //response is written
        write(fd, response, sizeof(response));
        close(fd);
        
        //if result is -4 which means there is no more client finish the program
        if(result==-4)
        {
        	pthread_mutex_unlock(&lock5);
        	exit(0);
        }
    }
   pthread_mutex_unlock(&lock5);//unlocking
   return NULL;
}

//this is general pipe all clients communicate with this pipe first
//this pipe gives them an id
void * listenpipe()
{

    
    char inputString[80], *Args[20];
    char response1[2]="1";
    char response2[2]="2";
    char response3[2]="3";
    char response4[2]="4";
    char response5[2]="5";

    int fd;

    char * myfifo = "/tmp/file_manager_named_pipe";
    void * status;
    
    mkfifo(myfifo, 0666);
    
    pthread_t tcb1;
    pthread_t tcb2;
    pthread_t tcb3;
    pthread_t tcb4;
    pthread_t tcb5;

    while(1)
    {
    	    pthread_mutex_lock(&lock); //locking
	    fd = open(myfifo, O_RDONLY);//open pipe
	    read(fd, inputString, sizeof(inputString));//the data is read
	    close(fd);//close pipe
	    
	    //if client is executed client number is increased
	    if(strcmp(inputString,"client")==0)
	    {
		client_num++;
		printf("client acildi number:%d\n",client_num);
				
     	    }
     	    fd = open(myfifo, O_WRONLY);//open pipe
     	    
     	    //if client number is 1 then listenpipe1 is executed with thread
     	    if(client_num==1)
     	    {   
     	    	    //printf("response num respone%s\n",response1);
     	    	    //id1 is given to client
		    write(fd, response1, sizeof(response1));
		    close(fd);
		    pthread_mutex_unlock(&lock);//unlocking
		    pthread_create(&tcb1, NULL, listenpipe1, NULL); 
		    
		    
	    }
	    //if client number is 2 then listenpipe2 is executed with thread
	    else if(client_num==2)
     	    {   
     	    	    //printf("response num respone%s\n",response2);
     	    	    //id2 is given to client
	    	    write(fd, response2, sizeof(response2));
		    close(fd);
		    pthread_mutex_unlock(&lock);//unlocking
		    pthread_create(&tcb2, NULL, listenpipe2, NULL); 
		    
	    }
	    //if client number is 3 then listenpipe3 is executed with thread
	    else if(client_num==3)
     	    {   
     	    	    //printf("response num respone%s\n",response3);
     	    	    //id3 is given to client
		    write(fd, response3, sizeof(response3));
		    close(fd);
		    pthread_mutex_unlock(&lock);
		    pthread_create(&tcb3, NULL, listenpipe3, NULL); 
		    
	    }
	    //if client number is 4 then listenpipe4 is executed with thread
	    else if(client_num==4)
     	    {   
     	    	    //printf("response num respone%s\n",response4);
     	    	    //id4 is given to client
		    write(fd, response4, sizeof(response4));
		    close(fd);
		    pthread_mutex_unlock(&lock);//unlocking
		    pthread_create(&tcb4, NULL, listenpipe4, NULL); 
		    
	    }
	    //if client number is 5 then listenpipe5 is executed with thread
	    else if(client_num==5)
     	    {   
     	    	    //printf("response num respone%s\n",response5);
     	    	    //id5 is given to client
		    write(fd, response5, sizeof(response5));
		    close(fd);
		    pthread_mutex_unlock(&lock);//unlocking
		    pthread_create(&tcb5, NULL, listenpipe5, NULL); 
		    
	    }
    }
}
int main(int argc, char*argv[])
{
    
    
    pthread_t tcb;

    
    
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
	printf("\nmutex init hatasi\n"); 
    }
    
    if (pthread_mutex_init(&lock1, NULL) != 0)
    {
	printf("\nmutex init hatasi\n"); 
    }
    if (pthread_mutex_init(&lock2, NULL) != 0)
    {
	printf("\nmutex init hatasi\n"); 
    }
    if (pthread_mutex_init(&lock3, NULL) != 0)
    {
	printf("\nmutex init hatasi\n"); 
    }
    if (pthread_mutex_init(&lock4, NULL) != 0)
    {
	printf("\nmutex init hatasi\n"); 
    }
    if (pthread_mutex_init(&lock5, NULL) != 0)
    {
	printf("\nmutex init hatasi\n"); 
    }
    
    // a thread is created to execute listenpipe
    pthread_create(&tcb, NULL, listenpipe, NULL); 
    //waits to finish listenpipe
    pthread_join(tcb, NULL);
	    
		    

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_mutex_destroy(&lock3);
    pthread_mutex_destroy(&lock4);
    pthread_mutex_destroy(&lock5);
    return 0;
}
