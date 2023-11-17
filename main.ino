//====================Define and include library====================//
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_MLX90614.h>
#include <TinyGPS++.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();     // Library Temp Sensor.
SoftwareSerial gpsSerial(2, 3); // RX, TX
SoftwareSerial mp3(6, 7);                       // Define Rx_pin Tx_pin.
SoftwareSerial sim800Serial(10, 11); // RX, TX
const String myphone = "0917565400";// Insert your phone !!!
//====================Smoke sensor and Temp sensor====================//
volatile  int smokeValue;                        // Smoke value.
volatile  int tempValue;                          // Temp value.
volatile  float a;                                // Value Latitude
volatile  float b;                                // Value Longitude
  String Data_ESP;
char a_buffer[10];
char b_buffer[10];
const int smokePin = A0;
TinyGPSPlus gps;
// Connect smoke sensor to A0.
//====================MP3 Player======================================//
static int8_t Send_buf[8] = {0};                 // Buffer to send commands.
static uint8_t ansbuf[10] = {0};                 // Buffer to receive response.

#define CMD_SEL_DEV           0X09               // Select device.
#define DEV_TF                0X02               // 02 for TF card.
#define CMD_PLAY              0X0D               // Play music.
void setup() {
  pinMode(4, OUTPUT); // Pin 4 to digitalWrite pin 12 - ESP32 Cam HIGH
  // Cần đặt đúng số thứ tự begin để tránh gây xung đột cổng.
  mp3.begin(9600);
  sim800Serial.begin(9600);
  gpsSerial.begin(9600);
  mlx.begin();
  Serial.begin(115200);
  delay(500);
  Serial.println();
  sendCommand(CMD_SEL_DEV, 0, DEV_TF);
  delay(500);
  while (sim800Serial.available()) {
    char c = sim800Serial.read();
    Serial.println(c);
  }
}
void loop() {
  int smokeValue, tempValue;
  readSensor(smokeValue, tempValue);
  Serial.println(tempValue);
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      // Lấy thông tin vị trí
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();
      // Gửi tin nhắn chứa thông tin vị trí qua SIM800A
      //sendLocationSMS(latitude, longitude);
      // In thông tin vị trí lên Serial Monitor
      a = latitude;
      b = longitude;
      // Convert Data to String to Send ESP32 send Email Captions.
      String GPS_ESP = ("Latitude: " + String(dtostrf(a, 4, 6, a_buffer)) + " ----- " + "Longitude:" + String(dtostrf(b, 4, 6, b_buffer))+ " Co chay !!!");
      Data_ESP = GPS_ESP;
    }
  }
  Serial.println(Data_ESP);
  //====================Check the value of the sensor====================//
  if (smokeValue > 600 || tempValue > 30) {
    digitalWrite(4, 1);
    delay(1000);
    digitalWrite(4, 0);
    sendCommand(CMD_PLAY);//The value of sensor is greater -> play music.
    sendLocationSMS(a, b); // Send GPS Location of Fire ware.
  }
}
void readSensor(int& smokeValue, int& tempValue) {
  //====================Read sensor value===============================//
  smokeValue = analogRead(smokePin);
  tempValue = mlx.readObjectTempC();
}
void sendCommand(byte command) {
  sendCommand(command, 0, 0);
}
void sendCommand(byte command, byte dat1, byte dat2) {
  delay(20);
  Send_buf[0] = 0x7E;                           // Every command should start with $(0x7E).
  Send_buf[1] = 0xFF;                           // Version information.
  Send_buf[2] = 0x06;                           // The number of bytes of the command without starting byte and ending byte.
  Send_buf[3] = command;                        // Such as PLAY and PAUSE and so on.
  Send_buf[4] = 0x01;                           // 0x00 = not feedback, 0x01 = feedback.
  Send_buf[5] = dat1;                           // Data1.
  Send_buf[6] = dat2;                           // Data2.
  Send_buf[7] = 0xEF;                           // Ending byte of the command.
  Serial.print("Sending: ");
  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(Send_buf[i]) ;
    Serial.print(sbyte2hex(Send_buf[i]));
  }
  Serial.println();
}
String sbyte2hex(uint8_t b) {
  String shex;
  shex = "0X";
  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}
//====================For hex to integer conversion====================//
int shex2int(char *s, int n) {
  int r = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] >= '0' && s[i] <= '9') {
      r *= 16;
      r += s[i] - '0';
    } else if (s[i] >= 'A' && s[i] <= 'F') {
      r *= 16;
      r += (s[i] - 'A') + 10;
    }
  }
  return r;
}
String sanswer(void) {
  uint8_t i = 0;
  String mp3answer = "";
  //====================Get only 10 Bytes====================//
  while (mp3.available() && (i < 10)) {
    uint8_t b = mp3.read();
    ansbuf[i] = b;
    i++;
    mp3answer += sbyte2hex(b);
  }
  //====================If the answer format is correct====================//
  if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF)) {
    return mp3answer;
  }
  return "???: " + mp3answer;
}
//===================================SendLocation=========================//
void sendLocationSMS(float latitude, float longitude) {
  sim800Serial.println("AT");
  sim800Serial.println("AT+CMGF=1"); // Đặt chế độ văn bản (text mode)
  // Gửi lệnh AT để thiết lập chế độ vị trí GPS
  sim800Serial.println("AT+CMGS=\"" + myphone + "\"");// Lệnh gửi tin nhắn đến số điện thoại
  delay(10000);
  sim800Serial.println("GPS Location Canh Bao Chay:");
  sim800Serial.println("Latitude: " + String(latitude, 6));
  sim800Serial.println("Longitude: " + String(longitude, 6));
  sim800Serial.write(26); // Gửi ký tự kết thúc (Ctrl+Z)
  Serial.println("Sent Successfully ...");
  delay(10000);
}
