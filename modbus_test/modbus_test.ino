#include <Crc16.h>

unsigned long previousMillis = 0;
unsigned long previousCheckMillis = 0;
const long interval = 1000;
const long checkInterval = 10;
bool isStringStart = false;
byte torqueCommand [] = {0x3E, 0x00, 0x01, 0x53, 0x02, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00};
byte moveCommand [] = {0x3E, 0x00, 0x01, 0x56, 0x02, 0x30, 0xF8, 0x00, 0x00, 0x00, 0x00};
byte checkCommand [] = {0x3E, 0x00, 0x01, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00};
int dataCount = 0;
byte buf[15];
String rec;
char bufStr[99] = "";
bool isGoingBack = true;

Crc16 crc;

void setup() {
  Serial1.begin (115200);              // vitesse com
  Serial.begin(115200);
}

void loop() {

  unsigned long currentMillis = millis();
  if (Serial.available()) {
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      String command = Serial.readStringUntil('\n');
      long cmd = command.toInt();
      uint16_t value = cmd;
      unsigned char high_byte = value >> 8;
      unsigned char low_byte = value & 0xFF;
      torqueCommand[5] = low_byte;;
      torqueCommand[6] = high_byte;
      Serial.print("0x");
      Serial.print(low_byte, HEX);
      Serial.print(" 0x");
      Serial.println(high_byte, HEX);


      //check CRC
      crc.clearCrc();
      unsigned short crcValue = crc.Modbus(torqueCommand, 0, sizeof(torqueCommand) - 4);
      //Serial.print("Modbus crc = 0x");
      //Serial.println(crcValue, HEX);
      uint16_t CRCvalue = crcValue;
      unsigned char high_ByteCRC = CRCvalue >> 8;
      unsigned char low_ByteCRC = CRCvalue & 0xFF;
      torqueCommand[7] = low_ByteCRC;
      torqueCommand[8] = high_ByteCRC;

      //Serial.println();
      //Serial.print("S : ");
      for (int i = 0; i < sizeof(torqueCommand); i++) {
        //Serial.print(torqueCommand[i], HEX);          // console print the data
        Serial1.write(torqueCommand[i]);                // flip display print
        if (i < sizeof(torqueCommand) - 1) {
          //Serial.print(" ");             // print a comma between data for console only :art:
        }
      }
      //Serial.println();

      isGoingBack = false;
    }
  }


  if (currentMillis - previousCheckMillis >= checkInterval) {
    // save the last time you blinked the LED
    previousCheckMillis = currentMillis;
    crc.clearCrc();
    unsigned short crcValue = crc.Modbus(checkCommand, 0, sizeof(checkCommand) - 4);
    //Serial.print("Modbus crc = 0x");
    //Serial.println(crcValue, HEX);
    uint16_t CRCvalue = crcValue;
    unsigned char high_ByteCRC = CRCvalue >> 8;
    unsigned char low_ByteCRC = CRCvalue & 0xFF;
    checkCommand[5] = low_ByteCRC;
    checkCommand[6] = high_ByteCRC;

    //Serial.println();
    //Serial.print("S : ");
    for (int i = 0; i < sizeof(checkCommand); i++) {
      //Serial.print(checkCommand[i], HEX);          // console print the data
      Serial1.write(checkCommand[i]);                // flip display print
      if (i < sizeof(checkCommand) - 1) {
        //Serial.print(" ");             // print a comma between data for console only :art:
      }
    }
    //Serial.println();
  }

  if (Serial1.available()) {
    byte b = Serial1.read();
    if (b == 60) {
      dataCount = 0;
    }
    //Serial.print(b, HEX);
    //Serial.print(" ");
    buf[dataCount] = b;
    Serial1.flush();
    dataCount++;
  }

  if (dataCount == 14) {
    dataCount = 0;
    if (buf[3] == 47) {
      String low_Byte = String(buf[11]);
      String high_Byte = String(buf[12]);
      String combinedmoveCommand = high_Byte + low_Byte;
      uint16_t value = combinedmoveCommand.toInt();
      //Serial.print("R : ");
      for (int i = 0; i < sizeof(buf); i++) {
        //Serial.print(buf[i], HEX);
        //Serial.print(" ");
      }
      //Serial.println();
      //Serial.print("Speed : ");
      //Serial.println(value);
      if ((value == 0 || value > 50000) && !isGoingBack) {
        Serial.println("Hit");
        //check CRC
        crc.clearCrc();
        unsigned short crcValue = crc.Modbus(moveCommand, 0, sizeof(moveCommand) - 4);
        uint16_t CRCvalue = crcValue;
        unsigned char high_ByteCRC = CRCvalue >> 8;
        unsigned char low_ByteCRC = CRCvalue & 0xFF;
        moveCommand[7] = low_ByteCRC;
        moveCommand[8] = high_ByteCRC;

        for (int i = 0; i < sizeof(moveCommand); i++) {
          Serial1.write(moveCommand[i]);                // flip display print
        }
        isGoingBack = true;
      }
    }
  }
}
