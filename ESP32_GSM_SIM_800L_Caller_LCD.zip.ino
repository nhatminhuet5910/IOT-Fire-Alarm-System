#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

SoftwareSerial mySerial (3,1);

static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

const int LED=16;
const int svibrate=15;//cam bien rung D8
const int shall=6;// cam bien hall D3
const int  slight =14 ;// cam bien anh sang D5

//boolean giatri_light =LOW;
//boolean giatri_vibrate=LOW;
//boolean  giatri_hall=LOW;

int val_light;
int val_vibrate;
int val_hall;

const String myphone = "0963892998";// nhap so dien thoai cua chu nha 
const int PWR_KEY =12; 
const int rx=3;
const int tx=2;



void Gsm_Power_On();     // Bat module Sim 800A
void Gsm_Init();         // Cau hinh Module Sim 800A
void Gsm_MakeCall();     // Ham goi dien
void Gsm_MakeSMS();     // Ham nhan tin canh bao
void loa(); // ham bat loa canh bao dang thay bang led 
void bat1()//bat he thong 
{
  Serial.println("\n bat canh bao chong trom ");
  digitalWrite(LED,HIGH);
  val_light=digitalRead(slight);
  val_vibrate=digitalRead(svibrate);
  val_hall=digitalRead(shall);
   Serial.println(val_light);
   Serial.println( val_vibrate);
   Serial.println(val_hall);
   Serial.print("\n");
  delay(100);
}
void tat1()//tat he thong 
{ 
  Serial.println("tat canh bao chong trom  ");
  digitalWrite(LED ,LOW);
  val_light=digitalRead(slight);
  val_vibrate=digitalRead(svibrate);
  val_hall=digitalRead(shall);
  Serial.print(val_light);
  Serial.print( val_vibrate);
  Serial.print(val_hall);
  Serial.print("\n");
  
}
void Gsm_Power_On()
{
  digitalWrite(PWR_KEY, HIGH);      // Du chan PWR_KEY len cao it nhat 1s 
  delay(1500);                      // o day ta de 1,5s
  digitalWrite(PWR_KEY, LOW);     // Du chan PWR_KEY xuong thap
  delay(10000);   
}
 
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
 
void Gsm_MakeSMS()
{
  Serial.println("AT+CMGS=\"" + myphone + "\"");     // Lenh gui tin nhan
  delay(5000);                      
  Serial.print("Canh bao an toan,co ke dot nhap : \n"); // Gui noi dung                  
  delay(5000);                                       
}
void loa ()
{
  digitalWrite(LED,HIGH);
  delay(100);
  digitalWrite(LED,LOW);
  delay(100);
}
void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(slight,INPUT);
  pinMode(svibrate,INPUT);
  pinMode(shall,INPUT);
  digitalWrite(LED,LOW);
  digitalWrite(PWR_KEY, LOW);
  pinMode(PWR_KEY, OUTPUT);
  delay(1000);     



  Gsm_Power_On();                         
  Gsm_Init();                                 
  Gsm_MakeCall(); 


// ket noi wifi
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  WiFi.begin("Nguyen Phuong Duy", "12345678");// nhap dia chi wifi va mat khau


  ss.begin(GPSBaud);
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();




  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  
}

void loop()
{
  val_light=digitalRead(slight);
  val_vibrate=digitalRead(svibrate);
  val_hall=digitalRead(shall);
   Serial.println(val_light);
   Serial.println( val_vibrate);
   Serial.println(val_hall);
   Serial.print("\n");
   delay(1000);
// xu ly du lieu cua GPS
   while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

//xu ly tin hieu dau vao tu cac cam bien 
  if(val_light==0 || val_vibrate==0 || val_hall==0)
         {
           loa();
           Gsm_MakeSMS();
           delay(20000);
           Gsm_MakeCall();
           delay(20000);
           }
  
}


void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
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
