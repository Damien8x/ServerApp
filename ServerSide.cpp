#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <syscall.h>

using namespace std;

void error(string msg)
{
	cout << msg << endl;

}
int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno, n;
	unsigned int clien;

	char buffer[256];

	struct sockaddr_in serv_addr, cli_addr;
	
struct sockaddr_in
{
	short sin_family; /*must be AF_INET*/
	u_short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];/*Not used, must be zero */
};


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
	
	listen(sockfd, 5); /*process listens to socket for connections*/

	clien = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *)
	&cli_addr, &clien);

	if(newsockfd < 0)
	{
		error("ERROR on accept");
	}

	bzero(buffer,256);

	n = read(newsockfd, buffer, 255);

	if(n < 0)
	{
		error("ERROR reading from socket");
	}

	printf("HERE is the message. %s",buffer);

	n = write(newsockfd, "I got your message", 18);

	if(n < 0)
	{
		error("ERROR writing to scoket");
	}

	return 0;

}











