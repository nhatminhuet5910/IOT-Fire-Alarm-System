// #include <ESP8266WiFi.h>
// #include <WiFiClient.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

SoftwareSerial mySerial (3,1);

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);



const String myphone = "0963892998";// nhap so dien thoai cua chu nha 
const int rx=0;
const int tx=1;


void Gsm_Init();         // Cau hinh Module Sim 800A
void Gsm_MakeCall();     // Ham goi dien
void Gsm_MakeSMS();     // Ham nhan tin canh bao


void Gsm_Init()
{
  Serial.println("ATE0");           // Tat che do phan hoi (Echo mode)
  delay(2000);
  Serial.println("AT+IPR=9600");    // Dat toc do truyen nhan du lieu 9600 bps
  delay(2000);
  Serial.println("AT+CMGF=1");     // Chon che do TEXT Mode
  delay(2000);
  Serial.println("AT+CLIP=1");     // Hien thi thong tin nguoi goi den
  delay(2000);
  Serial.println("AT+CNMI=2,2");   // Hien thi truc tiep noi dung tin nhan
  delay(2000);
}
 
void Gsm_MakeCall()           
{
  Serial.println("ATD" + myphone + ";");       // Goi dien 
  delay(15000);                                // Sau 15s
  Serial.println("ATH");                       // Ngat cuoc goi
  delay(2000);
}
 
void Gsm_MakeSMS(String location_GPS)
{

  Serial.println("AT+CMGS=\"" + myphone + "\"");     // Lenh gui tin nhan
  delay(5000);                      
  Serial.print(location_GPS); // Gui noi dung                  
  delay(5000);                                       
}

void setup()
{                     
  Gsm_Init();                                 
  Gsm_MakeCall(); 
  Gsm_MakeSMS();

// ket noi wifi
  Serial.begin(9600);
  Serial.println();
  // Serial.println();
  // Serial.print("Connecting to ");
  // WiFi.begin("Nguyen Phuong Duy", "12345678");// nhap dia chi wifi va mat khau


  ss.begin(GPSBaud);
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();




  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.println("WiFi connected"); 
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());
  
  
}

void loop()
{

String GPS_1;
// xu ly du lieu cua GPS
   while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo( GPS_1);

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

//xu ly tin hieu dau vao tu cac cam bien 
 
         String location_GPS = GPS_1;
         
           Gsm_MakeSMS(location_GPS);
           delay(20000);
           Gsm_MakeCall();
           delay(20000);
           
  
}


void displayInfo(String GPS_1)
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {

    GPS_1 = gps.location.lat();
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
