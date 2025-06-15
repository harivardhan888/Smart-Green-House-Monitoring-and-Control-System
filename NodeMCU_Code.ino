#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>  

#define WIFI_SSID "Elijah"
#define WIFI_PASSWORD "passwordledhu"

#define FIREBASE_HOST "project-c7698-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "AIzaSyDOtk8wBjHM_STGRRVonbVtw5VzjJgUTdQ"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("NodeMCU Ready to Receive Data and Send to Firebase");
}

void loop() {
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n'); 
    receivedData.trim(); 

    if (receivedData.startsWith("Received Data:")) {
      receivedData = receivedData.substring(String("Received Data:").length());
    }
    receivedData.trim(); 
    String path = "/LoRaData/" + String(millis()); 

    Serial.print("Sending to Firebase: ");
    Serial.println(receivedData);

    if (Firebase.setString(fbdo, path, receivedData)) {  
      Serial.println("Data sent to Firebase successfully");
    } else {
      Serial.print("Error sending data: ");
      Serial.println(fbdo.errorReason());
    }
  }
}
