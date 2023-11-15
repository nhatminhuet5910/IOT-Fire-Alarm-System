//====================Define and include library====================//
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();     // Library temp sensor.
SoftwareSerial mp3(6,7);                         // Define Rx_pin Tx_pin.
//====================Smoke sensor and Temp sensor====================//
volatile  int smokeValue;                        // Smoke value.
volatile  int tempValue;                         // Temp value.
const int smokePin = A0;                         // Connect smoke sensor to A0.
//====================MP3 Player====================//
static int8_t Send_buf[8] = {0};                 // Buffer to send commands. 
static uint8_t ansbuf[10] = {0};                 // Buffer to receive response.  

#define CMD_SEL_DEV           0X09               // Select device.
#define DEV_TF                0X02               // 02 for TF card.
#define CMD_PLAY              0X0D               // Play music.
 
void setup(){

  Serial.begin(9600);
  mp3.begin(9600);
  mlx.begin();
  delay(500);
  Serial.println();
  Serial.println("Select storage device to TF card.");
  sendCommand(CMD_SEL_DEV, 0, DEV_TF);
  delay(500); 
  Serial.println();
}

void loop(){
  
  int smokeValue,tempValue;
  readSensor(smokeValue,tempValue);
  //====================Check the value of the sensor====================//
  if(smokeValue > 600){
    sendCommand(CMD_PLAY);                // The value of the smoke sensor is greater than 600 -> play music.
  }
  if(tempValue > 50){
    sendCommand(CMD_PLAY);                // The value of the tempe sensor is greater than 50 -> play music.
  }
}

void readSensor(int& smokeValue, int& tempValue) {
  //====================Read sensor value====================//
  smokeValue = analogRead(smokePin);
  tempValue = mlx.readObjectTempC();
}

void sendCommand(byte command){
  sendCommand(command, 0, 0);
}

void sendCommand(byte command, byte dat1, byte dat2){
  delay(20);
  Send_buf[0] = 0x7E;                           // Every command should start with $(0x7E).
  Send_buf[1] = 0xFF;                           // Version information.
  Send_buf[2] = 0x06;                           // The number of bytes of the command without starting byte and ending byte.
  Send_buf[3] = command;                        // Such as PLAY and PAUSE and so on.
  Send_buf[4] = 0x01;                           // 0x00 = not feedback, 0x01 = feedback.
  Send_buf[5] = dat1;                           // data1.
  Send_buf[6] = dat2;                           // data2.
  Send_buf[7] = 0xEF;                           // Ending byte of the command.
  
  Serial.print("Sending: ");
  
  for (uint8_t i = 0; i < 8; i++){
    mp3.write(Send_buf[i]) ;
    Serial.print(sbyte2hex(Send_buf[i]));
  }
  
  Serial.println();
}

String sbyte2hex(uint8_t b){
  String shex;

  shex = "0X";

  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}

//====================For hex to integer conversion====================//
int shex2int(char *s, int n){
  int r = 0;
  for (int i=0; i<n; i++){
     if(s[i]>='0' && s[i]<='9'){
      r *= 16; 
      r +=s[i]-'0';
     }else if(s[i]>='A' && s[i]<='F'){
      r *= 16;
      r += (s[i] - 'A') + 10;
     }
  }
  return r;
}                     

String sanswer(void){
  uint8_t i = 0;
  String mp3answer = ""; 

  //====================Get only 10 Bytes====================//
  while (mp3.available() && (i < 10)){
    uint8_t b = mp3.read();
    ansbuf[i] = b;
    i++;

    mp3answer += sbyte2hex(b);
  }

  //====================if the answer format is correct====================//
  if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF)){
    return mp3answer;
  }

  return "???: " + mp3answer;
}