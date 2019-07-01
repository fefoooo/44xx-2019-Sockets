/*
    This sketch sends a message to a TCP server

*/

#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

uint8_t adcEntrada = 34;
uint8_t adcSaida = 35;

WiFiClient client;

uint16_t entradaValor = 0;
uint16_t saidaValor = 0;
uint16_t pessoas = 0;

boolean start = 0;
boolean atual = 0;
boolean passEntrada = 0;
boolean passSaida = 0;
boolean startTX = 0; 

unsigned long previousMillis = 0;

byte dado = 0;

const long interval = 15000;

void setup()
{
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  WiFiMulti.addAP("raspberryfefo", "hqztwwiw");

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}


void loop()
{
  unsigned long currentMillis = millis();

  if ( currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    atual = 1;
  }

  const uint16_t port = 54000;
  const char * host = "192.168.0.5"; // ip or dns

  if (start == 0)
  {
    Serial.print("Connecting to ");
    Serial.println(host);

    if (!client.connect(host, port)) {
      Serial.println("Connection failed.");
      Serial.println("Waiting 5 seconds before retrying...");
      delay(5000);
      return;
    }
  }

  if (analogRead(adcEntrada) == 0)
  {
    if (passEntrada == 0)
    {
      pessoas += 1;
      Serial.println(pessoas);
      passEntrada = 1;
    }
  }
  else if (analogRead(adcEntrada) != 0)
  {
    passEntrada = 0;
  }
  if (analogRead(adcSaida) == 0)
  {
    if (passSaida == 0)
    {
      pessoas -= 1;
      Serial.println(pessoas);
      passSaida = 1;
    }
  }
  else if (analogRead(adcSaida) != 0)
  {
    passSaida = 0;
  }

  if (atual == 1)
  {
    // This will send a request to the server

    //read back one line from the server
    String line = client.readStringUntil('\r');
    client.println("\npessoas: ");
    client.print(pessoas);
    atual = 0;
  }

  if (Serial.available())
  {
    dado = Serial.read();
    if (dado == 's')
    {
      client.stop();
    }
    if (dado == 'p')
    {
      startTX = 1;
    }
  }
  start = 1;
}
