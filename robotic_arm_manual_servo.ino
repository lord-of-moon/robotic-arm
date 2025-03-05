#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3);  // Bluetooth Module HC-05 (RX=D2, TX=D3)

Servo servo1, servo2, servo3, servo4;
int angle1 = 90, angle2 = 0, angle3 = 160, angle4 = 90;  // Default positions
const int stepDelay = 10;  // Delay between steps for smooth motion

void setup() {
  Serial.begin(9600);
  BT.begin(9600);  // Bluetooth Baud Rate

  servo1.attach(5);   // Base
  servo2.attach(6);   // Arm 1
  servo3.attach(9);   // Arm 2
  servo4.attach(10);  // Gripper

  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);
  servo4.write(angle4);

  Serial.println("🔹 Bluetooth 4-Servo Control Ready...");
  Serial.println("🔹 Send commands: Axx, Bxx, Cxx, Dxx (e.g., A45 moves Base to 45°)");
  Serial.println("-------------------------------------------------");
}

void smoothMove(Servo &servo, int &currentAngle, int targetAngle) {
  targetAngle = constrain(targetAngle, 0, 180);
  if (currentAngle < targetAngle) {
    for (int pos = currentAngle; pos <= targetAngle; pos++) {
      servo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = currentAngle; pos >= targetAngle; pos--) {
      servo.write(pos);
      delay(stepDelay);
    }
  }
  currentAngle = targetAngle;
}

void loop() {
  if (BT.available()) {
    String command = BT.readString();  // Read full command (e.g., "A45")
    command.trim();  // Remove spaces or newlines

    if (command.length() < 2) return;  // Ignore invalid commands

    char servoID = command.charAt(0);  // First character (servo ID)
    int targetAngle = command.substring(1).toInt();  // Extract angle from string

    Serial.print("📡 Received Command: ");
    Serial.println(command);

    // Process servo movement based on the command
    switch (servoID) {
      case 'A':
        Serial.print("🔄 Moving Base (Servo 1) to ");
        Serial.print(targetAngle);
        Serial.println("°");
        smoothMove(servo1, angle1, targetAngle);
        break;

      case 'B':
        Serial.print("🔄 Moving Arm 1 (Servo 2) to ");
        Serial.print(targetAngle);
        Serial.println("°");
        smoothMove(servo2, angle2, targetAngle);
        break;

      case 'C':
        Serial.print("🔄 Moving Arm 2 (Servo 3) to ");
        Serial.print(targetAngle);
        Serial.println("°");
        smoothMove(servo3, angle3, targetAngle);
        break;

      case 'D':
        Serial.print("🔄 Moving Gripper (Servo 4) to ");
        Serial.print(targetAngle);
        Serial.println("°");
        smoothMove(servo4, angle4, targetAngle);
        break;

      default:
        Serial.println("⚠️ Invalid Command!");
        return;
    }

    // Print updated servo angles
    Serial.println("-------------------------------------------------");
    Serial.print("🎯 Servo Angles -> ");
    Serial.print("Base: "); Serial.print(angle1);
    Serial.print(" | Arm 1: "); Serial.print(angle2);
    Serial.print(" | Arm 2: "); Serial.print(angle3);
    Serial.print(" | Gripper: "); Serial.println(angle4);
    Serial.println("-------------------------------------------------");
  }
}
