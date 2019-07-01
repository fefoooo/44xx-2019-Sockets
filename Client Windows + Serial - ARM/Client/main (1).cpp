#include <windows.h>
//#include <conio2.h>
#include <stdio.h>
#include <stdlib.h>
//#include <myconio.h>
#include <conio.h>

HANDLE AbreComm(char *nomecom, int baudrate)
{
    HANDLE hcom; //cria um ponteiro de nome hcom
    DCB cdcb;    //nomeia a estrutura DCB (Device Control Block)
                 //utilizada para definir todos os par�metros da comunica��o
    COMMTIMEOUTS comto; //nomeia a estrutura COMMTIMEOUTS (COMMon TIME OUTS)
                        //utilizada para definir os timeouts da comunica��o
    hcom = CreateFile(
           nomecom, //nome do arquivo
           GENERIC_READ | GENERIC_WRITE, //abre arquivo para leitura ou escrita
           0, //indica que o arquivo n�o pode ser compartilhado
           NULL, //utiliza a estrutura default para as fun��es de seguran�a
           OPEN_EXISTING, //abre o arquivo, se n�o existir, retorna erro
           FILE_ATTRIBUTE_NORMAL, //o arquivo deve ser utilizado sozinho
           NULL); //sem gabarito de atributos

    if(hcom == INVALID_HANDLE_VALUE) // testa falha na abertura do arquivo
    {
      fprintf(stderr, "Nao abriu a %s\n", nomecom);
      return hcom;
    }

    GetCommState(hcom, &cdcb); //le os par�metros de comunica��o atuais
    cdcb.BaudRate    = baudrate; //define a taxa de transmiss�o
    cdcb.ByteSize    = 8; //define o tamanho do dado - 8 bits
    cdcb.StopBits    = ONESTOPBIT; //define o tamanho do stop bit - 1 stop bit
    cdcb.Parity      = NOPARITY; //define o tipo de paridade - sem paridade


    if(!SetCommState(hcom,&cdcb)) //seta os novos par�metros de comunica��o
    {
      fputs("SetCommState", stderr);
      return INVALID_HANDLE_VALUE;
    }


    GetCommTimeouts(hcom, &comto); //Le os par�metros atuais de COMMTIMEOUTS
    comto.ReadIntervalTimeout         = MAXDWORD;//tempo m�ximo entre a chegada
                                                 //de dois caracters consecutivos(ms)
    comto.ReadTotalTimeoutMultiplier  =    0;
    comto.ReadTotalTimeoutConstant    =    0;
    comto.WriteTotalTimeoutMultiplier =    0;
    comto.WriteTotalTimeoutConstant   =    0;
    SetCommTimeouts(hcom, &comto); //seta os par�metros de COMMTIMEOUTS
    return hcom;
}

int main()
{

 	HANDLE hcom; //cria o ponteiro, �rea de mem�ria intermedi�ria, buffer
    unsigned long n;
    char *ncom="COM4", c=1, l=1, c2=1; //define nome do arquivo, no nosso caso a porta usada
    int baud=115200; //define a taxa de transmiss�o
    char dado[1], dado2[1]; //cria o buffer de programa, vari�vel do programa
    hcom=AbreComm(ncom,baud); //abre o aquivo e relaciona com ponteiro
    if(hcom==INVALID_HANDLE_VALUE) {getch();return 1;} //se n�o abriu arquivo/porta,
                                                       // encerra o programa
    while(1)
    {
       ReadFile(hcom, dado, 1, &n, NULL); // le um dado do arquivo/porta
    }

    CloseHandle(hcom); //fecha porta
    return 0;
}
