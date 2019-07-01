#include <conio.h>
#include <conio2.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <winsock2.h>
//#include <WS2tcpip.h>
//#pragma comment(lib, "ws2_32.lib")

HANDLE AbreComm(char *nomecom, int baudrate)
{
    HANDLE hcom; //cria um ponteiro de nome hcom
    DCB cdcb;    //nomeia a estrutura DCB (Device Control Block)
                 //utilizada para definir todos os parâmetros da comunicação
    COMMTIMEOUTS comto; //nomeia a estrutura COMMTIMEOUTS (COMMon TIME OUTS)
                        //utilizada para definir os timeouts da comunicação
    hcom = CreateFile(
           nomecom, //nome do arquivo
           GENERIC_READ | GENERIC_WRITE, //abre arquivo para leitura ou escrita
           0, //indica que o arquivo não pode ser compartilhado
           NULL, //utiliza a estrutura default para as funções de segurança
           OPEN_EXISTING, //abre o arquivo, se não existir, retorna erro
           FILE_ATTRIBUTE_NORMAL, //o arquivo deve ser utilizado sozinho
           NULL); //sem gabarito de atributos

    if(hcom == INVALID_HANDLE_VALUE) // testa falha na abertura do arquivo
    {
      fprintf(stderr, "Nao abriu a %s\n", nomecom);
      return hcom;
    }

    GetCommState(hcom, &cdcb); //le os parâmetros de comunicação atuais
    cdcb.BaudRate    = baudrate; //define a taxa de transmissão
    cdcb.ByteSize    = 8; //define o tamanho do dado - 8 bits
    cdcb.StopBits    = ONESTOPBIT; //define o tamanho do stop bit - 1 stop bit
    cdcb.Parity      = NOPARITY; //define o tipo de paridade - sem paridade


    if(!SetCommState(hcom,&cdcb)) //seta os novos parâmetros de comunicação
    {
      fputs("SetCommState", stderr);
      return INVALID_HANDLE_VALUE;
    }


    GetCommTimeouts(hcom, &comto); //Le os parâmetros atuais de COMMTIMEOUTS
    comto.ReadIntervalTimeout         = MAXDWORD;//tempo máximo entre a chegada
                                                 //de dois caracters consecutivos(ms)
    comto.ReadTotalTimeoutMultiplier  =    0;
    comto.ReadTotalTimeoutConstant    =    0;
    comto.WriteTotalTimeoutMultiplier =    0;
    comto.WriteTotalTimeoutConstant   =    0;
    SetCommTimeouts(hcom, &comto); //seta os parâmetros de COMMTIMEOUTS
    return hcom;
}

using namespace std;

int main(void)
{
	HANDLE hcom; //cria o ponteiro, área de memória intermediária, buffer
    unsigned long n;
    char *ncom="COM4",c=1,l=1, c2=1; //define nome do arquivo, no nosso caso a porta usada
    int baud=115200; //define a taxa de transmissão
    char dado[1], dado2[1]; //cria o buffer de programa, variável do programa
    hcom=AbreComm(ncom,baud); //abre o aquivo e relaciona com ponteiro
    if(hcom==INVALID_HANDLE_VALUE) {getch();return 1;} //se não abriu arquivo/porta,
                                                       // encerra o programa
    gotoxy(1,24);
    printf("Terminal %s %dbaud\t\t\t\t\t\tESC para sair", ncom, baud);
    do{
       //-----------------------------------------------------------------------
       // função para ler um dado da porta
       // hcom = nome do ponteiro
       // dado = buffer de dados
       // 1 = número de bytes a receber do buffer
       // &n = número de bytes efetivamente recebidos
       // NULL = sem sobreposição/overlapped
       ReadFile(hcom, dado, 1, &n, NULL); // le um dado do arquivo/porta
       //-----------------------------------------------------------------------
       if(n) // se dado?
       {
             if(c>80) c=1, l++;
             if(l>15) l=1;
             gotoxy(c++,l);
            textcolor(12);
             printf("%c",*dado); //escreve na tela
            if(*dado=='\n') {c=1; l++;}
            textcolor(7);
       }
       if(kbhit()) //se tecla pressionada
   	   {
           *dado=getch();
           if(*dado=='0') system("cls"), _sleep(10);
           if(*dado=='1') system("cls"), _sleep(10);
           if(*dado=='2') system("cls"), _sleep(10);
           if(*dado=='3') system("cls"), _sleep(10);
           c=1;
           l=1;
           gotoxy(1,24);
           printf("Terminal %s %dbaud\t\t\t\t\t\tESC para sair", ncom, baud);
           //-------------------------------------------------------------------
           // função para enviar/escrever um dado no buffer
           // hcom = nome do ponteiro
           // dado = buffer de dados
           // 1 = número de bytes a enviar/escrever para o buffer
           // &n = número de bytes efetivamente escritos/enviados
           // NULL = sem sobreposição/overlapped
    	   WriteFile(hcom,dado, 1, &n, NULL); //escreve conteúdo do buffer no
           //arquivo/porta
    	   //-------------------------------------------------------------------
           textcolor(5);
           if(c2>10) c2=1;
           gotoxy(c2++,22);
           printf("%c",*dado); //escreve dado enviado para o buffer na tela
           textcolor(7);
        }
    }while(*dado!=27); //se dado diferente de ctrl-X, retorna
    CloseHandle(hcom); //fecha porta
    
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
