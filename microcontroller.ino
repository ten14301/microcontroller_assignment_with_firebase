#include <WiFi.h> 
#include <WiFiClientSecure.h> 
#include <FirebaseESP32.h> 
#include "time.h" 
#define FIREBASE_HOST "https://smart-house-f9020-default-rtdb.firebaseio.com/ " 
#define FIREBASE_AUTH "8mkP29URBskQ00ewZNQITvsFvk6DAzaF5qvSFU9B" 
#define LINE_TOKEN "H54xih3H9rg4ImU7L9m3yjbdFCYO2p3ji6LVtrxPpKH" 
#define WIFI_SSID "jangasivi" 
#define WIFI_PASSWORD "0614646485" 
//....timer 
const char* ntpServer = "pool.ntp.org"; 
const long gmtOffset_sec = 25200; 
const int daylightOffset_sec = 25200; 
struct tm timeinfo; 
String timeAlert; 
char Nowhour[3]; 
char Nowmin[3]; 
String h; 
String m; 
//firebase 
FirebaseData fbdo; 
//esp input/output 
int lamp3 = 32; 
int lamp4 = 12; 
int lamp = 14; 
int lamp2 = 13;
int pir = 27; 
int smoke = 33; 
int lpin = 34; 
int infra = 25; 
//variable 
int LineNf = 0; 
String TimeD="0"; 
int alert = 1; 
void setup() { 
// put your setup coe here, to run once: Serial.begin(115200); 
pinMode(lamp,OUTPUT); 
pinMode(lamp2,OUTPUT); 
pinMode(lamp3,OUTPUT); 
pinMode(lamp4,OUTPUT); 
pinMode(pir,INPUT); 
WiFi.begin(WIFI_SSID, WIFI_PASSWORD); Serial.print("Connecting to Wi-Fi"); while (WiFi.status() != WL_CONNECTED) { 
Serial.print("."); 
delay(500); 
} 
Serial.println(); 
Serial.print("Connected with IP: "); Serial.println(WiFi.localIP());
Serial.println(); 
configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
Firebase.reconnectWiFi(true); 
} 
void loop() { 
// put your main code here, to run repeatedly: int alert = digitalRead(pir); 
int analogIN1 = analogRead(lpin); 
delay(50); 
int analogIN3 = analogRead(smoke); 
delay(50); 
int analogIN4 = analogRead(infra); 
delay(50); 
int lh = map(analogIN1,0,4095,0,100); 
int lh1 = map(analogIN3,0,4095,0,100); 
int lh2 = map(analogIN4,0,4095,0,100); 
if(lh1>100){ 
digitalWrite(lamp4,HIGH);  
} 
else{digitalWrite(lamp4,LOW);} 
if (lh<50) 
{ 
digitalWrite(lamp,HIGH); 
} 
else 
{
digitalWrite(lamp,LOW); 
} 
if (alert==0) 
{ 
digitalWrite(lamp2,HIGH); 
LINE_Notify("FIRE Alert"); 
} 
else 
{ 
digitalWrite(lamp2,LOW); 
} 
if (lh2>50) 
{ 
digitalWrite(lamp3,HIGH); 
} 
else 
{ 
digitalWrite(lamp3,LOW); 
} 
Firebase.setInt(fbdo, "/house1/light",lh); Firebase.setInt(fbdo, "/house1/infrared",lh1); Firebase.setInt(fbdo, "/house1/smoke",lh2); Firebase.setInt(fbdo, "/house1/alert",alert); Firebase.getString(fbdo, "/house1/gate"); String fireBaseMsg1 = String(fbdo.stringData()); String gate1 = fireBaseMsg1.substring(2,4); Serial.println(gate1);
Firebase.getString(fbdo, "/house1/gate1"); String fireBaseMsg1 = String(fbdo.stringData()); String gate2 = fireBaseMsg1.substring(2,4); Serial.println(gate2); 
if(gate == "on") 
{ 
digitalWrite(lamp,HIGH); 
} 
else 
{ 
digitalWrite(lamp,LOW); 
} 
if(gate1=="on"){ 
 digitalWrite(lamp2,HIGH); 
 } 
 else{ 
 digitalWrite(lamp2,LOW); 
 } 
Firebase.getString(fbdo, "/house1/clear"); String temp = String(fbdo.stringData()); String secure_clear = temp.substring(2,3); Serial.println(secure_clear); 
bool LINE_Notify(String message) { 
WiFiClientSecure client; 
if (!client.connect("notify-api.line.me", 443)) { Serial.println("connection failed"); 
return false; 
}
String payload = "message=" + message; 
String req = ""; 
req += "POST /api/notify HTTP/1.1\r\n"; 
req += "Host: notify-api.line.me\r\n"; 
req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n"; req += "User-Agent: ESP32\r\n"; 
req += "Content-Type: application/x-www-form-urlencoded\r\n"; req += "Content-Length: " + String(payload.length()) + "\r\n"; req += "\r\n"; 
req += payload; 
client.print(req); 
delay(20); 
long timeOut = millis() + 30000; 
while(client.connected() && timeOut > millis()) { 
if (client.available()) { 
String str = client.readString(); 
// Serial.print(str); 
} 
delay(10); 
} 
return timeOut > millis(); 
} 
Firebase.getString(fbdo, "/house1/clear"); 
String temp = String(fbdo.stringData()); 
String secure_clear = temp.substring(2,3); 
Serial.println(secure_clear); 
Firebase.setString(fbdo, "/house1/Secure",alert); 
} 
void setting()
{ 
alert = 0; 
} 
bool LINE_Notify(String message) { 
WiFiClientSecure client; 
if (!client.connect("notify-api.line.me", 443)) { 
Serial.println("connection failed"); 
return false; 
} 
String payload = "message=" + message; 
String req = ""; 
req += "POST /api/notify HTTP/1.1\r\n"; 
req += "Host: notify-api.line.me\r\n"; 
req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n"; 
req += "User-Agent: ESP32\r\n"; 
req += "Content-Type: application/x-www-form- 
