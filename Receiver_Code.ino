#include <SPI.h>
#include <LoRa.h>

void setup() {
    Serial.begin(9600); 

    while (!Serial);

    Serial.println("LoRa Receiver");

    if (!LoRa.begin(915E6)) {
        Serial.println("LoRa init failed!");
        while (1);
    }
}

void loop() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        Serial.print("Received Data: ");
        
        String receivedData = "";
        while (LoRa.available()) {
            char incoming = (char)LoRa.read();
            Serial.print(incoming);    
            receivedData += incoming;  
        }
        Serial.println();
        Serial.println(receivedData);
    }
}
