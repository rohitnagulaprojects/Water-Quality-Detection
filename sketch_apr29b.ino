#include <WiFi.h>
#include <HTTPClient.h>

#define RXp2 16
#define TXp2 17

const char* ssid = "Om sai ram";
const char* password = "ramesh7130";
const char* server = "api.thingspeak.com";
const String apiKey = "NWDUF19BEX91UL12";

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (Serial2.available()) {
    String message = Serial2.readStringUntil('\n');
    Serial.print("Received message from Arduino: ");
    Serial.println(message);

    // Split the message into turbidity and temperature values
    int turbidityIndex = message.indexOf("Turbidity:");
    int temperatureIndex = message.indexOf("Temperature:");

    if (turbidityIndex != -1 && temperatureIndex != -1) {
      String turbidityValueStr = message.substring(turbidityIndex + 10, temperatureIndex);
      String temperatureValueStr = message.substring(temperatureIndex + 12);

      float turbidityValue = turbidityValueStr.toFloat();
      float temperatureValue = temperatureValueStr.toFloat();

      // Send data to ThingSpeak
      if (WiFi.status() == WL_CONNECTED) {
        String url = "http://";
        url += server;
        url += "/update?api_key=";
        url += apiKey;
        url += "&field1=";
        url += String(turbidityValue);
        url += "&field2=";
        url += String(temperatureValue);

        Serial.print("Sending data to ThingSpeak: ");
        Serial.println(url);

        HTTPClient http;
        http.begin(url);
        int httpCode = http.GET();
        if (httpCode > 0) {
          Serial.printf("HTTP response code: %d\n", httpCode);
          String payload = http.getString();
          Serial.println(payload);
        } else {
          Serial.println("Failed to connect to ThingSpeak");
        }
        http.end();
      } else {
        Serial.println("WiFi not connected");
      }
    } else {
      Serial.println("Invalid message format");
      Serial.println("Turbidity index: " + String(turbidityIndex));
      Serial.println("Temperature index: " + String(temperatureIndex));
      Serial.println("Message length: " + String(message.length()));
      Serial.println("Message content: " + message);
    }
  }
}
