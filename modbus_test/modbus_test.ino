#include <Crc16.h>
#include <Wire.h>
#include <AccelStepper.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCBundle.h>
#include <SoftwareSerial.h>

EthernetUDP Udp;

// Slider 102 - 104
int dd = 111;

SerialPIO serialOut1( 3, 4 );
SerialPIO serialOut2( 7, SerialPIO::NOPIN );

//the Arduino's IP
IPAddress ip(192, 168, 8, dd);
IPAddress outIp(192, 168, 8, 255);

//port numbers
const unsigned int inPort = 10000 + dd;
const unsigned int outPort = 20000;

unsigned long previousMillis = 0;
unsigned long previousCheckMillis = 0;
const long interval = 500;
const long checkInterval = 10;
bool isStringStart = false;
byte torqueCommand [] = {0x3E, 0x00, 0x01, 0x54, 0x02, 0xD0, 0x07, 0x00, 0x00, 0x00, 0x00};
byte positionCommand [] = {0x3E, 0x00, 0x01, 0x55, 0x04, 0x74, 0x00, 0x00, 0x27, 0x00, 0x00};
byte moveCommand [] = {0x3E, 0x00, 0x01, 0x56, 0x02, 0x30, 0xF8, 0x00, 0x00, 0x00, 0x00};
byte checkCommand [] = {0x3E, 0x00, 0x01, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00};
byte speedCommand1 [] = {0x3E, 0x00, 0x01, 0x54, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int dataCount = 0;
byte buf[15];
String rec;
char bufStr[99] = "";
bool isGoingBack = true;
bool torqueBack = false;
bool isTorqueMode = false;
int prevValue = 0;
long currentMillis;

bool debugMode = false;

Crc16 crc;

void setup() {
  Serial.begin(115200);
  serialOut1.begin(115200);
  serialOut2.begin(115200);
  delay(5000);
  ethernetSetup();
  delay(2000);
  motorSpeed();
  Serial.println("Start");
}

void loop() {
  currentMillis = millis();

  ethernetLoop();
  serialLoop();
  motorCheck();
}
