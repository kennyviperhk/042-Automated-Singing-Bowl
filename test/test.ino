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
String lastCommand;
unsigned long previousMillis = 0;
long currentMillis;
long interval = 10;
int currValue = 0;
int roundValue = 0;
long relativeValue = 0;
long preValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  canBusSetup();
  motorLoop();
}

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    motorCheck();
  }
}
