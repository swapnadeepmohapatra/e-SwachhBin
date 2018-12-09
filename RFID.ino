#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define SS_PIN 2
#define RST_PIN 0
#define LED_PIN 16

#define FIREBASE_HOST "iottest-89b28.firebaseio.com"
#define FIREBASE_AUTH "FbuIpILw3Rq4dp9DSRxpI6se23dzJbN8eUIF1SCS"

#define WIFI_SSID "Swapnadeep"
#define WIFI_PASSWORD "deep7378"

MFRC522 mfrc522(SS_PIN, RST_PIN);
int statuss = 0;
int out = 0;
int n = 0;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_PIN, OUTPUT);

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
}
void loop()
{
  digitalWrite(LED_PIN, HIGH);
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  Serial.println();
  Serial.print(" UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();

  // Allow to get Points
  Serial.println(content.substring(1));
  String garbageOld = Firebase.getString("gar");
  String garbagePresent = Firebase.getString("garbage");

  Serial.println(garbageOld);
  Serial.println(garbagePresent);

  // Checks
  if (garbageOld != garbagePresent)
  {
    digitalWrite(LED_PIN, LOW);
    String name = Firebase.pushInt(content.substring(1), n);
    if (Firebase.failed())
    {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());
      return;
    }
    Serial.print("pushed: /logs/");
    Serial.println(name);
  }

  delay(1000);
}
