
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
}; // you can find this written on the board of some Arduino Ethernets or shields

void ethernetSetup() {
  Ethernet.init(17);
  Ethernet.begin(mac, ip);
  Udp.begin(inPort);
  Serial.print("Local IP : ");
  Serial.println(Ethernet.localIP());
  Serial.print("OSC PORT : ");
  Serial.println(inPort);
}

void ethernetLoop() {
  OSCBundle bundleIN;
  int size;
  if ( (size = Udp.parsePacket()) > 0)
  {
    while (size--)
      bundleIN.fill(Udp.read());
    if (!bundleIN.hasError())
    {
      bundleIN.route("/hit" , oscHit);
      bundleIN.route("/position" , oscPos);
      bundleIN.route("/speed" , oscSpeed);

    } else {
      Serial.println("BUNDLE IN ERROR");
    }
  }
}

void oscHit(OSCMessage & msg, int addrOffset) {
  if (msg.isInt(0)) {
    String cmd = "hit " + String(msg.getInt(0));
    uint16_t value = msg.getInt(0);
    unsigned char high_byte = value >> 8;
    unsigned char low_byte = value & 0xFF;

    torqueCommand[5] = low_byte;;
    torqueCommand[6] = high_byte;

    Serial.println(cmd);

    motorHit();
  }
}

void oscPos(OSCMessage & msg, int addrOffset) {
  if (msg.isInt(0)) {
    String cmd = "position " + String(msg.getInt(0));
    uint32_t value = msg.getInt(0);
    unsigned char high_byte = value >> 24;
    unsigned char mid_byte2 = value >> 16;
    unsigned char mid_byte1 = value >> 8;
    unsigned char low_byte = value & 0xFF;

    positionCommand[5] = low_byte;;
    positionCommand[6] = mid_byte1;
    positionCommand[7] = mid_byte2;
    positionCommand[8] = high_byte;

    Serial.print(low_byte);
    Serial.print(" ");
    Serial.print(mid_byte1);
    Serial.print(" ");
    Serial.print(mid_byte2);
    Serial.print(" ");
    Serial.print(high_byte);
    Serial.print(" ");
    Serial.println(cmd);

    motorMove();
  }
}

void oscSpeed(OSCMessage & msg, int addrOffset) {
  if (msg.isInt(0)) {
    String cmd = "speed " + String(msg.getInt(0));
    uint16_t value = msg.getInt(0);
    unsigned char high_byte = value >> 8;
    unsigned char low_byte = value & 0xFF;

    speedCommand1[5] = low_byte;;
    speedCommand1[6] = high_byte;

    Serial.println(cmd);

    motorSpeed();
  }
}
