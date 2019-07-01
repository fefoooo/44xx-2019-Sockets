#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

using namespace std;

#define Key_A 25
#define Key_B 24
#define Key_C 23
#define Key_D 7

int main(void)
{
	//Initialize wiringPi()
	wiringPiSetup();

	//Configure the GPIO's
	pinMode(Key_A, INPUT);
	pinMode(Key_B, INPUT);
	pinMode(Key_C, INPUT);
	pinMode(Key_D, INPUT);

	bool KeypressA, KeypressB, KeypressC, KeypressD;
	int buttonPressed = 0;

	while(true)
	{
		if(digitalRead(Key_A))
		{
			//cout << "Key_A pressed!" << endl;
			//KeypressA = 1;
		}
		else KeypressA = 0;

		if(digitalRead(Key_B))
		{
			//cout << "Key_B pressed!" << endl;
			//KeypressB = 1;
		}
		else KeypressB = 0;
		if(digitalRead(Key_C))
		{
			//cout << "Key_C pressed!" << endl;
			//KeypressC = 1;
		}
		else KeypressC = 0;		
		if(digitalRead(Key_D))
		{
			//cout << "Key_D pressed!" << endl;
			//KeypressD = 1;
		}
		else KeypressD = 0;
		
		//buttonPressed = KeypressA + KeypressB + KeypressC + KeypressD;
		buttonPressed = (digitalRead(Key_A)) + (digitalRead(Key_B)) + (digitalRead(Key_C)) + (digitalRead(Key_D));
		cout << "Botões pressionados " << buttonPressed << endl;
	}

	return 0;

}