#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";   

const char* server = "http://api.thingspeak.com/update";
String apiKey = "J2YEQ6S4FD2Y5YKE";  

#define DHTPIN 2     
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

#define LDRPIN 34      

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  dht.begin();

  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int luminosidade = analogRead(LDRPIN);

  Serial.print("Temp: ");
  Serial.print(temperatura);
  Serial.print(" °C | Umid: ");
  Serial.print(umidade);
  Serial.print(" % | Luz: ");
  Serial.println(luminosidade);

  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey +
                 "&field1=" + String(temperatura) +
                 "&field2=" + String(umidade) +
                 "&field3=" + String(luminosidade);

    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("Dados enviados para ThingSpeak!");
    } else {
      Serial.println("Erro ao enviar.");
    }
    http.end();
  }

  delay(20000); 
}