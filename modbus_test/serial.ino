void serialLoop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    if (command.equals("d")) {
      if (!debugMode) {
        Serial.println("Debug Mode On");
        debugMode = true;
      } else {
        Serial.println("Debug Mode Off");
        debugMode = false;
      }
    }
  }
}
