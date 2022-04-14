bool isDecode = false;

void serialLoop() {
  if (Serial.available() > 0) {

    // get incoming byte:
    int inChar = Serial.read();
    char_decode(inChar) ;
    //Serial.println(inChar);
  }
}

void char_decode(int inChar) {
  if (inChar != '|' && inChar != '\n')
  {
    isDecode = true;
    // convert the incoming byte to a char
    // and add it to the string:
    command += (char)inChar;
  }
  else if (inChar == '|' || inChar == '\n')
  {
    isDecode = false;
  }

  if (!isDecode)
  {
    String subCommand[2];

    subCommand[0] = command.substring(0, 1);

    subCommand[1] = command.substring(1, command.length());

    Serial.println("received :" + command);
    if (subCommand[0] == "T" || subCommand[0] == "t") {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        Serial.println("Torque Mode");
        motorTorqueMode();
      }
    } else if (subCommand[0] == "m" || subCommand[0] == "M") {
      int pos = subCommand[1].toInt();
      long posToDegree = map(pos, 0, 360, 0, 35999);
      Serial.print("Move tO ");
      Serial.println(posToDegree);
      motorPositionMode(posToDegree);
    }
    command = "";
  }
}
