all:	mainProg prog1 run

mainProg:
	gcc file_manager.c -o file_manager -l pthread
prog1:
	gcc file_client.c -o file_client
run:
	./file_manager
	
