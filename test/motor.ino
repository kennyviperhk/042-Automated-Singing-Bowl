unsigned char canReadBuffer = 0;
unsigned char len = 0;

void canBusSetup() {

  while (CAN_OK != CAN.begin(CAN_1000KBPS)) {            // init can bus : baudrate = 500k
    SERIAL.println("CAN BUS Shield init fail");
    SERIAL.println(" Init CAN BUS Shield again");
    delay(100);
  }
  SERIAL.println("CAN BUS Shield init ok!");
}


void motorCheck() {
  unsigned char mecanumWheelMotor1[8];
  //===================
  mecanumWheelMotor1[0] = 0x9C;
  mecanumWheelMotor1[1] = 0x00;
  mecanumWheelMotor1[2] = 0x00;
  mecanumWheelMotor1[3] = 0x00;
  mecanumWheelMotor1[4] = 0x00;
  mecanumWheelMotor1[5] = 0x00;
  mecanumWheelMotor1[6] = 0x00;
  mecanumWheelMotor1[7] = 0x00;

  CAN.sendMsgBuf(0x141, 0, 8, mecanumWheelMotor1);
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&canReadBuffer, mecanumWheelMotor1);

    currValue = mecanumWheelMotor1[2];
    roundValue = mecanumWheelMotor1[3];

  }
  if (currValue > 0) {
    Serial.print("lowbyte: ");
    Serial.print(currValue);
    Serial.print("  highbyte: ");
    Serial.println(roundValue);
  }
  //motorLoop();
}

void motorLoop() {

  unsigned char buf1[8];
  //===================
  buf1[0] = 0xA1;
  buf1[2] = 0x00;
  buf1[3] = 0x00;
  buf1[4] = -6;
  buf1[5] = 0;
  buf1[6] = 0x00;
  buf1[7] = 0x00;

  CAN.sendMsgBuf(0x141, 0, 8, buf1);
}
