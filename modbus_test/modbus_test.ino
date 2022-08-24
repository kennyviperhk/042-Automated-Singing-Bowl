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
int dd = 102;


//the Arduino's IP
IPAddress ip(192, 168, 8, dd);
IPAddress outIp(192, 168, 8, 255);

//port numbers
const unsigned int inPort = 10000 + dd;
const unsigned int outPort = 20000;

unsigned long previousMillis = 0;
unsigned long previousCheckMillis = 0;
const long interval = 3000;
const long checkInterval = 3;
bool isStringStart = false;
byte torqueCommand [] = {0x3E, 0x00, 0x02, 0x54, 0x02, 0xD0, 0x07, 0x00, 0x00, 0x00, 0x00};
byte moveCommand [] = {0x3E, 0x00, 0x02, 0x56, 0x02, 0x30, 0xF8, 0x00, 0x00, 0x00, 0x00};
byte checkCommand [] = {0x3E, 0x00, 0x02, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00};
byte speedCommand1 [] = {0x3E, 0x00, 0x01, 0x54, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int dataCount = 0;
byte buf[15];
String rec;
char bufStr[99] = "";
bool isGoingBack = true;
int prevValue = 0;
long currentMillis;

Crc16 crc;

void setup() {
  Serial1.begin(115200);              // vitesse com
  Serial.begin(115200);
  ethernetSetup();
  delay(1000);
  motorMove();
  Serial.println("Start");
}

void loop() {

  currentMillis = millis();

  ethernetLoop();
  motorCheck();
}
