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

using namespace std;

void error(string msg)
{
	cout << msg << endl;
	exit(EXIT_FAILURE);
	
}

int main(int argc, char *argv[])
{
	
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent * server;

	char buffer[256];

	if(argc < 3)
	{
		fprintf(stderr, "usage %s hostname port",
			argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0)
	{
		error("ERROR opening socket");
	}

	server = gethostbyname(argv[1]);
	
	if(server == NULL)
	{
		fprintf(stderr, "ERROR, no such host");

	}
	struct hostent *gethostbyname(char *name);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);

	if(connect(sockfd,( struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("ERROR connecting");
	}
	
	cout << "Welcome to Number Guessing Game" << endl;	

	printf("Enter your name: ");

	bzero(buffer, 256);
	
	bool con = true;

	int count = 1;

	while(con == true){

	string getLine;
	
	getline(cin, getLine);
	bool invalidNameSize = true;
	bool fail = false;
	bool check = true;
	bool innerCheck = true;
	int success[4]{0,0,0,0};
	if(count == 1)
	{	
		while(invalidNameSize == true){
		if(getLine.length() > 100)
		{
			cout << "NAME CANNOT EXCEED 100 CHARACTERS" << endl;
			getline(cin, getLine);
		}else{
			invalidNameSize = false;
		}

		}
	}
	if(count > 1)
	{
		while(check == true){
			while(innerCheck == true){
			if(getLine.length() > 4 || getLine.length() < 4)
			{
				cout << "ENTER 4 DIGITS WITHOUT SPACES" << endl;
				getline(cin, getLine);
			}
				
			else{
			 	innerCheck = false;
			}
			}
			for(int i = 0; i < 4; i++)
			{
				int a = getLine[i] - '0';
				for(int j = 0; j < 10; j++){
					if(a == j)
						success[i] = 1;
				}
			}
			for(int i = 0; i< 4; i++)
			{
				if(success[i] == 0)
				{
				fail = true;
				}
			}
			if(fail == true)
			{
				cout << "ENTER 4 DIGITS WITHOUT SPACES" << endl;
				getline(cin, getLine);
				innerCheck = false;
				fail = false;
			}else{
				check = false;
			}
	
		}
	}




	getLine.copy(buffer, 100);	

	n = write(sockfd,buffer,strlen(buffer));

	if(n<0)
	{
		error("ERROR writing to socket");
	}

	bzero(buffer,256);
	
	n = read(sockfd, buffer, 255);

	if(n < 0)
	{
		error("ERROR reading from socket");
	}

	cout << buffer << endl;

	bzero(buffer, 256);
	
      	count++;	
	}
	return 0;
}
