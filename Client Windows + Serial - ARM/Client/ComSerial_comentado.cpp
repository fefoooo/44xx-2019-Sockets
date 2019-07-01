#include <windows.h>
#include <conio2.h>
#include <stdio.h>
#include <stdlib.h>
//#include <myconio.h>
#include <conio.h>


/****************************************************************
Abre uma porta de comunicação RS232
Retorna:
	Handle do arquivo de comunicação se foi bem sucedido
	INVALID_HANDLE_VALUE de não conseguiu.
******************************************************************/

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
            textcolor(3);
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
           textcolor(15);
           if(c2>10) c2=1;
           gotoxy(c2++,22);
           printf("%c",*dado); //escreve dado enviado para o buffer na tela
           textcolor(7);
        }
    }while(*dado!=27); //se dado diferente de ctrl-X, retorna
    CloseHandle(hcom); //fecha porta
    return 0; //encerra o main()
}
