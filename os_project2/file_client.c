#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char*argv[])
{
    int fd_general;
    int satir=0;
    char * myfifo = "/tmp/file_manager_named_pipe";
    char * fifo1 = "/tmp/file_manager_named_pipe1";
    char * fifo2 = "/tmp/file_manager_named_pipe2";
    char * fifo3 = "/tmp/file_manager_named_pipe3";
    char * fifo4 = "/tmp/file_manager_named_pipe4";
    char * fifo5 = "/tmp/file_manager_named_pipe5";

    mkfifo(myfifo, 0666);
    
	
	
    char start[7]="client";
    fd_general = open(myfifo, O_WRONLY);//open general named pipe
    write(fd_general, start, sizeof(start));// write client to general named pipe to specify the file is run 
    close(fd_general);//close general named pipe
    char id[2];
    fd_general = open(myfifo, O_RDONLY);//open general named pipe
    read(fd_general, id, sizeof(id));//take id from general named pipe
    printf("id:%s\n",id);
    close(fd_general);//close named pipe
    
    //control id and decide the true named pipe 
    //communicate the manager through the specific named pipe
    if(!strcmp(id,"1"))
    {
	int fd;
	int get=1;
	while (get)
	{
	    char arr1[80];
	    char response[80];
	    fd = open(fifo1, O_WRONLY);//open named pipe1
	    fgets(arr1, 80, stdin);//gets command from user
	    write(fd, arr1, strlen(arr1)+1);//send is to manager
	    close(fd);//close named pipe1
	    
	    //if the taken command is exit break the loop
	    //finish the program
	    if(strcmp(arr1,"exit\n")==0){
	    	get=0;
	    }
	    
	    fd = open(fifo1, O_RDONLY);//open named pipe1
	    read(fd, response, sizeof(response));//take response from manager
	    close(fd);//close named pipe1
	    printf("%s\n", response);//print the response
	}
    }
    else if(!strcmp(id,"2"))
    {
	int fd;
	int get=1;
	while (get)
	{
	    char arr1[80];
	    char response[80];
	    fd = open(fifo2, O_WRONLY);//open named pipe2
	    fgets(arr1, 80, stdin);//gets command from user
	    write(fd, arr1, strlen(arr1)+1);//send is to manager
	    close(fd);//close named pipe2
	    
	    //if the taken command is exit break the loop
	    //finish the program
	    if(strcmp(arr1,"exit\n")==0){
	    	get=0;
	    }
	    
	    fd = open(fifo2, O_RDONLY);//open named pipe2
	    read(fd, response, sizeof(response));//take response from manager
	    close(fd);//close named pipe2
	    printf("%s\n", response);//print the response
	}
    }
    else if(!strcmp(id,"3"))
    {
	int fd;
	int get=1;
	while (get)
	{
	    char arr1[80];
	    char response[80];
	    fd = open(fifo3, O_WRONLY);//open named pipe3
	    fgets(arr1, 80, stdin);//gets command from user
	    write(fd, arr1, strlen(arr1)+1);//send is to manager
	    close(fd);//close named pipe3
	    
	    //if the taken command is exit break the loop
	    //finish the program
	    if(strcmp(arr1,"exit\n")==0){
	    	get=0;
	    }
	    
	    fd = open(fifo3, O_RDONLY);//open named pipe3
	    read(fd, response, sizeof(response));//take response from manager
	    close(fd);//close named pipe3
	    printf("%s\n", response);//print the response
	}
    }
    else if(!strcmp(id,"4"))
    {
	int fd;
	int get=1;
	while (get)
	{
	    char arr1[80];
	    char response[80];
	    fd = open(fifo4, O_WRONLY);//open named pipe4
	    fgets(arr1, 80, stdin);//gets command from user
	    write(fd, arr1, strlen(arr1)+1);//send is to manager
	    close(fd);//close named pipe4
	    
	    //if the taken command is exit break the loop
	    //finish the program
	    if(strcmp(arr1,"exit\n")==0){
	    	get=0;
	    }
	    
	    fd = open(fifo4, O_RDONLY);//open named pipe4
	    read(fd, response, sizeof(response));//take response from manager
	    close(fd);//close named pipe4
	    printf("%s\n", response);//print the response
	}
    }
    else if(!strcmp(id,"5"))
    {
	int fd;
	int get=1;
	while (get)
	{
	    char arr1[80];
	    char response[80];
	    fd = open(fifo5, O_WRONLY);//open named pipe5
	    fgets(arr1, 80, stdin);//gets command from user
	    write(fd, arr1, strlen(arr1)+1);//send is to manager
	    close(fd);//close named pipe5
	    
	    //if the taken command is exit break the loop
	    //finish the program
	    if(strcmp(arr1,"exit\n")==0){
	    	get=0;
	    }
	    
	    fd = open(fifo5, O_RDONLY);//open named pipe5
	    read(fd, response, sizeof(response));//take response from manager
	    close(fd);//close named pipe5
	    printf("%s\n", response);//print the response
	}
    }
    return 0;
}
