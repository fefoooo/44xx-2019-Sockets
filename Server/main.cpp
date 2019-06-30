#include <iostream>
//#include <WS2tcpip.h>
#include <string.h>
#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
//#pragma comment (lib, "ws2_32.lib")

using namespace std;

#define NI_MAXHOST 1024
#define NI_MAXSERV 1024

#define timeout_in_seconds 1

int main(void)
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	
	cerr << "Initialising..." << endl;

	int wsOk = WSAStartup(ver, &wsData);
	
	cerr << "WINsock ON!" << endl;
	
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return 99;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return 99;
	}
	cerr << "SOCKET created!" << endl;
	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000); //LOCAL PORT that will be connected!!!
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton ....
	//hint.sin_addr.S_un.S_addr = INET_PTON;
	
	//Timeout config
	DWORD timeout = timeout_in_seconds * 1000;
	setsockopt(listening, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);

	cerr << "Waiting for connection..." << endl;
	// Wait for a connection for client 1:
	
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	cerr << "CLient created!" << endl;
    strcpy (host, inet_ntoa(client.sin_addr));
    cout << host << " connected on port " <<
    ntohs(client.sin_port) << endl;

	cerr << "Waiting for Client 2 connection..." << endl;
	// Wait for a connection for client 2:
	
	sockaddr_in client2;
	int client2Size = sizeof(client2);

	SOCKET client2Socket = accept(listening, (sockaddr*)&client2, &client2Size);

	cerr << "CLient2 created!" << endl;
    strcpy (host, inet_ntoa(client2.sin_addr));
    cout << host << " connected on port " <<
    ntohs(client2.sin_port) << endl;

	cerr << "Waiting for Client 3 connection..." << endl;

	sockaddr_in client3;
	int client3Size = sizeof(client3);

	SOCKET client3Socket = accept(listening, (sockaddr*)&client3, &client3Size);

	cerr << "CLient3 created!" << endl;
    strcpy (host, inet_ntoa(client3.sin_addr));
    cout << host << " connected on port " <<
    ntohs(client3.sin_port) << endl;

	// Close listening socket
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096];
	bool statusTx = 0;
	bool firstExecution = 0;

	while (true)
	{
		//Create a buffer to receive data from the sockets.
       	ZeroMemory(buf, 4096);
       	
		if (statusTx)
		{
			if(!firstExecution)
			{
				char startCMD[] = "/start";
				//Send the commando to the clients start transmission simmultaneously by the 3 clients.
				send(clientSocket, startCMD, strlen(startCMD), 0);
				send(client2Socket, startCMD, strlen(startCMD), 0);
				send(client3Socket, startCMD, strlen(startCMD), 0);
				
				//Set flag that Transmission will start.				
				firstExecution = 1;
			}
		
			// Wait for client to send data (Client 1):
			int bytesReceived1 = recv(clientSocket, buf, 4096, 0);
			
			if (bytesReceived1 == SOCKET_ERROR)
			{
				/*cerr << "Error in recv(). Quitting" << endl;
				break;*/
			}
	
			if (bytesReceived1 == 0)
			{
				cout << "Client disconnected " << endl;
				break;
			}
		
			if(bytesReceived1 != -1 && (bytesReceived1 != 2))
			{
				string(buf, 0, bytesReceived1);
				buf[strlen(buf)] = '\0';
				printf("\nClient 1: %s", buf);
				printf("\nbyte: %i", bytesReceived1);
				/*printf("\nClient 1: %c", (char*)bytesReceived1);
				printf("\nbyte: %i", bytesReceived1);*/
				//cout << "Client 1 data: "<< string(buf, 0, bytesReceived1) << endl;
			}	
			
			//Receiving for Client 2:	
			
			int bytesReceived2 = recv(client2Socket, buf, 4096, 0);
			if (bytesReceived2 == SOCKET_ERROR)
			{
				/*cerr << "Error in recv(). Quitting" << endl;
				break;*/
			}
	
			if (bytesReceived2 == 0)
			{
				cout << "Client disconnected " << endl;
				break;
			}
     		if(bytesReceived2 != -1 && (bytesReceived2 != 2))
     		{
     			//cout << "Client 2 data: "<< string(buf, 0, bytesReceived2) << endl;
     			string(buf, 0, bytesReceived2);
     			buf[strlen(buf)] = '\0';
     			printf("\nClient 2: %s", buf);
     			printf("\nbyte: %i", bytesReceived2);
			}
     		
     		//Receiving for cleint 3:
     		
     		int bytesReceived3 = recv(client3Socket, buf, 4096, 0);
			if (bytesReceived3 == SOCKET_ERROR)
			{
				/*cerr << "Error in recv(). Quitting" << endl;
				break;*/
			}
	
			if (bytesReceived3 == 0)
			{
				cout << "Client disconnected " << endl;
				break;
			}
     		if(bytesReceived3 != -1 && (bytesReceived3 != 2))
     		{
     			//cout << "Client 2 data: "<< string(buf, 0, bytesReceived2) << endl;
     			string(buf, 0, bytesReceived3);
     			buf[strlen(buf)] = '\0';
     			printf("\nClient 3: %s", buf);
     			printf("\nbyte: %i", bytesReceived3);
			}
		}
		else
		{	//Check if the user wants to start the transmission.
			char cmd = 0;
			printf("\nIniciar a transmissão? [Y/N]\n");
			scanf("%c", &cmd);
			if(cmd == 'Y') statusTx = 1;
			else statusTx = 0;			
		}
		// Echo message back to client
		
		//send(clientSocket, buf, bytesReceived + 1, 0);
		
	}

	// Close the socket
	closesocket(listening);

	// Cleanup winsock
	WSACleanup();

	system("pause");
	return 0;
}
