#include <iostream>
#include <string>
#include <winsock2.h>
#include <stdio.h>
#include <time.h>
//#include <WS2tcpip.h>
//#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(void)
{
	char ipAddress[] = "127.0.01";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return -1;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return -2;
	}
	cout << "Socket created!" << endl;
	cout << "Configuring connection..." << endl;
	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	//inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	//inet_ntoa(client2.sin_addr)
	//hint.sin_addr.S_un.S_addr = ;
	hint.sin_addr.s_addr = inet_addr(ipAddress);
	//Convert IP adress in string
	char bufs[1024];
	ZeroMemory(bufs, 1024); // same as memset(host, 0, NI_MAXHOST);
	strcpy (bufs, inet_ntoa(hint.sin_addr));
    cout << bufs << " connected on port " <<
    ntohs(hint.sin_port) << endl;
    
	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return -3;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	string userInput;
	bool transmissionStatus = 0;
	char startByte[] = "/start";
	if(!transmissionStatus)
	{
		// Wait for start byte
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(sock, buf, 4096, 0);
		if (bytesReceived > 0)
		{
			//Convert the received data to string
			string(buf, 0, bytesReceived);
			buf[strlen(buf)] = '\0';
			
			int confirmation = 0;
			for(int i = 0; i<strlen(buf); i++)
			{
				if(buf[i] == startByte[i]) confirmation ++;
			}
			printf("Confirmation value: %i\n", confirmation);
			if(confirmation == 6)
			{
				transmissionStatus = 1;
				cout << "Start byte confirmed! Starting transmission..." << endl;
			 } 
			// Echo response to console
		//	cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
		}
	}
	if(transmissionStatus)
	{
		do
		{	
			// Prompt the user for some text
			cout << "> ";
			getline(cin, userInput);
	
			if (userInput.size() > 0)		// Make sure the user has typed in something
			{
				// Send the text
				int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
				if (sendResult != SOCKET_ERROR)
				{
					//Wait for info
				}
			}
		} while (userInput.size() > 0);
	}	
	


	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}
