
void setup() {
  Serial.begin(115200);      // Khởi tạo kết nối với Serial Monitor
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println(data);
  }
  delay(100);
}
