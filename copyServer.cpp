#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <vector>

void doWork();

using namespace std;

int main( int argc, char *argv[] )
{
	std::vector<int*>* clients = new vector<int*>(10);


    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        return 1;
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5002;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
 
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         return 1;
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

	//loop begin

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, 
                                (socklen_t*)&clilen);
    if (newsockfd < 0) 
    {
        perror("ERROR on accept");
        return 1;
    }
    /* If connection is established then start communicating */
    
	while (1)
	{
		bzero(buffer,256);
		n = read( newsockfd,buffer,255 );
		if (n < 0)
		{
			perror("ERROR reading from socket");
			return 1;
		}
		printf("Here is the message: %s\n",buffer);

		/* Write a response to the client */
		n = write(newsockfd,buffer,255);
		if (n < 0)
		{
			perror("ERROR writing to socket");
			return 1;
		}
	}
    
	



	close(newsockfd);

	//loop end

    return 0; 
}

void doWork(){

};