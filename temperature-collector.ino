#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>

//Biblioteca Sensor DHT
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

//Declarar o sensor
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Conectado a Porta::9600");

  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
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