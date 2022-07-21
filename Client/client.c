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

	int clientSocket;
	int ret;
	int port;

	char buffer[1024];
	char ip_address[20];

	struct sockaddr_in serverAddr;
	
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	
	printf("Enter IP address of server:");
	scanf("%[^\n]", ip_address);

	printf("Enter port of server:");
	scanf("%d", &port);
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(ip_address);
	
	printf("[+]Client Socket is created.\n");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	if(ret < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1)
	{
		printf("Client:");
		scanf("%s",&buffer[0]);
		send(clientSocket, buffer, 1024, 0);
	
		if(strcmp(buffer, "disconnect") == 0)
		{
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
		else if(strcmp(buffer, "get_time") == 0)
		{
			time_t t = time(NULL);
  			struct tm tm = *localtime(&t);
  			printf("Date and time: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		}
	}

	return 0;
}
