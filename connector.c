int gameStart(int player1,int player2){

	int sent;
	int received;
	int setted1;
	int setted2;
	char buffer[100];

	puts("[ + ] Everyone is ready!\n");
	
	puts("[ + ] Link Start!\n");
	
	puts("[ ? ] Waiting for the players to setup their table!");		
	usleep(2000);
	
	sendString(player1,"[ ! ] You are going first!");
	sendString(player2,"[ ! ] You are going second!");


	received = recv(player1,&setted1,100,0);
	checkRecv(received);
	usleep(2000);
	
	received = recv(player2,&setted2,100,0);
	checkRecv(received);	

	//received = recv(player1,&setted,1,0);
	//checkRecv(received);
	
	if(setted1 != 0 && setted2 != 0){
		puts("[ + ] Players setted up their table");
	}else{
		puts("[ - ] Error occured while setting the tables");
		exit(1);
	}

	int winner;
	sendString(player1,"1");
	sendString(player2,"0");

	while(1){
		
		received = recv(player2,&buffer,100,0);
		checkRecv(received);

		memset(&buffer,0,sizeof(buffer));

		received = recv(player1,&buffer,100,0);
		checkRecv(received);
		
		if(strcmp(buffer,"end") == 0){
			sendString(player2,"end");
			winner = 2;
			break;
		}

		sent = send(player2,&buffer,100,0);
		checkSent(sent);

		received = recv(player2,&buffer,100,0);
		checkRecv(received);
		if(strcmp(buffer,"end") == 0){
			sent = send(player1,buffer,100,0);
			checkSent(sent);
			winner = 1;
			break;
		}

		sent = send(player1,&buffer,100,0);
		checkSent(sent);

//------------------------------------------------------------------------------------

		received = recv(player1,&buffer,100,0);
		checkRecv(received);

		memset(&buffer,0,sizeof(buffer));

		received = recv(player2,&buffer,100,0);
		checkRecv(received);
		
		if(strcmp(buffer,"end") == 0){
			sendString(player1,"end");
			winner = 1;
			break;
		}

		sent = send(player1,&buffer,100,0);
		checkSent(sent);

		received = recv(player1,&buffer,100,0);
		checkRecv(received);
		
		if(strcmp(buffer,"end") == 0){
			sent = send(player2,buffer,100,0);
			checkSent(sent);
			winner = 2;
			break;
		}

		sent = send(player2,&buffer,100,0);
		checkSent(sent);

	}
	int answer1;
	int answer2;
	if(winner == 1){
		sendString(player1,"Congratulations! You are the winner!");
		sendString(player2,"Sorry, You lost!");

		received = recv(player1,&buffer,101,0);
		checkRecv(received);
	
		if(strcmp(buffer,"y") == 0){
			answer1 = 1;
		}
		
		received = recv(player2,&buffer,101,0);
		checkRecv(received);
	
		if(strcmp(buffer,"y") == 0){
			answer2 = 1;
		}

		if(answer1 == 1 && answer2 == 1){
			sendString(player1,"restart");
			sendString(player2,"restart");
			gameStart(player2,player1);
		}
		
	}else{
		sendString(player2,"Congratulations! You are the winner!");
		sendString(player1,"Sorry, You lost!");

		received = recv(player1,&buffer,101,0);
		checkRecv(received);
	
		if(strcmp(buffer,"y") == 0){
			answer1 = 1;
		}
		
		received = recv(player2,&buffer,101,0);
		checkRecv(received);
	
		if(strcmp(buffer,"y") == 0){
			answer2 = 1;
		}

		if(answer1 == 1 && answer2 == 1){
			sendString(player1,"restart");
			sendString(player2,"restart");
			gameStart(player2,player1);
		}
	}
}

void checkSent(int s){
	if(s < 0){
		puts("[ - ] Error while sending the message!");
		exit(0);
	}else{
		puts("[ + ] Data successfully sent!");
	}
}

void checkRecv(int r){
	if(r < 0){
		puts("[ - ] Error while receiving the message!");
		exit(0);
	}else{
		puts("[ + ] Data successfully received!");
	}
}

void fillBoard(int board[10][10]){
	puts("[ ! ] Row and Column numbers between 0 and 9");
	puts("[ ! ] You have 5 ships.Give values like: 'rowNumber columnNumber'");
	for(int i = 0; i < 10; ++i){
		for(int j = 0; j < 10; ++j){
			board[i][j] = 0;
		}
	}

	for(int i = 0; i < 5; ++i){
		char row;
		char col;

		printf("%d. ship: ",i+1);
		scanf("%s %s",&row,&col);
		if((int)row-48 < 0 || (int)row-48 > 9 || (int)col-48 < 0 || (int)col-48 > 9){
			puts("Enter a number between 0 and 9 please!");			
			--i;
		}else if(board[(int)row-48][(int)col-48] == 1){
			puts("Already have ship on this location!");
			--i;
		}else{
			board[(int)row-48][(int)col-48] = 1;
		}	
	}

	puts("[ + ] Your game board: ");
	for(int i = 0; i < 10; ++i){
		for(int j = 0; j < 10; ++j){
			printf("%d ",board[i][j]);
		}
		printf("\n");
	}
}

int sendString(int Stream, char* ToSend) {
	int Sent = send(Stream, ToSend, strlen(ToSend) + 1, 0);
	if (Sent < 0) {
		printf("Cannot send data.\n");
	}
}

int checkHit(int board[10][10],int posA,int posB){
	if(board[posA][posB] == 1){
		board[posA][posB] = 0;
		return 1;
	}else{
		return 0;
	}
}
