#define BLYNK_TEMPLATE_ID "TMPL6hTsqvdDp"
#define BLYNK_TEMPLATE_NAME "egg inc"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

// -------- Blynk credentials --------
char auth[] = "J6dLbDSWi-F7ojPwzx8CabtIGYqitCRj";
char ssid[] = "Pixel 6a";   // ⚠️ MUST be 2.4GHz
char pass[] = "1234512345";

// -------- DHT11 setup --------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// -------- Fan / Relay --------
#define RELAY_PIN 5

float tempOn  = 38.0;
float tempOff = 37.0;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup started");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  dht.begin();

  Serial.println("Connecting to Blynk...");
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("❌ DHT read failed");
    delay(2000);
    return;
  }

  if (temperature >= tempOn) {
    digitalWrite(RELAY_PIN, HIGH);
  } else if (temperature <= tempOff) {
    digitalWrite(RELAY_PIN, LOW);
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | Hum: ");
  Serial.print(humidity);
  Serial.print(" % | Fan: ");
  Serial.println(digitalRead(RELAY_PIN));

  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, digitalRead(RELAY_PIN));

  delay(2000);
}
