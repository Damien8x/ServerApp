//Damien Sudol
//Assignment 5
//Version 1.0

#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <syscall.h>
#include <pthread.h>
#include <vector>
#include <sstream>
#include <cstring>
#include <stdlib.h>

using namespace std;

//Create mutex for shared variables amongst threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//function prototype for thread
void * threadMain(void *vs);

//error handling method
void error(string msg)
{
	cout << msg << endl;
	exit(EXIT_FAILURE);
};

//struct to pass onto thread
struct ThreadArgs
{
	int clientSock;

};

//declare shared global variables
int highScoreArr[3]{ 0,0,0 };
string nameArr[3]{ "","","" };

int main(int argc, char *argv[])
{
	struct sockaddr_in
	{
		short sin_family; /*must be AF_INET*/
		u_short sin_port;
		struct in_addr sin_addr;
		char sin_zero[8];/*Not used, must be zero */
	};
	
	//create a capacity of 900 threads
	//declare variables used in main 
	pthread_t id[900];
	int count = 0;
	int sockfd, newsockfd, portno;
	unsigned int clien;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2)
	{
		fprintf(stderr, "ERROR, no port provided");

	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}

	//create viable connection
	int socket(int family, int type, int protocol);
	bzero((char *)&serv_addr, sizeof(serv_addr)); /*sets values in buffer to 0*/
	portno = stoi(argv[1]); /*port # server will listen for*/
	serv_addr.sin_family = AF_INET; /*code for addr. family set to constant*/
	serv_addr.sin_port = htons(portno); /*port # to convert to network byte order*/
	serv_addr.sin_addr.s_addr = INADDR_ANY; /*contains ip addr. of host*/

	/*binds a socket to an addr. takes three args; socket file descriptor,
	 * addr. to which bound, size of addr. bound*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
	{
		error("ERROR on binding");
	}
	
	//listen for connections on port number sockfd
	while (true)
	{

		listen(sockfd, 5); /*process listens to socket for connections*/
		clien = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *)
			&cli_addr, &clien);

		if (newsockfd < 0)
		{
			error("ERROR on accept");
		}
		
		struct ThreadArgs * threadArgs;
		threadArgs = new struct ThreadArgs;
		threadArgs->clientSock = newsockfd;
		count++;
		
		int status = pthread_create(&id[count], NULL, threadMain, (void*)threadArgs);

		}
	return 0;
}

