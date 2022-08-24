void motorMove() {
  //check CRC
  crc.clearCrc();
  unsigned short crcValue = crc.Modbus(speedCommand1, 0, sizeof(speedCommand1) - 4);
  //Serial.print("Modbus crc = 0x");
  //Serial.println(crcValue, HEX);
  uint16_t CRCvalue = crcValue;
  unsigned char high_ByteCRC = CRCvalue >> 8;
  unsigned char low_ByteCRC = CRCvalue & 0xFF;
  speedCommand1[7] = low_ByteCRC;
  speedCommand1[8] = high_ByteCRC;

  for (int i = 0; i < sizeof(speedCommand1); i++) {
    Serial.print(speedCommand1[i], HEX);
    Serial.print(" ");
    Serial1.write(speedCommand1[i]);                // flip display print
  }
  Serial.println();
}

void motorTorque() {
  if ((currentMillis - previousMillis >= interval) && (isGoingBack)) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
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

void motorCheck() {
  if ((currentMillis - previousCheckMillis >= checkInterval) ) {
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
      String angleLow_Byte = String(buf[5]);
      String angleHigh_Byte = String(buf[6]);
      String combinedangleValue = angleHigh_Byte + angleLow_Byte;
      uint16_t angleValue = combinedangleValue.toInt();

      String low_Byte = String(buf[11]);
      String high_Byte = String(buf[12]);
      String combinedSpeedValue = high_Byte + low_Byte;
      uint16_t speedValue = combinedSpeedValue.toInt();
      //Serial.print("R : ");
      for (int i = 0; i < sizeof(buf); i++) {
        //Serial.print(buf[i], HEX);
        //Serial.print(" ");
      }
      //Serial.println();

      if ((angleValue > 59000) && (speedValue > 50000) && !isGoingBack) {
        Serial.print("Angle : ");
        Serial.print(angleValue);
        Serial.print(" Speed : ");
        Serial.println(speedValue);
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
