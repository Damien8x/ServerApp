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

using namespace std;

void * threadMain(void *vs);
void processClient(int clientSock);

void error(string msg)
{
	cout << msg << endl;

};


struct ThreadArgs
{
	int clientSock;

};

int highScoreArr[3]{0,0,0};
string nameArr[3]{"","",""};


int main(int argc, char *argv[])
{
	struct sockaddr_in
{
	short sin_family; /*must be AF_INET*/
	u_short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];/*Not used, must be zero */
};

	int sockfd, newsockfd, portno;
	unsigned int clien;

	char buffer[256];

	struct sockaddr_in serv_addr, cli_addr;
	

	if(argc < 2)
	{
		fprintf(stderr, "ERROR, no port provided");
		
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0)
	{
		error("ERROR opening socket");
	}
	
	int socket(int family, int type, int protocol);

	bzero((char *) &serv_addr, sizeof(serv_addr)); /*sets values in buffer to 0*/

	portno = stoi(argv[1]); /*port # server will listen for*/

	serv_addr.sin_family = AF_INET; /*code for addr. family set to constant*/

	serv_addr.sin_port = htons(portno); /*port # to convert to network byte order*/

	serv_addr.sin_addr.s_addr = INADDR_ANY; /*contains ip addr. of host*/

	/*binds a socket to an addr. takes three args; socket file descriptor,
	 * addr. to which bound, size of addr. bound*/
	if(bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0)
	{	
		error("ERROR on binding");
	}
	pthread_t id[200];	
	int count = 0;
	while(true)
	{

	listen(sockfd, 5); /*process listens to socket for connections*/

	clien = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *)
	&cli_addr, &clien);

	if(newsockfd < 0)
	{
		error("ERROR on accept");
	}
	

	struct ThreadArgs * threadArgs;
      	threadArgs = new struct ThreadArgs;

	threadArgs->clientSock = newsockfd;


	count++;
	
	
	int status = pthread_create(&id[count], NULL, threadMain, (void*) threadArgs);
	





}
	return 0;

}

void *threadMain(void * args)
{

	struct ThreadArgs * threadArgs = (struct ThreadArgs *) args;

	int clientSock = threadArgs->clientSock;



	processClient(clientSock);
	delete threadArgs;
	pthread_detach(pthread_self());
	close(clientSock);

	return NULL;
	

}


void processClient(int clientSock)
{
	string name ="";
	int count = 1;
	int n;
	char buffer[256];
	bzero(buffer,256);
	bool open = true;
	string output;
	string guess;
	int guessResult = 0;
	int randomResult = 0;
	int difResult = 0;
	string rando ="";
	
	for (int i = 0; i < 4; i++)
	{
		n = rand() %10;
		randomResult += n;
		rando += to_string(n);
	}
	

	n = read(clientSock, buffer, 255);
	
	if(n < 0)
	{
		error("ERROR reading from socket");
	}


	

	name = buffer;
	while(open){
	

	guessResult = 0;

	if(count == 1){
	output = "Random Number: " + rando + "\nTurn: " + to_string(count) + "\nEnter a guess: ";
	strcpy(buffer, output.c_str());
	n = write(clientSock, buffer, strlen(buffer));

		if(n < 0)
		{
			error("ERROR writing to scoket");
		}
	}

	
	bzero(buffer, 256);

	n = read(clientSock, buffer, 255);

	if(n < 0)
	{
		error("ERROR reading from socket");
	}

	string guess = buffer;
	int guessArr[4];
	for(int i = 0; i < 4; i++)
	{
		char x = guess[i];
		guessArr[i] = x - '0';
       		guessResult += guessArr[i];		
	}

	cout << guessResult << endl;
	cout << randomResult<< endl;


	bzero(buffer, 256);

	if(guessResult > randomResult){
		difResult = guessResult - randomResult;
	}
	else if(guessResult < randomResult){
		difResult = randomResult - guessResult;
	}else{


	
	
	if(highScoreArr[0] > count || highScoreArr[0] == 0)
	{
		highScoreArr[2] = highScoreArr[1];
		nameArr[2] = nameArr[1];
		highScoreArr[1] = highScoreArr[0];
		nameArr[1] = nameArr[0];
		highScoreArr[0] = count;
		nameArr[0] = name;
		 
	}else if(highScoreArr[1] > count || highScoreArr[1] == 0)
	{
		highScoreArr[2] = highScoreArr[1];
		nameArr[2] = nameArr[1];
		highScoreArr[1] = count;
		nameArr[1] = name;
	}else if(highScoreArr[2] > count || highScoreArr[2] == 0)
	{
		highScoreArr[2] = count;
		nameArr[2] = name;
	}
	
	

	
	
	output = "Result of guess: 0\n\nCongratulations! It took " + to_string(count) + "  tries to guess the number!" +
		"\n\nLeader board:\n 1. " + nameArr[0] +" " + to_string(highScoreArr[0]) + "\n 2. " + nameArr[1] +
		" " + to_string(highScoreArr[1]) + "\n 3. " + nameArr[2] + " " + to_string(highScoreArr[2]);
	output.copy(buffer,255 );

	n= write(clientSock, buffer, strlen(buffer));
	if(n < 0)
	{
		error("ERROR writing to socket");
	}



	open = false;
	}
	
	if(open)
	{

	output = "Result of guess: " + to_string(difResult) + "\nTurn: " + to_string(count) + "\nEnter a guess: "; 
	strcpy(buffer, output.c_str());
	n = write(clientSock, buffer, strlen(buffer));
	if(n<0)
	{
		error("ERROR writing to socket");
	}

	count++;
	}	

	
	
	}

}






