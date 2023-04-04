#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>

// #include <DHT.h>
// #include <DHT_U.h>
#include "DHT.h"

#include "SoftwareSerial.h"

SoftwareSerial esp8266(2, 3);

#define DHTPIN 4
#define DHTTYPE DHT11

#define DEBUG true

WiFiEspClient espClient;

#define WIFI_AP "MUITILASER 1200AC 2.4G"
#define WIFI_PASSWORD "123456789"

int status = WL_IDLE_STATUS;
unsigned long lastSend;

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(9600);

  dht.begin();

  InitWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:

  status = WiFi.status();
  if(status != WL_CONNECTED) {
    while( status != WL_CONNECTED) {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(WIFI_AP);

      status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      delay(500);
    }
    Serial.println("Connected to AP");
  }
  getAndSendTemperatureAndHumidityData();
}

void getAndSendTemperatureAndHumidityData() {
  Serial.println("Coletando os dados");

  float h = dht.readHumidity();

  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na leitura do sensor DHT!");
    return;
  }

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print(" Temperatura: ");
  Serial.print(t);
  Serial.print(" °C ");

  String temperature = String(t);
  String humidity = String(h);

  Serial.print( "Temperatura and Umidade : [");
  Serial.print( temperature ); Serial.print( "," );
  Serial.print( humidity );
  Serial.print( "] -> " );

  delay(5000);
}

void InitWiFi() {
    
    esp8266.begin(19200);

    WiFi.init(&esp8266);

    if(WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");

      while(true);
    }

    Serial.println("Connecting to AP ...");

    while( status != WL_CONNECTED) {
      Serial.print("Attemting to connect to WAP SSID: ");
      Serial.println(WIFI_AP);

      status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      delay(500);
    }
    Serial.println("Connected to AP: ");
    Serial.println(WiFi.localIP());
  }