#include <BluetoothSerial.h>

BluetoothSerial Serial_BT;
String command = "";

// Motor pins
int motor1Pin1 = 13;
int motor1Pin2 = 12;
int motor2Pin1 = 14;
int motor2Pin2 = 27;

// PWM settings
const int freq = 1000;
const int resolution = 8;

// PWM channels
const int ch1 = 0;
const int ch2 = 1;

// Speed control
int speedValue = 150;

// Timing for auto stop
unsigned long lastCommandTime = 0;

void setup() {
  Serial.begin(115200);

  // Bluetooth
  Serial_BT.begin("deeps");
  Serial.println("Bluetooth Turned on");

  // Motor pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // ✅ Attach PWM channels to pins
  ledcAttachChannel(motor1Pin1, freq, resolution, ch1);
  ledcAttachChannel(motor2Pin1, freq, resolution, ch2);

  stopMotors();
}

void loop() {

  while (Serial_BT.available()) {
    char c = Serial_BT.read();

    if (c == '\n') {
      command.trim();
      command.toLowerCase();

      Serial.println(command);
      lastCommandTime = millis();
    } else {
      command += c;
    }
  }

  if (command == "forward") {
    moveForward(speedValue);
  }
  else if (command == "backward") {
    moveBackward(speedValue);
  }
  else if (command == "left") {
    turnLeft(speedValue);
  }
  else if (command == "right") {
    turnRight(speedValue);
  }
  else if (command == "stop") {
    stopMotors();
  }
  else if (command == "slow") {
    speedValue = 100;
  }
  else if (command == "medium") {
    speedValue = 180;
  }
  else if (command == "fast") {
    speedValue = 255;
  }

  // Auto stop
  if (millis() - lastCommandTime > 2000) {
    stopMotors();
  }
}

// -------- MOTOR FUNCTIONS --------

void moveForward(int spd) {
  ledcWrite(ch1, spd);
  digitalWrite(motor1Pin2, LOW);

  ledcWrite(ch2, spd);
  digitalWrite(motor2Pin2, LOW);
}

void moveBackward(int spd) {
  digitalWrite(motor1Pin1, LOW);
  ledcWrite(ch1, spd);

  digitalWrite(motor2Pin1, LOW);
  ledcWrite(ch2, spd);
}

void turnLeft(int spd) {
  digitalWrite(motor1Pin1, LOW);
  ledcWrite(ch1, spd);

  ledcWrite(ch2, spd);
  digitalWrite(motor2Pin2, LOW);
}

void turnRight(int spd) {
  ledcWrite(ch1, spd);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  ledcWrite(ch2, spd);
}

void stopMotors() {
  ledcWrite(ch1, 0);
  ledcWrite(ch2, 0);

  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin2, LOW);
}
