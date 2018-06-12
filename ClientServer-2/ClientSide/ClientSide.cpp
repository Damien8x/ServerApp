//Damien Sudol
//HW5
//Version 1.0

#include <string.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <syscall.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h> //defines structure hostent
#include <stdlib.h>
#include <sys/ioctl.h>

using namespace std;

//message for error handling and exiting program
void error(string msg)
{
	cout << msg << endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	//main scope variables
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent * server;
	char buffer[512];
	bool con = true;
	int count = 1;

	// ensure the proper number of arguments has been passed
	if (argc < 3)
	{
		fprintf(stderr, "usage %s hostname port",
			argv[0]);
		exit(0);
	}
	
	// assign port number. open socket
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}

	//find host by ip argument
	server = gethostbyname(argv[1]);

	if (server == NULL)
	{
		fprintf(stderr, "ERROR, no such host");

	}
	struct hostent *gethostbyname(char *name);

	//establish connection
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("ERROR connecting");
	}

	//prompt user for name
	cout << "Welcome to Number Guessing Game" << endl;
	printf("Enter your name: ");
	bzero(buffer, 512);

	//loop for server->client game back n forth
	while (con == true) {
		//variable for loop/reset
		string getLine;
		getline(cin, getLine);
		bool invalidNameSize = true;
		bool fail = false;
		bool check = true;
		bool innerCheck = true;
		int success[4]{ 0,0,0,0 };
		int guessResult = -1;
		
		
		//if statement for first send to server
		if (count == 1)
		{
			//check input for acceptable name length
			while (invalidNameSize == true) {
				if (getLine.length() > 100)
				{
					cout << "NAME CANNOT EXCEED 100 CHARACTERS" << endl;
					getline(cin, getLine);
				}
				else {
					invalidNameSize = false;
				}

			}
	
		//write to server if first write (use char buffer)	
		getLine.copy(buffer, 512);
		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
		{
			error("ERROR writing to socket");
		}

		}
		//if statement to conduct the rest of the game
		if (count > 1)
		{
			//check user submits proper data. 4 digits no whitespace.
			while (check == true) {
				while (innerCheck == true) {
					if (getLine.length() > 4)
					{
						cout << "ENTER a number between 0000 - 9999" << endl;
						getline(cin, getLine);
					}

					else {
						innerCheck = false;
					}
				}
				for (int i = 0; i < getLine.length(); i++)
				{
					int a = getLine[i] - '0';
					for (int j = 0; j < 10; j++) {
						if (a == j)
							success[i] = 1;
					}
				}
				for (int i = 0; i < getLine.length(); i++)
				{
					if (success[i] == 0)
					{
						fail = true;
					}
				}
				if (fail == true)
				{
					cout << "ENTER a number between 0000 - 9999" << endl;
					getline(cin, getLine);
					innerCheck = false;
					fail = false;
				}
				else {
					check = false;
				}

			}
			//acceptable data is cast to long int and sent to server
			long hostInt = stol(getLine);
			long networkInt = htonl(hostInt);
			int bytesSent = send(sockfd, (void *) &networkInt,
					sizeof(long), 0);
			if (bytesSent != sizeof(long))
			{
				error("ERROR writing to socket");
			}
	
		}

		//clear buffer, read and print from buffer
		int bytesLeft = sizeof(long);
		long networkInt;
		char *bp = (char*) &networkInt;

		while(bytesLeft){

			int bytesRecv = recv(sockfd, bp, bytesLeft, 0);
			if(bytesRecv <= 0)
			{
				error("ERROR ON BYTES RECEIVED");
			}
	
		bytesLeft = bytesLeft - bytesRecv;
		bp = bp + bytesRecv;

		}

		long hostInt = ntohl(networkInt);
		guessResult = (int)hostInt;
	
		bzero(buffer, 512);

		n = read(sockfd, buffer, 512);
		if (n < 0)
		{
			error("ERROR reading from socket");
		}
		if(guessResult != 0){
		cout << buffer << endl;
		}
		//statement checks for game comopletion. if game is completed exit loop and end program
		if(guessResult == 0){
			cout << buffer << endl;
			con = false;
			
		}

		bzero(buffer, 512);
		count++;
	
	}
	return 0;
}
