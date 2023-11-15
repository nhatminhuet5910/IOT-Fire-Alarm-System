String a = "Ngày hôm nay của em thế nào ? \n ";
void setup() {
  Serial.begin(115200); // Khởi tạo kết nối với Serial Monitor
  
}

void loop() {
  Serial.println(a); // Gửi dữ liệu từ Arduino đến ESP8266
  delay(1000);
  }