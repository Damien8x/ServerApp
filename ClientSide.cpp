#include <string.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <syscall.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h> //defines structure hostent

using namespace std;

void error(string msg)
{
	cout << msg << endl;
	
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

	printf("Please enter the message: ");
	
	bzero(buffer, 256);
	
	fgets(buffer,255, stdin);
	
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

	printf("%s", buffer);
	
	return 0;
}
