#include "thingProperties.h"
#include <LiquidCrystal_I2C.h>

// ultrasonic sensor
const int HEAD_TRIG = 2;
const int HEAD_ECHO = 3;
const int CHEST_TRIG = 4;
const int CHEST_ECHO = 5;
const int NO_ECHO = 350;

// passive buzzer
const int BUZZER_PIN = 6;
const int TONE = 1000; // change tone

// RGB LED
const int LED_R = 9;
const int LED_G = 10;
const int LED_B = 11;

// liquid crystal
LiquidCrystal_I2C lcd(0x27, 16, 2);

// local variable
const int SENSOR_INTERVAL  = 250;
const int CLOUD_INTERVAL   = 3000; // 3 seconds
const int ALERT_DELAY      = 10000; // wrong position more than this will triggered
const int OUT_FRAME_MARGIN = 50;
const int STATE_CONFIRM    = 4; // need to hold 4 sensor cycles

int local_head_distance;
int local_chest_distance;
int head_base_line  = 55; // default can changed
int chest_base_line = 60; // default can changed

bool calibrateRequested = false;

String candidateState = "";
int candidateCount = 0;

// time control variable
unsigned long now;
unsigned long previousRead;
unsigned long lastCloudSync;
unsigned long badPostureStartTime = 0;
bool isBadPosture = false;

void setup()
{
  Serial.begin(9600);
  delay(1500); // delay gives the chance to wait for a Serial Monitor

  pinMode(HEAD_TRIG,  OUTPUT);
  pinMode(HEAD_ECHO,  INPUT);
  pinMode(CHEST_TRIG, OUTPUT);
  pinMode(CHEST_ECHO, INPUT);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // no pin setup need for passive buzzer

  lcd.init();
  lcd.backlight();
  lcd.print("SPYNE Starting..");

  // Connecting to Arduino IOT Cloud
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  while (ArduinoCloud.connected() == 0) { // blink Blue while not connected
    ArduinoCloud.update(); // Keep background connection
    setColor(0, 0, 255); // Blink Blue
    delay(300);
    setColor(0, 0, 0);
    delay(300);
    Serial.print(".");
  }
  
  setColor(0, 255, 0);
  lcd.clear();
  postureState = "NOT SITTING";
  lcd.print("Connected!");
  delay(1000);
  lcd.clear();
}

// +-----------+
// | MAIN LOOP |
// +-----------+
void loop()
{
  ArduinoCloud.update();

  if (calibrateRequested) { // calibration
    calibrateRequested = false;
    runCalibration();
    calibrateTrigger = false;
  }

  now = millis();
  // read from sensor
  if (now - previousRead >= SENSOR_INTERVAL) {
    previousRead = now;
    local_head_distance = readFiltered(HEAD_TRIG, HEAD_ECHO);
    delay(30);
    local_chest_distance = readFiltered(CHEST_TRIG, CHEST_ECHO);

    classifyPosture();

    displayLocalFeedback();
  }

  // sync distance to cloud
  if (now - lastCloudSync >= CLOUD_INTERVAL) {
    lastCloudSync = now;
    headDistance = local_head_distance;
    chestDistance = local_chest_distance;
  }
}

void setColor(int r, int g, int b) {
  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);
}

int readUltrasonicSensor(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long pulseWidth = pulseIn(echo, HIGH, 25000); // timeout 25ms
  if (pulseWidth == 0) { // timeout
    return NO_ECHO;
  }
  return pulseWidth*0.0343/2.0;
}

int readFiltered(int trig, int echo) {
  int sum = 0;
  int count = 0;
  for (int i=0; i<3; i++) {
    int d = readUltrasonicSensor(trig, echo);
    if (d < NO_ECHO) { // ignore timeout
      sum += d;
      count++;
    }
    delay(15);
  }
  if (count == 0) return NO_ECHO;   // all 3 timed out
  return sum / count;
}

