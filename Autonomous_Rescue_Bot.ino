#include <BluetoothSerial.h>

BluetoothSerial SerialBT;


#define MOTOR_FREQ 20000
#define SERVO_FREQ 50

#define RPWM1  5
#define LPWM1  4
#define R_EN1  14
#define L_EN1  12

#define RPWM2  19
#define LPWM2  18
#define R_EN2  21
#define L_EN2  22


#define RPWM3  13  
#define LPWM3  27   
#define R_EN3  25
#define L_EN3  26

#define SERVO_PIN 32  // ✅ was 27

int speedValue = 150;
int diff = 4;

int servoAngle = 90;
bool servoUpRun = false;
bool servoDownRun = false;

bool newMotorLeftRun  = false;
bool newMotorRightRun = false;
char currentDriveCmd = 'S';
int extraMotorDrive = 0;

unsigned long lastServoMove = 0;
const int servoInterval = 20;

/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Car");

  pinMode(R_EN1, OUTPUT); pinMode(L_EN1, OUTPUT);
  pinMode(R_EN2, OUTPUT); pinMode(L_EN2, OUTPUT);
  pinMode(R_EN3, OUTPUT); pinMode(L_EN3, OUTPUT);

  digitalWrite(R_EN1, HIGH); digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH); digitalWrite(L_EN2, HIGH);
  digitalWrite(R_EN3, HIGH); digitalWrite(L_EN3, HIGH);

  ledcAttach(RPWM1, MOTOR_FREQ, 8);
  ledcAttach(LPWM1, MOTOR_FREQ, 8);
  ledcAttach(RPWM2, MOTOR_FREQ, 8);
  ledcAttach(LPWM2, MOTOR_FREQ, 8);
  ledcAttach(RPWM3, MOTOR_FREQ, 8);
  ledcAttach(LPWM3, MOTOR_FREQ, 8);

  ledcAttach(SERVO_PIN, SERVO_FREQ, 16);
  setServoAngle(servoAngle);

  Serial.println("ESP32 READY ✅");
}

/* ================= LOOP ================= */
void loop() {

  if (SerialBT.hasClient() && SerialBT.available()) {
    char cmd = SerialBT.read();
    Serial.println(cmd);   

    if (cmd >= '0' && cmd <= '9') {
      speedValue = map(cmd - '0', 0, 9, 60, 255);
    } else {
      controlCar(cmd);
    }
  }

  /* ===== CONTINUOUS NEW DC MOTOR ===== */
  if (newMotorLeftRun) {
    newMotorLeft();
  }
  if (newMotorRightRun) {
    newMotorRight();
  }

  /* ===== CONTINUOUS SERVO ===== */
  unsigned long now = millis();
  if (now - lastServoMove >= servoInterval) {
    lastServoMove = now;

    if (servoUpRun) {
      servoAngle++;
      setServoAngle(servoAngle);
    }

    if (servoDownRun) {
      servoAngle--;
      setServoAngle(servoAngle);
    }
  }
}

/* ================= COMMAND HANDLER ================= */
void controlCar(char c) {
  switch (c) {

    /* ---- NEW DC MOTOR ---- */
    case 'W':   // LEFT
      newMotorLeftRun  = true;
      newMotorRightRun = false;
      break;

    case 'U':   // RIGHT
      newMotorRightRun = true;
      newMotorLeftRun  = false;
      break;

    case 'w':
    case 'u':   // STOP
      newMotorLeftRun  = false;
      newMotorRightRun = false;
      stopNewMotor();
      break;

    /* ---- SERVO ---- */
    case 'V':
      servoUpRun = true;
      servoDownRun = false;
      break;

    case 'v':
      servoUpRun = false;
      break;

    case 'X':
      servoDownRun = true;
      servoUpRun = false;
      break;

    case 'x':
      servoDownRun = false;
      break;

    /* ---- CAR ---- */
    case 'F': moveForward(); break;
    case 'B': moveBackward(); break;
    case 'L': turnLeft(); break;
    case 'R': turnRight(); break;
    case 'S': stopAll(); break;

    case 'G': moveLeftForward(); break;
    case 'I': moveRightForward(); break;
    case 'H': moveLeftBackward(); break;
    case 'J': moveRightBackward(); break;
  }
}

