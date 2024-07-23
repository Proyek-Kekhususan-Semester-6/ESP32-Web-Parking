#include <WiFi.h>
#include <HTTPClient.h>
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>

char ssid[] = "Infinix GT 10 Pro";
char pass[] = "123123123";

const int triggerPin1 = 26;
const int echoPin1 = 27;
const int ultrasonicSensorId1 = 1;

const int triggerPin2 = 32;
const int echoPin2 = 33;
const int ultrasonicSensorId2 = 2;

const int triggerPin3 = 14;
const int echoPin3 = 12;
const int ultrasonicSensorId3 = 3;


const String serverUrl = "https://webparkingiot.6l9.dev/parking";


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);
  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(triggerPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");
}

void loop() {
  long duration1, distance1;
  long duration2, distance2;
  long duration3, distance3;

  // Measure distance for sensor 1
  digitalWrite(triggerPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  // Measure distance for sensor 2
  digitalWrite(triggerPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  // Measure distance for sensor 3
  digitalWrite(triggerPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = duration3 * 0.034 / 2;

  Serial.print("Distance Sensor 1: ");
  Serial.println(distance1);
  Serial.print("Distance Sensor 2: ");
  Serial.println(distance2);
  Serial.print("Distance Sensor 3: ");
  Serial.println(distance3);

  String ultrasonicStatus1, ultrasonicStatus2, ultrasonicStatus3;

  if (distance1 < 10) {
    ultrasonicStatus1 = "terisi";
    sendDataToServer(ultrasonicSensorId1, ultrasonicStatus1);
  } else {
    ultrasonicStatus1 = "kosong";
    
    sendDataToServer(ultrasonicSensorId1, ultrasonicStatus1);
  }

  if (distance2 < 10) {
    ultrasonicStatus2 = "terisi";
    sendDataToServer(ultrasonicSensorId2, ultrasonicStatus2);
  } else {
    ultrasonicStatus2 = "kosong";
    sendDataToServer(ultrasonicSensorId2, ultrasonicStatus2);
  }

  if (distance3 < 10) {
    ultrasonicStatus3 = "terisi";
    sendDataToServer(ultrasonicSensorId3, ultrasonicStatus3);
  } else {
    ultrasonicStatus3 = "kosong";
    sendDataToServer(ultrasonicSensorId3, ultrasonicStatus3);
  }

  // sendDataToServer(1, "terisi");
  // sendDataToServer(ultrasonicSensorId2, ultrasonicStatus2);
  // sendDataToServer(ultrasonicSensorId3, ultrasonicStatus3);

}

void sendDataToServer(int id, String status) {
  if (WiFi.status() == WL_CONNECTED) {
    
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    String httpRequestData = "{\"id_slot_parkir\":"; 
    httpRequestData += id;
    httpRequestData += ",\"status\":\"";
    httpRequestData += status;
    httpRequestData += "\"}";
   
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response: " + response);
    } else {
      Serial.println("Error sending data to server");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end();

  } else {
    Serial.println("WiFi Disconnected. Unable to send data.");
  }
}