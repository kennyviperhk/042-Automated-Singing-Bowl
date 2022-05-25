#include <Crc16.h>

unsigned long previousMillis = 0;
const long interval = 1000;
bool isStringStart = false;
byte msg [] = {0x3E, 0x00, 0x01, 0x56, 0x02, 0xe8, 0x03, 0x47, 0x4A, 0x00, 0x00};

byte msg2 [] = {0x3E, 0x00, 0x01, 0x56, 0x02, 0xe8, 0x03};

Crc16 crc;

void setup() {
  Serial1.begin (115200);              // vitesse com
  Serial.begin(115200);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    Serial.print("S : ");
    for (int i = 0; i < sizeof(msg); i++) {
      Serial.print(msg[i], HEX);          // console print the data
      Serial1.write(msg[i]);                // flip display print
      if (i < sizeof(msg) - 1) {
        Serial.print(" ");             // print a comma between data for console only :art:
      }
    }
    Serial.println();
    crc.clearCrc();
    unsigned short value;
    value = crc.Modbus(msg2, 0, sizeof(msg2));
    Serial.print("Modbus crc = 0x");
    Serial.println(value, HEX);
    String cmd = String(value);
    Serial.println(cmd.substring(0, 2), HEX);
  }

  while (Serial1.available()) {
    char inChar = (char)Serial.read();
    if (!isStringStart) {
      Serial.print("R : ");
      isStringStart = true;
    }
    Serial.print(Serial1.read(), HEX);
    Serial.print(" ");
  }
  if (isStringStart) {
    Serial.println();
    isStringStart = false;
  }
}
