//Trabalho Winsock -- 2019
//4411 - Equipe Alpha
//Ana Paula Schneider, Eduarda Basotti, Fernando Caletti, Kauê Portella, Gustavo Wingert e Henrique Schumacher.
//Windows Client


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
	//Create timers to control the data sending only in 15 seconds;
	time_t timer1;
	time_t timer2;
	char buf[4096];
	string userInput;
	bool transmissionStatus = 0;
	char startByte[] = "/start";
	int timStatus = 0;
	while(true)
	{
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
				//Check timer status, if timer status == 0, let the user sent a message.
				if(!timStatus)
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
					//After the user send the message, timerstatus are changed to 1.
					timStatus = 1;
				}
				else if(timStatus == 1)
				{
					//Get the current time about executing the program.
					time(&timer1);
					timStatus = 2;
				}
				else if(timStatus == 2)
				{
					//Get the current time again and compare with the time gotten before.
					//If the resulting time is 15, the timStatus are changed to 0 again.
					//This mode, the user only can send messages every 15 seconds.
					time(&timer2);
					double timePassed = timer2 - timer1;
					if(timePassed == 15)
					{
						timStatus = 0;
					}
				}
			} while (userInput.size() > 0);
		}
	}
	
	


	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}
