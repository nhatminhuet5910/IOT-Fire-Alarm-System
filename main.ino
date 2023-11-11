//=================== Define and Include Library======================//
#include <Wire.h>
#include <Adafruit_MLX90614.h>
// Thư viện của cảm biến nhiệt không tiếp xúc.
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); 
//====================== Declare variables============================//
volatile  int smokeValue; // Gia tri cam bien khoi
volatile  int tempValue;// Gia tri cam bien nhiet 
const int smokePin = A0;  // Chân nối cảm biến khói với chân Analog 0
//====================== Declare variables============================//
void setup() {
  Serial.begin(9600); 
  mlx.begin();  
}

void loop() {
  int smokeValue,tempValue;
  readSensor(smokeValue,tempValue);
// In giá trị đọc được thu được từ cảm biến.
  Serial.print("Giá trị đọc từ cảm biến khói: ");
  Serial.println(smokeValue);
  // Kiểm tra nếu giá trị đọc vượt qua một ngưỡng nào đó
  if (smokeValue > 600) {
  Serial.println("Phát hiện khói!");
  }
  Serial.print("Temp_Object = "); 
  Serial.print(tempValue); 
  Serial.println("*C");
  delay(1000);  // Đợi 1 giây trước khi đọc lại giá trị từ cảm biến
}
void readSensor(int& smokeValue, int& tempValue) {
  // Đọc giá trị từ cảm biến khói
  smokeValue = analogRead(smokePin);
  tempValue =mlx.readObjectTempC();
}
