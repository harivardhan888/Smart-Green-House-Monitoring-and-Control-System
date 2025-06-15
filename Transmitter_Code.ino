#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define DHTPIN 5   
#define DHTTYPE DHT11 
#define SOIL_MOISTURE_PIN A0  
#define MOTOR_PIN 4    

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    while (!Serial);

    Serial.println("LoRa Transmitter");
    if (!LoRa.begin(915E6)) { 
        Serial.println("LoRa init failed!");
        while (1);
    }
    pinMode(SOIL_MOISTURE_PIN, INPUT);
    pinMode(MOTOR_PIN, OUTPUT);
    dht.begin();
}

void loop() {
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    String soilStatus;
    if (soilMoisture > 800) {
        soilStatus = "Very Dry";
        digitalWrite(MOTOR_PIN, HIGH); 
    } else if (soilMoisture > 500) {
        soilStatus = "Moderate";
        digitalWrite(MOTOR_PIN, LOW); 
    } else {
        soilStatus = "Moist";
        digitalWrite(MOTOR_PIN, LOW); 
    }

    String tempStatus = (temperature > 30) ? "High" : (temperature > 20) ? "Moderate" : "Low";
    String humidityStatus = (humidity > 70) ? "High" : (humidity > 40) ? "Moderate" : "Low";

    LoRa.beginPacket();
    LoRa.print("Soil: " + soilStatus);
    LoRa.print(", Temp: " + tempStatus);
    LoRa.print(", Hum: " + humidityStatus);
    LoRa.endPacket();

    Serial.println("Sent Data:");
    Serial.println("Soil Moisture: " + soilStatus);
    Serial.println("Temperature: " + tempStatus);
    Serial.println("Humidity: " + humidityStatus);
    Serial.println("-------------------------");

    delay(500);
}