#include <Crc16.h>

unsigned long previousMillis = 0;
const long interval = 1000;
bool isStringStart = false;
byte msg [] = {0x3E, 0x00, 0x01, 0x56, 0x02, 0x10, 0x27, 0x47, 0x4A, 0x00, 0x00};

int dataCount = 0;
byte buf[15];
String rec;
char bufStr[99] = "";

Crc16 crc;

void setup() {
  Serial1.begin (115200);              // vitesse com
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    long cmd = command.toInt();
    uint16_t value = cmd;
    unsigned char high_byte = value >> 8;
    unsigned char low_byte = value & 0xFF;
    msg[5] = low_byte;;
    msg[6] = high_byte;;


  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    //check CRC
    crc.clearCrc();
    unsigned short crcValue = crc.Modbus(msg, 0, 7);
    //Serial.print("Modbus crc = 0x");
    //Serial.println(crcValue, HEX);
    uint16_t CRCvalue = crcValue;
    unsigned char high_ByteCRC = CRCvalue >> 8;
    unsigned char low_ByteCRC = CRCvalue & 0xFF;
    msg[7] = low_ByteCRC;
    msg[8] = high_ByteCRC;

    Serial.println();
    Serial.print("S : ");
    for (int i = 0; i < sizeof(msg); i++) {
      Serial.print(msg[i], HEX);          // console print the data
      Serial1.write(msg[i]);                // flip display print
      if (i < sizeof(msg) - 1) {
        Serial.print(" ");             // print a comma between data for console only :art:
      }
    }
    Serial.println();
  }

  if (Serial1.available()) {

    byte b = Serial1.read();
    //Serial.print(b, HEX);
    //Serial.print(" ");
    buf[dataCount] = b;

    Serial1.flush();

    dataCount++;
    if (dataCount >= 15) {
      dataCount = 0;
      String low_Byte = String(buf[11]);
      String high_Byte = String(buf[12]);
      String combinedMsg = high_Byte + low_Byte;
      uint16_t value = combinedMsg.toInt();
      Serial.print("Speed : ");
      Serial.println(value, DEC);
    }
  }
}
