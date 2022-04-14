#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL SerialUSB
#else
#define SERIAL Serial
#endif

const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);
String command;

unsigned long previousMillis = 0;
long currentMillis;
long interval = 5000;
int currValue = 0;
int prevValue = 0;
int sensorValue = 0;
bool isHit = false;
int torqueSpeed = -1;
long homePos = 28000;

int prevSensorValue = 0;
long hittingCount = 0;
long hittingInterval = 400;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  canBusSetup();
  motorPositionMode(homePos);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();
  motorCheck();
  serialLoop();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println("Torque Mode");
    motorTorqueMode();
  }
}
