#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 
#include <netdb.h>
#include <iostream>

#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <errno.h>

void doListen(int sockfd, int * isConnected);
void doWrite(int sockfd, int * isConnected);

//int * isConnected=(int *)malloc(sizeof(int));


int main(int argc, char *argv[])
{
	
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

	//set of socket descriptors
	fd_set readfds;
	int activity,valread;

    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 0;
    }
    portno = atoi(argv[2]);
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        return 1;
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
		return 0;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
           (char *)&serv_addr.sin_addr.s_addr,
                server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) 
    {
         perror("ERROR connecting");
         return 1;
    }	

	while(1){

		//clear the socket set
		FD_ZERO(&readfds);
		
		//add server socket to set
		FD_SET(sockfd, &readfds);

		//add stdin to set
		FD_SET(STDIN_FILENO, &readfds);

		int max_sd= STDIN_FILENO > sockfd ? STDIN_FILENO : sockfd;
		//printf("STDIN_FILENO : %d , sockfd : %d , max_sd : %d ",STDIN_FILENO,sockfd,max_sd);

		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

		if ((activity < 0) && (errno!=EINTR)) 
		{
			printf("select error");
		}

		if (FD_ISSET(sockfd, &readfds)) {
			printf("Incoming message: ");

			if ((valread = read( sockfd , buffer, 255)) == 0)
			{
				printf("Server Disconnected");
				return 0;
			}
			else{
				//std::string message(buffer);
				//int n=message.find_first_of("\n");
				//printf("s: %s\n",message.substr(0,n));
				std::cout<<buffer;
			}

		}

		else if(FD_ISSET(STDIN_FILENO, &readfds)){
			//printf("Console input: ");
			
			if ((valread = read( STDIN_FILENO , buffer, 255)) == 0)
			{
				printf("O.o");
				return 0;
			}
			else{
				//printf("c: %s\n",buffer);

				std::string message(buffer);

				int n;
				n = write(sockfd,buffer,message.find_first_of("\n"));

				if (n < 0) 
				{
					perror("ERROR writing to socket");
					
					return 0;
				}

			}

		}
		else{
			printf("Error");
		}



	}


	//*isConnected=1;
    /* Now ask for a message from the user, this message
    * will be read by server
    */
    //printf("Please enter the message: ");
    //bzero(buffer,256);
    //fgets(buffer,255,stdin);
    ///* Send message to the server */
    //n = write(sockfd,buffer,strlen(buffer));
    //if (n < 0) 
    //{
    //     perror("ERROR writing to socket");
    //     return 1;
    //}
    /* Now read server response */
    /*bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
    {
         perror("ERROR reading from socket");
         return 1;
    }
    printf("%s\n",buffer);*/

	//pid_t pID = fork();

	////if child do read
	//if (pID == 0){
	//	doListen(sockfd, isConnected);
	//}
	////if parent do write
	//else if(pID>0){
	//	doWrite(sockfd, isConnected);
	//}
	//else{
	//	printf("ERROR forking!.");
	//}

	//free(isConnected);

    return 0;
}
//
//void doListen(int sockfd, int * isConnected){
//	//printf("Child");
//	std::cout<<isConnected;
//	char buffer[256];
//	int n;
//	while(*isConnected){
//		
//		bzero(buffer,256);
//		
//
//		n = recv(sockfd,buffer,255,0);
//		if (n <= 0) 
//		{
//			perror("ERROR reading from socket");
//			*isConnected=0;
//			std::cout<<isConnected;
//			return;
//		}
//		printf("\nReceived: %s\n",buffer);
//
//	}
//	
//
//}
//
//void doWrite(int sockfd, int * isConnected){
//	char buffer[256];
//	int n;
//	std::cout<<isConnected;
//
//
//
//	while(*isConnected){
//		//std::cout<<*isConnected;
//		printf("\nPlease enter the message: ");
//		bzero(buffer,256);
//		fgets(buffer,255,stdin);
//		/* Send message to the server */
//		
//		n = write(sockfd,buffer,strlen(buffer));
//		
//		
//		//std::cout<<"client - parent :" << n<<std::endl;
//		if (n <= 0) 
//		{
//			perror("ERROR writing to socket");
//			*isConnected=0;
//			return;
//		}
//
//		std::cout<<"Writer isConnected: "<<*isConnected<<std::endl;
//
//	}
//
//	
//	
//
//}