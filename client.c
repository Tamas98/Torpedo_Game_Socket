#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "connector.c"

int main(int argc,char *argv[]){

	int clientfd;
	struct sockaddr_in clientAddr;
	char on = 1;

	int board[10][10];
	char buffer[100];

	int received;
	int sent;

	clientfd = socket(AF_INET,SOCK_STREAM,0);
	if(clientfd < 0){
		puts("[ - ] Socket creation failed\n");
		exit(1);	
	}else{
		puts("[ + ] Socket created successfully\n");
	}

   	setsockopt(clientfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
	memset(&clientAddr,'0',sizeof(clientAddr));

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddr.sin_port = htons(1313);

	int connection = connect(clientfd,(struct sockaddr*) &clientAddr,sizeof(clientAddr));
	if(connection < 0){
		puts("[ - ] Failed to connect to the server!");
		exit(2);
	}else{
		puts("[ + ] Connected to the server!");
	}

restart:
	received = recv(clientfd,&buffer,101,0);
	checkRecv(received);
	printf("%s\n",buffer);

	puts("[ ! ] Fill your game board!");
	fillBoard(board);
	
	int ready = 1;
	sent = send(clientfd,&ready,100,0);
	checkSent(sent);

	int currentPlayer = 0;
	received = recv(clientfd,&buffer,100,0);
	checkRecv(received);
		
	if(strcmp(buffer,"1") == 0){
		currentPlayer = atoi(buffer);
	}

	int hitCounter = 0;

	while(1){

		if(currentPlayer == 1 && hitCounter < 5){
			char row[10];
			char col[10];
			int correctInput = 1;
			memset(&buffer,0,sizeof(buffer));			
			puts("[ ! ] Give me the coordinates to fire!(rownumber colnumber)");
			
			while(1){
			printf("Coordinates: ");
			scanf("%s %s",&row,&col);

				if(strcmp(row,"surrender") == 0 || strcmp(col,"surrender") == 0 ){
					//sendString(clientfd,"end");
					break;
				}
				else if((int) row[0] > 57 || (int)row[0] < 48 ||(int) col[0] > 57 || (int)col[0] < 48){
					puts("Enter a number between 0 and 9 please");
				}else{
					break;
					}
			}

			if(strcmp(row,"surrender") == 0 || strcmp(col,"surrender") == 0 ){
				sendString(clientfd,"end");
				break;
			}
			
			buffer[0] = row[0];
			buffer[1] = col[0];

			sent = send(clientfd,buffer,100,0);
			//checkSent(sent);

			received = recv(clientfd,&buffer,100,0);
			//checkRecv(received);
			if(strcmp(buffer,"end") == 0)
				break;

			printf("%s\n",buffer);
			currentPlayer = 0;
		}else{
			sendString(clientfd,"OK");
			
			memset(&buffer,0,sizeof(buffer));

			received = recv(clientfd,&buffer,100,0);
			//checkRecv(received);

			if(strcmp(buffer,"end") == 0){
				puts("Your oppenent gives up the game!");
				break;
			}

			int row = buffer[0]-48;
			int col = buffer[1]-48;

			if(checkHit(board,row,col) == 1){
				
				++hitCounter;

				if(hitCounter == 5){
					sendString(clientfd,"end");
					break;
				}else{
					sendString(clientfd,"Your shoot was a hit");
					printf("Your opponent destroyed your ship at: %d %d\n",row,col);
				}
			}else{
				sendString(clientfd,"Your shoot was a missed shoot");
				printf("Your opponent fired to: %d %d,but he missed!\n",row,col);
			}

			currentPlayer = 1;
		}
	}
	received = recv(clientfd,&buffer,100,0);
	//checkRecv(received);
	printf("%s\n",buffer);

	printf("Do you want to play again?(y or n) ");
	scanf("%s",&buffer);
	sent = send(clientfd,buffer,100,0);
	//checkSent(sent);

	received = recv(clientfd,&buffer,100,0);
	//checkRecv(received);

	if(strcmp(buffer,"restart") == 0)
		goto restart;
	else
		puts("Both players doesn't want to play again.GoodBye");
}