void runCalibration() {
  // count down 3 seconds and set head_base_line and chest_base_line
  int SAMPLES = 10;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sit Upright!");
  lcd.setCursor(0,1);
  lcd.print("Calibrate in: ");
  for (int i=3; i>0; i--) {
    ArduinoCloud.update(); // Keep background connection
    lcd.setCursor(14,1);
    lcd.print(i);
    setColor(0, 255, 255);
    tone(BUZZER_PIN, 400, 300); // change count down tone
    delay(300);
    setColor(0, 0, 0);
    delay(700);
  }
  tone(BUZZER_PIN, TONE, 100);

  long sumH = 0, sumC = 0;
  for (int s=0; s<SAMPLES; s++) {
    sumH += readUltrasonicSensor(HEAD_TRIG, HEAD_ECHO);
    delay(35);
    sumC += readUltrasonicSensor(CHEST_TRIG, CHEST_ECHO);
    delay(35);
  }

  head_base_line  = sumH / SAMPLES;
  chest_base_line = sumC / SAMPLES;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Saved Baseline!");
  lcd.setCursor(0, 1);
  lcd.print("H:"); 
  lcd.print(head_base_line);
  lcd.print("cm C:"); 
  lcd.print(chest_base_line); 
  lcd.print("cm");
  tone(BUZZER_PIN, TONE, 100);

  delay(2000);
  lcd.clear();
}

String computeState() {
  int tol = (toleranceDistance > 0) ? toleranceDistance : 6;

  bool headOutFrame  = local_head_distance  > (head_base_line  + OUT_FRAME_MARGIN);
  bool chestOutFrame = local_chest_distance > (chest_base_line + OUT_FRAME_MARGIN);

  bool headNormal  = abs(local_head_distance  - head_base_line)  <= tol;
  bool chestNormal = abs(local_chest_distance - chest_base_line) <= tol;

  bool headNear  = local_head_distance  < (head_base_line  - tol);
  bool chestNear = local_chest_distance < (chest_base_line - tol);

  bool headFar   = local_head_distance  > (head_base_line  + tol);
  bool chestFar  = local_chest_distance > (chest_base_line + tol);

  if (headOutFrame && chestOutFrame)        return "NOT SITTING";
  if (headNormal && chestNormal)            return "UPRIGHT";
  if (headNear && chestNear)                return "TOO CLOSE";
  if (headFar && chestFar)                  return "TOO FAR";
  if (headNear && chestNormal)              return "HEAD FORWARD";
  if (chestFar && (headNormal || headNear)) return "SLIDE / SLUMP";
  return "UNBALANCED";
}

void classifyPosture() {
  String raw = computeState();

  if (raw == postureState) {          // stable
    candidateCount = 0;
    candidateState = "";
  } else if (raw == candidateState) { // same new state again
    if (++candidateCount >= STATE_CONFIRM) {
      postureState = raw;
      candidateCount = 0;
      candidateState = "";
    }
  } else {                            // different state, start counting
    candidateState = raw;
    candidateCount = 1;
  }
}

void displayLocalFeedback() {
  static unsigned long lastBeepTime = 0;
  // row 1: head and chest distance
  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.print(local_head_distance);
  lcd.print("cm  ");
  lcd.setCursor(8, 0);
  lcd.print("C:");
  lcd.print(local_chest_distance);
  lcd.print("cm   ");

  // row 2: posture
  lcd.setCursor(0, 1);
  lcd.print(postureState);
  for (int i = postureState.length(); i < 16; i++) {
    lcd.print(" "); // clear old String
  }

  // RGB and passive buzzer control
  if (postureState == "UPRIGHT") {
    setColor(0, 255, 0); // green
    isBadPosture = false;
  } 
  else if (postureState == "NOT SITTING") {
    setColor(0, 0, 255); // blue
    isBadPosture = false;
  } 
  else { // else is bad posture: (Too Close, Too Far, Head Forward, Slide)
    if (!isBadPosture) {
      badPostureStartTime = millis();
      isBadPosture = true;
    }

    unsigned long badDuration = millis() - badPostureStartTime;
    if (badDuration < ALERT_DELAY) {
      setColor(255, 180, 0); // 1st warning: yellow RGB, no Buzzer
    } else {
      setColor(255, 0, 0);   // 2nd warning: red RGB, Buzzer sound
      if (millis() - lastBeepTime >= 1000) {
        lastBeepTime = millis();
        tone(BUZZER_PIN, TONE, 50);
      }
    }
  }
}

// +----------------+
// | CLOUD FUNCTION |
// +----------------+

void onCalibrateTriggerChange()  {
  if (calibrateTrigger) {
    calibrateRequested = true;
  }
}

void onToleranceDistanceChange()  {
  Serial.print("New Tolerance from Cloud: +/- ");
  Serial.print(toleranceDistance);
  Serial.println(" cm");
}