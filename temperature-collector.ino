#include <Adafruit_Sensor.h>

#include <Adafruit_ESP8266.h>

#include <DHT.h>
#include <DHT_U.h>

#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>

// Programa: Web Server com modulo ESP8266
// Alteracoes e adaptacoes: MakerHero
 
#include <SoftwareSerial.h>

 
//RX pino 2, TX pino 3
SoftwareSerial esp8266(2, 3);

#define DHTPIN 4
#define DHTTYPE DHT11

#define DEBUG true

DHT dht(DHTPIN, DHTTYPE);
 
void setup()
{
  Serial.begin(9600);
  esp8266.begin(19200);
  dht.begin();
 
  sendData("AT+RST\r\n", 2000, DEBUG); // rst
  // Conecta a rede wireless
  sendData("AT+CWJAP=\"Comp 1 Sala 3\",\"12345678\"\r\n", 2000, DEBUG);
  delay(3000);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  // Mostra o endereco IP
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
}
 
void loop()
{
  getAndSendTemperatureAndHumidityData();  
}
 
String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}

void getAndSendTemperatureAndHumidityData() {

  if (esp8266.available())
  {
    if (esp8266.find("+IPD,"))
    {

  Serial.println("Coletando os dados");

  float h = dht.readHumidity();

  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na leitura do sensor DHT!");
    return;
  }

  delay(300);
  int connectionId = esp8266.read() - 48;

  String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
  webpage += "</head><h1><u>ESP8266 - Web Server</u></h1><h2>Porta";
  webpage += "<h2>Umidade: ";
  int a = h;
  webpage += a;
  webpage += "<h2>Temperatura: ";
  int b = t;
  webpage += b;
  webpage += " °C</h2>";

  webpage += "<h2>Temperatura & Umidade: [";
  webpage += b;
  webpage += ", ";
  webpage += a;
  webpage += "]</h2>";

  String cipSend = "AT+CIPSEND=";
  cipSend += connectionId;
  cipSend += ",";
  cipSend += webpage.length();
  cipSend += "\r\n";
 
  sendData(cipSend, 1000, DEBUG);
  sendData(webpage, 1000, DEBUG);
 
  String closeCommand = "AT+CIPCLOSE=";
  closeCommand += connectionId; // append connection id
  closeCommand += "\r\n";

  sendData(closeCommand, 3000, DEBUG);

  delay(5000);
    }
  }
}