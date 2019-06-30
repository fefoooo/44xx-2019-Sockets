#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <ctime>

using namespace std;

int main()
{
    // Create a socket
    int socket1 = socket(AF_INET, SOCK_STREAM, 0);
    if(socket1 == -1)
    {
        return 1;
    }
    //Save the values to the connection.
    int port = 54000;
    string ipAddr = "192.168.0.5";
    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddr.c_str(), &hint.sin_addr);

    //Connect to the server socket
    int connection = connect(socket1, (sockaddr*)&hint, sizeof(hint));
    if(connection == -1)
    {
        return 1;
    }

    //While loop
    char buf[4096];
    time_t timer1;
	time_t timer2;
	string userInput;
	bool transmissionStatus = 0;
	char startByte[] = "/start";
	int timStatus = 0;

    while (true)
    {
        memset(buf, 0, 4096);
		if(!transmissionStatus)
		{
			// Wait for start byt
			int bytesReceived = recv(socket1, buf, 4096, 0);
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
						int sendResult = send(socket1, userInput.c_str(), userInput.size() + 1, 0);
						if (sendResult != -1)
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
					if(timePassed == 1)
					{
						timStatus = 0;
					}
				}
			} while (userInput.size() > 0);
		}



    }
    // Close the socket
    close(socket1);

    return 0;
}
