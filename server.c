#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "connector.c" 

#define BUFSIZE 1024

int main(int argc,char *argv[]){

	int serverfd;
	struct sockaddr_in serverAddr;
	
	int player1fd;
	struct sockaddr_in player1Addr;

	int player2fd;
	struct sockaddr_in player2Addr;

	socklen_t serverSize;
	char buffer[BUFSIZE] = {};
	char on = 1;

	serverfd = socket(AF_INET,SOCK_STREAM,0);
	if(serverfd < 0){
		puts("[ - ] Socket creation failed\n");
		exit(1);	
	}else{
		puts("[ + ] Socket created successfully\n");
	}
	
	setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
	memset(&serverAddr,'0',sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(1313);

	int binded = bind(serverfd,(struct sockaddr*) &serverAddr,sizeof(serverAddr));
	if(binded < 0){
		puts("[ - ] Server binding failed!\n");
		exit(2);
	}else{
		puts("[ + ] Server successfully binded!\n");
	}

	int listening = listen(serverfd,2);
	if(listening < 0){
		puts("[ - ] Server listening failed!\n");
		exit(3);
	}else{
		puts("[ + ] Server is listening!\n");
	}		
	
	puts("Waiting for the first player...");
	usleep(2000);
	
	player1fd = accept(serverfd,(struct sockaddr*) &serverAddr,&serverSize);
	if(player1fd < 0){
		puts("[ - ] Player1 connection error!");
	}else{
		puts("[ + ] Player1 connected succesfully");
	}

	puts("Waiting for the second player...");
	usleep(2000);

	player2fd = accept(serverfd,(struct sockaddr*) &serverAddr,&serverSize);
	if(player2fd < 0){
		puts("[ - ] Player2 connection error!");
	}else{
		puts("[ + ] Player2 connected succesfully");
	}

	gameStart(player1fd,player2fd);

	puts("Game Over!Closing streams!");

	close(serverfd);
	close(player1fd);
	close(player2fd);
}