/* ================= SERVO ================= */
void setServoAngle(int angle) {
  servoAngle = constrain(angle, 0, 180);
  uint32_t duty = map(servoAngle, 0, 180, 1638, 8192);
  ledcWrite(SERVO_PIN, duty);
}

/* ================= MOTOR HELPERS ================= */
void setExtraMotor(int speed) {
  if (speed >= 0) {
    ledcWrite(RPWM3, speed);
    ledcWrite(LPWM3, 0);
  } else {
    ledcWrite(RPWM3, 0);
    ledcWrite(LPWM3, -speed);
  }
}

void setDriveMotors(int m1Speed, int m2Speed) {
  if (m1Speed >= 0) {
    ledcWrite(RPWM1, m1Speed);
    ledcWrite(LPWM1, 0);
  } else {
    ledcWrite(RPWM1, 0);
    ledcWrite(LPWM1, -m1Speed);
  }

  if (m2Speed >= 0) {
    ledcWrite(RPWM2, m2Speed);
    ledcWrite(LPWM2, 0);
  } else {
    ledcWrite(RPWM2, 0);
    ledcWrite(LPWM2, -m2Speed);
  }
}

/* ================= EXTRA MOTOR ================= */
void newMotorLeft() {
  ledcWrite(RPWM3, 0);
  ledcWrite(LPWM3, speedValue);
}

void newMotorRight() {
  ledcWrite(RPWM3, speedValue);
  ledcWrite(LPWM3, 0);
}

void stopNewMotor() {
  ledcWrite(RPWM3, 0);
  ledcWrite(LPWM3, 0);
}

/* ================= CAR MOVEMENT ================= */
void moveForward() {
  ledcWrite(RPWM1, speedValue);
  ledcWrite(LPWM1, 0);
  ledcWrite(RPWM2, speedValue);
  ledcWrite(LPWM2, 0);
}

void moveBackward() {
  ledcWrite(RPWM1, 0);
  ledcWrite(LPWM1, speedValue);
  ledcWrite(RPWM2, 0);
  ledcWrite(LPWM2, speedValue);
}

void turnLeft() {
  ledcWrite(RPWM1, speedValue);
  ledcWrite(LPWM1, 0);
  ledcWrite(RPWM2, 0);
  ledcWrite(LPWM2, speedValue);
}

void turnRight() {
  ledcWrite(RPWM1, 0);
  ledcWrite(LPWM1, speedValue);
  ledcWrite(RPWM2, speedValue);
  ledcWrite(LPWM2, 0);
}

void moveLeftForward() {
  ledcWrite(RPWM1, speedValue);
  ledcWrite(LPWM1, 0);
  ledcWrite(RPWM2, speedValue / diff);
  ledcWrite(LPWM2, 0);
}

void moveRightForward() {
  ledcWrite(RPWM1, speedValue / diff);
  ledcWrite(LPWM1, 0);
  ledcWrite(RPWM2, speedValue);
  ledcWrite(LPWM2, 0);
}

void moveLeftBackward() {
  ledcWrite(RPWM1, 0);
  ledcWrite(LPWM1, speedValue);
  ledcWrite(RPWM2, 0);
  ledcWrite(LPWM2, speedValue / diff);
}

void moveRightBackward() {
  ledcWrite(RPWM1, 0);
  ledcWrite(LPWM1, speedValue / diff);
  ledcWrite(RPWM2, 0);
  ledcWrite(LPWM2, speedValue);
}

void stopAll() {
  ledcWrite(RPWM1, 0);
  ledcWrite(LPWM1, 0);
  ledcWrite(RPWM2, 0);
  ledcWrite(LPWM2, 0);
  stopNewMotor();
}
