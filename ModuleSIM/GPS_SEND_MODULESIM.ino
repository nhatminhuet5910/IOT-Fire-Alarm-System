#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial gpsSerial(2,3); // RX, TX
TinyGPSPlus gps;
SoftwareSerial sim800Serial(10,11); // RX, TX
const String myphone = "0843342998";// Insert your phone !!! 

void setup() {
  sim800Serial.begin(9600);
  gpsSerial.begin(9600);
  Serial.begin(9600);
  delay(1000);
   while (sim800Serial.available()) {
    char c = sim800Serial.read();
    Serial.println(c);
}
}
void loop() {
  // Đọc dữ liệu từ GPS
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      // Lấy thông tin vị trí
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();
      // Gửi tin nhắn chứa thông tin vị trí qua SIM800A
      sendLocationSMS(latitude, longitude);
      // In thông tin vị trí lên Serial Monitor
      Serial.print("Latitude: ");
      Serial.println(latitude, 6);
      Serial.print("Longitude: ");
      Serial.println(longitude, 6);
    }
  }
}
void sendLocationSMS(float latitude, float longitude) {
  sim800Serial.println("AT");
  sim800Serial.println("AT+CMGF=1"); // Đặt chế độ văn bản (text mode)
  // Gửi lệnh AT để thiết lập chế độ vị trí GPS
  sim800Serial.println("AT+CMGS=\"" + myphone + "\"");// Lệnh gửi tin nhắn đến số điện thoại
  delay(10000);
  sim800Serial.println("GPS Location:");
  sim800Serial.println("Latitude: " + String(latitude, 6));
  sim800Serial.println("Longitude: " + String(longitude, 6));
  sim800Serial.write(26); // Gửi ký tự kết thúc (Ctrl+Z)
  Serial.println("Sent Successfully !!! ...");
  delay(10000);
}
