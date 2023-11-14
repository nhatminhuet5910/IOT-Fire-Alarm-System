#include <SoftwareSerial.h>

SoftwareSerial sim800Serial(9,10); // RX, TX

const String myphone = "0917565400";// Insert your phone !!! 
void setup() {
  Serial.begin(9600);
  sim800Serial.begin(9600);
  delay(1000);
  Serial.println("Initializing...");
  sim800Serial.println("AT");
  delay(1000);
    while (sim800Serial.available()) {
    char c = sim800Serial.read();
    Serial.println(c);
}
}
void loop() {
    Gsm_MakeSMS();
    while (sim800Serial.available()) {
    char c = sim800Serial.read();
    Serial.write(c);
  }
  delay(20000);
}

void Gsm_MakeSMS()
{
  sim800Serial.println("AT");
  delay(1000);
  // Đợi và đọc phản hồi từ module SIM800A
  sim800Serial.println("AT+CMGF=1"); // Đặt chế độ văn bản (text mode)
  delay(5000);
  
  sim800Serial.println("AT+CMGS=\"" + myphone + "\"");     // Lệnh gửi tín nhắn đến số điện thoại
  delay(5000); 
  Serial.println("Sending SMS..."); 
  sim800Serial.println("This is a test message from ESP32."); // Nội dung tin nhắn.        
  sim800Serial.println((char)26);            // Commit  
  delay(5000);                                       
}
