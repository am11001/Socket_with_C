#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int main()
{

	int sockid;
	int ret;
	int newSocket;
	const int port = 4444;

	struct sockaddr_in serverAddr;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockid = socket(AF_INET, SOCK_STREAM, 0);

	if(sockid < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}

	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockid, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	if(ret < 0)
	{
		printf("[-]Error in binding.\n");
		exit(1);
	}

	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockid, 5) == 0)
	{
		printf("[+]Listening...\n");
	}
	else
	{
		printf("[-]Error in binding.\n");
	}

	while(1)
	{
		newSocket = accept(sockid, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0)
		{
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0)
		{
			close(sockid);

			while(1)
			{
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, "disconnect") == 0)
                                {	
                                        printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                                        break;
                                }
				else if(strcmp(buffer, "get_time") == 0)
				{
					time_t t = time(NULL);
  					struct tm tm = *localtime(&t);
  					printf("Date and time: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				}
			}
		}

	}

	close(newSocket);

	return 0;
}