//thread to treat each client independently 
void *threadMain(void * args)
{

	//delcare all utilized variables for thread
	struct ThreadArgs * threadArgs = (struct ThreadArgs *) args;
	int clientSock = threadArgs->clientSock;
	string name = "";
	int count = 1;
	int n;
	char buffer[256];
	bzero(buffer, 256);
	bool open = true;
	string output;
	string guess;
	int guessResult = 0;
	int randomResult = 0;
	string rando = "";
	int guessArr[4];
	int randomArr[4];

	//create random 4 digit number
	for (int i = 0; i < 4; i++)
	{
		n = rand() % 10;
		randomArr[i] = n;
		rando += to_string(n);
	}

	//print random number for error testing
	cout << "Random Number: " << rando << " Client Socket: " << clientSock << endl;

	//read client name
	n = read(clientSock, buffer, 255);
	if (n < 0)
	{
		error("ERROR reading from socket");
	}
	name = buffer;
	
	//while loop to fascilitate game back and forth, server -> client
	while (open) {
		guessResult = 0;

		//handles user input for name vs integer for guess @count == 0 (first input)
		//writes first prompt to user from serverside to guess number
		if (count == 1) {
			output = "\nTurn: " + to_string(count) + "\nEnter a guess: ";
			strcpy(buffer, output.c_str());
			if (n != 0) {
				n = write(clientSock, buffer, strlen(buffer));
			}
			else {
				open = false;
			}
			if (n < 0)
			{
				error("ERROR writing to scoket");
			}
		}
		
		//clear buffer, handle user input as incoming long integer value
		//assign value to hostInt
		bzero(buffer, 256);
		int bytesLeft = sizeof(long);
		long networkInt;
		char *bp = (char *) &networkInt;

		while(bytesLeft){
			int bytesRecv = recv(clientSock, bp, bytesLeft, 0);
			if (bytesRecv <=0 )
			{
				cout << "Socket " << clientSock << " closed. Error on Bytes Received." << endl;
				 	
				open = false;
				n = 0;
				break;
			}
			bytesLeft = bytesLeft - bytesRecv;
			bp = bp + bytesRecv;
		}

		long hostInt = ntohl(networkInt);

		//cast user input to string and assign to guess
		string guess = to_string(hostInt);
		
		//test if precision has been lost due to leading 0s. Change as necessary for completeness
		if(guess.length() == 3)
			guess = "0" + guess;
		else if(guess.length() == 2)
			guess = "00" + guess;
		else if(guess.length() == 1)
			guess = "000" + guess;

		//find digit to digit difference for random number and user guess
		for (int i = 0; i < 4; i++)
		{
			char x = guess[i];
			guessArr[i] = x - '0';
			if(guessArr[i] > randomArr[i]){
				guessResult += (guessArr[i] - randomArr[i]);
			}else{
				guessResult +=(randomArr[i] - guessArr[i]);
			}
			
		}
		
		//clear buffer
		bzero(buffer, 256);

		//check if user guesses correctly. if yes, enter if statement
		if(guessResult == 0){		
			//update global variables for leaderboard. Use mutex to prevent race conditions	
			pthread_mutex_lock(&mutex);
			if (highScoreArr[0] > count || highScoreArr[0] == 0)
			{
				highScoreArr[2] = highScoreArr[1];
				nameArr[2] = nameArr[1];
				highScoreArr[1] = highScoreArr[0];
				nameArr[1] = nameArr[0];
				highScoreArr[0] = count;
				nameArr[0] = name;

			}
			else if (highScoreArr[1] > count || highScoreArr[1] == 0)
			{
				highScoreArr[2] = highScoreArr[1];
				nameArr[2] = nameArr[1];
				highScoreArr[1] = count;
				nameArr[1] = name;
			}
			else if (highScoreArr[2] > count || highScoreArr[2] == 0)
			{
				highScoreArr[2] = count;
				nameArr[2] = name;
			}
			
			//format data to be sent to client, including leaderboard
			if (highScoreArr[2] != 0) {
				output = "Result of guess: 0\n\nCongratulations! It took " + to_string(count) + "  tries to guess the number!" +
					"\n\n Leader board:\n 1. " + nameArr[0] + " " + to_string(highScoreArr[0]) + "\n 2. " + nameArr[1] +
					" " + to_string(highScoreArr[1]) + "\n 3. " + nameArr[2] + " " + to_string(highScoreArr[2]);
			}
			else if (highScoreArr[2] == 0 && highScoreArr[1] != 0) {
				output = "Result of guess: 0\n\nCongratulations! It took " + to_string(count) + "  tries to guess the number!" +
					"\n\n Leader board:\n 1. " + nameArr[0] + " " + to_string(highScoreArr[0]) + "\n 2. " + nameArr[1] +
					" " + to_string(highScoreArr[1]);
			}
			else {
				output = "Result of guess: 0\n\nCongratulations! It took " + to_string(count) + "  tries to guess the number!" +
					"\n\n Leader board:\n 1. " + nameArr[0] + " " + to_string(highScoreArr[0]);
			}

			pthread_mutex_unlock(&mutex);
			bzero(buffer, 256);
			
			string outputEnd;
			string outputStart;
			if(output.length() < 255){
			//write data to client/ ensure connection is active
			output.copy(buffer, 255);
			if (n != 0) {
				n = write(clientSock, buffer, strlen(buffer));
			}
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			}else{
			
			outputStart = output.substr(0, 240);
			outputEnd = output.substr(241, (output.length() - 241));
			outputStart.copy(buffer, 255);
			if (n != 0) {
				n = write(clientSock, buffer, strlen(buffer));
			}
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
	

			bzero(buffer, 256);
			outputEnd.copy(buffer, 255);

			if (n != 0) {
				n = write(clientSock, buffer, strlen(buffer));
			}
			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			}


			//end loop after correct guess
			open = false;
		}

		//if connection still exists and guess is incorrect
		if (open)
		{
			//add to turn index and display message to user without leaderboard or congrat message. check for connection before
			//write to user
			count++;

			output = "\nResult of guess: " + to_string(guessResult) + "\nTurn: " + to_string(count) + "\nEnter a guess: ";
			strcpy(buffer, output.c_str());
			if (n != 0) {
				n = write(clientSock, buffer, strlen(buffer));
			}
			else {
				open = false;
			}

			if (n < 0)
			{
				error("ERROR writing to socket");
			}
			
		}	
	}
	
	//game has concluded. Reallocate dynamic memory, detach thread, close socket, return null
	delete threadArgs;
	pthread_detach(pthread_self());
	close(clientSock);

	return NULL;
}





