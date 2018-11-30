#include <Servo.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "iottest-89b28.firebaseio.com"
#define FIREBASE_AUTH "FbuIpILw3Rq4dp9DSRxpI6se23dzJbN8eUIF1SCS"
#define WIFI_SSID "Swapnadeep"
#define WIFI_PASSWORD "deep7378"

Servo servo;

const int trigPin = 0;
const int echoPin = 2;

const int trigPinBin = 12;
const int echoPinBin = 14;

const int servoPin = 15;

int percentGarbage;
long durationBin;
long distanceBin;
long averageBin;
long averBin[3];

long duration;
long dist;
long average;
long aver[3];

void setup()
{
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  servo.attach(servoPin);
  servo.write(180);
  delay(100);
  servo.detach();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(trigPinBin, OUTPUT);
  pinMode(echoPinBin, INPUT);
}

void measure()
{
  digitalWrite(10, HIGH);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  dist = (duration / 2) / 29.1;
}

void measureBin()
{
  digitalWrite(10, HIGH);
  digitalWrite(trigPinBin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinBin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPinBin, LOW);
  pinMode(echoPinBin, INPUT);
  durationBin = pulseIn(echoPinBin, HIGH);
  distanceBin = (durationBin / 2) / 29.1;
}

void loop()
{
  for (int i = 0; i <= 2; i++)
  {
    measure();
    aver[i] = dist;
    delay(10);
  }
  dist = (aver[0] + aver[1] + aver[2]) / 3;
  Serial.println(dist);

  for (int i = 0; i <= 2; i++)
  {
    measureBin();
    averBin[i] = distanceBin;
    delay(10);
  }
  distanceBin = (averBin[0] + averBin[1] + averBin[2]) / 3;
  Serial.print("Distance Bin : ");
  Serial.println(distanceBin);

  percentGarbage = 100 - ((distanceBin * 100) / 44);
  Firebase.setInt("garbage", percentGarbage);

  if (Firebase.failed())
  {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    return;
  }

  if (dist < 50 && (distanceBin > 5 || distanceBin > 2000))
  {
    servo.attach(servoPin);
    delay(1);
    servo.write(0);
    delay(5000);
    servo.write(180);
    delay(1000);
    servo.detach();
  }

  String openLid = Firebase.getString("google/open");
  Serial.println(openLid);

  if (openLid == "1")
  {
    servo.attach(servoPin);
    delay(1);
    servo.write(0);
    delay(3000);
    servo.write(180);
    delay(1000);
    Firebase.setString("/google/open", "2");
    servo.detach();
  }
  if (openLid == "3")
  {
    servo.attach(servoPin);
    delay(1);
    servo.write(0);
  }
  if (openLid == "4")
  {
    delay(1);
    servo.write(180);
    delay(100);
    Firebase.setString("/google/open", "2");
    servo.detach();
  }
}
