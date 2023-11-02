#include <SoftwareSerial.h>

SoftwareSerial sim800Serial(1,3); // RX, TX

void setup() {
  Serial.begin(9600);
  sim800Serial.begin(9600);

  delay(1000);

  Serial.println("Initializing...");
  sim800Serial.println("AT");
  delay(1000);
while (sim800Serial.available()) {
    char c = sim800Serial.read();
    Serial.print(c);
}
}

void loop() {
  // Gửi lệnh AT để kiểm tra kết nối với module SIM800A
  sim800Serial.println("AT");
  delay(1000);

  // Đợi và đọc phản hồi từ module SIM800A
    sim800Serial.println("AT+CMGF=1"); // Đặt chế độ văn bản (text mode)
  delay(5000);

  Serial.println("Sending SMS...");
  sim800Serial.println("AT+CMGS=\"0917565400\""); // Thay thế bằng số điện thoại thực
  delay(5000);

  sim800Serial.println("This is a test message from ESP32.");
  sim800Serial.write(26); // Gửi Ctrl+Z (End of Message)
  delay(5000);
    while (sim800Serial.available()) {
    char c = sim800Serial.read();
    Serial.write(c);
  }
}
