#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>


using namespace std;

#define NI_MAXHOST 1024
#define NI_MAXSERV 1024
#define timeout_in_seconds 1

//Handle to change text color
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()

int main(void)
{
	//Initialize winSock

	// Set parameters and create data buffers.
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

	//Create SOCKET to stabilish connections.
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
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	
	//Configure timeout
	DWORD timeout = timeout_in_seconds * 1000;
	setsockopt(listening, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);

	//Then we need to stabilish connection to a client.
	cerr << "Waiting for client 1 connection..." << endl;
	//Creates a client to comunicate with Client 1.
	/////////////////////////////Conection with Client 1 ////////////////////////
	//Socket address to store client conection.
	sockaddr_in client;
	int clientSize = sizeof(client);
	//Creates another socket by accepting the comunication.
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	/////////////////////////////////////////////////////////////////////////////
	//Creates variables to store conection host and service.
	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on
	//Reserve local memory.
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	//Connection stabilished with Client 1!
	cerr << "CLient created!" << endl;
    strcpy (host, inet_ntoa(client.sin_addr));
    cout << host << " connected on port " <<
    ntohs(client.sin_port) << endl;

    cerr << "Waiting for Client 2 connection..." << endl;

    ////////////////////////////Connection with Client 2 /////////////////////////
	sockaddr_in client2;
	int client2Size = sizeof(client2);

	SOCKET client2Socket = accept(listening, (sockaddr*)&client2, &client2Size);
	//////////////////////////////////////////////////////////////////////////////
	//Connection stabilished with Client 2!
	cerr << "CLient2 created!" << endl;
    strcpy (host, inet_ntoa(client2.sin_addr));
    cout << host << " connected on port " <<
    ntohs(client2.sin_port) << endl;

    cerr << "Waiting for Client 3 connection..." << endl;

 	////////////////////////////Connection with Client 3 //////////////////////////
	sockaddr_in client3;
	int client3Size = sizeof(client3);

	SOCKET client3Socket = accept(listening, (sockaddr*)&client3, &client3Size);
	///////////////////////////////////////////////////////////////////////////////
	//Connection stabilished with Client 3!
	cerr << "CLient3 created!" << endl;
    strcpy (host, inet_ntoa(client3.sin_addr));
    cout << host << " connected on port " <<
    ntohs(client3.sin_port) << endl;

    //Close the listening socket.
    closesocket(listening);

    //Create variables to control the receiving of data.
    bool statusTx = 0;
    bool firstExecution = 0;
    //Create a buffer to te received data.
    char buf[4096];
    ZeroMemory(buf, 4096);

    //Check if the user wants to start the receiving.
    do
    {
    	char cmd = 0;
    	printf("\nStart transmission? [Y/N]\n");
		scanf("%c", &cmd);
		if(cmd == 'Y') statusTx = 1;
		else
		{
			 statusTx = 0;
		}

    }while(!statusTx);

    //To start the transmission of all the clients at the same time, we send a start message.
	char startCMD[] = "/start";
	//Send the command to the clients start transmission simmultaneously by the 3 clients.
	send(clientSocket, startCMD, strlen(startCMD), 0);
	send(client2Socket, startCMD, strlen(startCMD), 0);
	send(client3Socket, startCMD, strlen(startCMD), 0);
	
	//While loop: Check constantly the received data from the clients and print ir on the screen.
	while(true)
	{
		ZeroMemory(buf, 4096); 
		//Receive data from client 1:
		int bytesReceived1 = recv(clientSocket, buf, 4096, 0);	
		//Check if any error has ocurred.	
		if (bytesReceived1 == SOCKET_ERROR)
		{
			//Received: None. Don`t do nothing.
			//cerr << "Received Nothing..." << endl;
		}else if (bytesReceived1 == 0)
		{
			cerr << "Client disconnected " << endl;
			break;
		}else if(bytesReceived1 != -1)
		{
			string(buf, 0, bytesReceived1);
			buf[strlen(buf)] = '\0';
			//Print data reived from client 1.
			SetConsoleTextAttribute(console, 4);
			printf("\nClient 1: %s", buf);
		}
		//Receiving from Client 2:		
		int bytesReceived2 = recv(client2Socket, buf, 4096, 0);
		//Check if any error has ocurred.
		if (bytesReceived2 == SOCKET_ERROR)
		{
			//Received: None. Don`t do nothing.
			//cerr << "Received Nothing..." << endl;		
		}else if (bytesReceived2 == 0)
		{
			cerr << "Client disconnected " << endl;
			break;
		}else if(bytesReceived2 != -1)
 		{
 			string(buf, 0, bytesReceived2);
 			buf[strlen(buf)] = '\0';
 			//Print data reived from client 1.
 			SetConsoleTextAttribute(console, 3);
 			printf("\nClient 2: %s", buf);
		} 		
 		//Receiving for cleint 3: 		
 		int bytesReceived3 = recv(client3Socket, buf, 4096, 0);
 		//Check if any error has ocurred.
		if (bytesReceived3 == SOCKET_ERROR)
		{
			//Received: None. Don`t do nothing.
			//cerr << "Received Nothing..." << endl;
		}else if (bytesReceived3 == 0)
		{
			cerr << "Client disconnected " << endl;
			break;
		}else if(bytesReceived3 != -1)
 		{
 			string(buf, 0, bytesReceived3);
 			buf[strlen(buf)] = '\0';
 			//Print data reived from client 1.
 			SetConsoleTextAttribute(console, 2);
 			printf("\nClient 3: %s", buf);
		}
	}

	// Close the sockets.
	closesocket(clientSocket);
	closesocket(client2Socket);
	closesocket(client3Socket);

	// Cleanup winsock.
	WSACleanup();

	//End the program.
	system("pause");
	return 0;

}


