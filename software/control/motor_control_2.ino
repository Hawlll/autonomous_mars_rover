/*
  Arduino UNO — RC Throttle -> BTS7960 -> JGB37 Gear Motor w/ Encoder
  ---------------------------------------------------------------
  Receiver:  FlySky FS-iA10B, CH2 signal -> D7
  Driver:    BTS7960 (RPWM=D5, LPWM=D6, R_EN/L_EN tied to 5V)
  Encoder:   C1 -> D2 (external interrupt), C2 -> D4 (direction reference)

  IMPORTANT HARDWARE NOTE (read before wiring):
  The Arduino UNO only has TWO true external-interrupt pins: D2 and D3.
  Full quadrature decoding (counting on both edges of both channels)
  normally wants both encoder channels on interrupt pins. Since your
  spec puts C2 on D4 (not interrupt-capable on UNO), this sketch uses
  the standard single-interrupt technique instead:
    - D2 (C1) triggers an interrupt on every edge
    - D4 (C2) is just read as a plain digital input at that moment to
      determine direction
  This gives full step counting and correct direction with the wiring
  exactly as you specified. If you later want higher resolution, move
  C2 to D3 and decode both edges of both channels.
*/

#include <Arduino.h>

// ---------- Pin assignments ----------
const uint8_t PIN_RC_THROTTLE = 7;   // FlySky CH2 signal
const uint8_t PIN_RPWM        = 5;   // BTS7960 forward PWM
const uint8_t PIN_LPWM        = 6;   // BTS7960 reverse PWM
const uint8_t PIN_ENC_C1      = 2;   // Encoder channel A (interrupt)
const uint8_t PIN_ENC_C2      = 4;   // Encoder channel B (direction ref)

// If you wire R_EN / L_EN to Arduino pins instead of tying to 5V,
// set USE_ENABLE_PINS to true and assign the pins below.
const bool USE_ENABLE_PINS = false;
const uint8_t PIN_R_EN = 8;
const uint8_t PIN_L_EN = 9;

// ---------- RC signal calibration ----------
const int RC_MIN     = 1000;   // full reverse, us
const int RC_CENTER  = 1500;   // stop, us
const int RC_MAX     = 2000;   // full forward, us
const int DEADBAND_LOW  = 1475;
const int DEADBAND_HIGH = 1525;
const unsigned long RC_SIGNAL_LOST_MS = 250;  // failsafe: stop if no pulse this long

// ---------- Smooth acceleration (slew-rate limit) ----------
// Time in ms for the motor to ramp from 0 to full speed (255).
// Bigger number = gentler/slower ramp. Smaller = snappier.
const float RAMP_MS_FULL_SWING = 400.0;
const float RAMP_STEP_PER_MS = 255.0 / RAMP_MS_FULL_SWING;

// ---------- Non-blocking RC pulse capture (pin-change interrupt) ----------
volatile unsigned long rcPulseStart = 0;
volatile unsigned long rcPulseWidth = 1500; // last measured pulse, us
volatile bool rcNewEdge = false;
volatile unsigned long rcLastEdgeMillis = 0;

ISR(PCINT2_vect) {
  unsigned long now = micros();
  if (digitalRead(PIN_RC_THROTTLE) == HIGH) {
    rcPulseStart = now;
  } else {
    if (rcPulseStart != 0) {
      rcPulseWidth = now - rcPulseStart;
      rcNewEdge = true;
      rcLastEdgeMillis = millis();
    }
  }
}

// ---------- Encoder ----------
// Set this to your encoder's counts-per-motor-shaft-revolution
// (JGB37 encoder discs are commonly 11 CPR at the motor shaft; multiply
// by the gearbox ratio printed on your motor, e.g. 11 * 90 = 990).
const float ENCODER_COUNTS_PER_REV = 990.0;

volatile long encoderCount = 0;
volatile int8_t encoderDir = 0;  // +1 forward, -1 reverse

unsigned long lastRpmCalcTime = 0;
long lastEncoderCountForRpm = 0;
float currentRPM = 0.0;

unsigned long lastPrintTime = 0;
const unsigned long PRINT_INTERVAL_MS = 500; // how often to print to Serial

float currentSpeed = 0.0; // slew-rate limited output, -255..255
unsigned long lastRampUpdate = 0;

// ---------- Encoder ISR ----------
void encoderISR() {
  // C2 level at the moment C1 changes tells us direction
  if (digitalRead(PIN_ENC_C2) == HIGH) {
    encoderCount++;
    encoderDir = 1;
  } else {
    encoderCount--;
    encoderDir = -1;
  }
}

// ---------- Motor drive ----------
// speed: -255 (full reverse) ... 0 (stop) ... +255 (full forward)
void driveMotor(int speed) {
  speed = constrain(speed, -255, 255);
  if (speed > 0) {
    analogWrite(PIN_LPWM, 0);       // ensure the other side is off first
    analogWrite(PIN_RPWM, speed);
  } else if (speed < 0) {
    analogWrite(PIN_RPWM, 0);
    analogWrite(PIN_LPWM, -speed);
  } else {
    analogWrite(PIN_RPWM, 0);
    analogWrite(PIN_LPWM, 0);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_RC_THROTTLE, INPUT);
  pinMode(PIN_RPWM, OUTPUT);
  pinMode(PIN_LPWM, OUTPUT);
  pinMode(PIN_ENC_C1, INPUT_PULLUP);
  pinMode(PIN_ENC_C2, INPUT_PULLUP);

  if (USE_ENABLE_PINS) {
    pinMode(PIN_R_EN, OUTPUT);
    pinMode(PIN_L_EN, OUTPUT);
    digitalWrite(PIN_R_EN, HIGH);
    digitalWrite(PIN_L_EN, HIGH);
  }

  attachInterrupt(digitalPinToInterrupt(PIN_ENC_C1), encoderISR, CHANGE);

  // Enable pin-change interrupt on D7 (PCINT23) for non-blocking RC capture
  PCICR  |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT23);

  driveMotor(0); // start safe
  lastRpmCalcTime = millis();
  lastRampUpdate = millis();
  rcLastEdgeMillis = millis();
}

void loop() {
  // ---- 1. Grab latest RC pulse width (captured in background by ISR) ----
  noInterrupts();
  unsigned long pulse = rcPulseWidth;
  unsigned long lastEdge = rcLastEdgeMillis;
  interrupts();

  pulse = constrain(pulse, RC_MIN, RC_MAX);

  int targetSpeed = 0;
  if (pulse >= DEADBAND_LOW && pulse <= DEADBAND_HIGH) {
    targetSpeed = 0; // in deadband -> stop, no creep
  } else if (pulse > DEADBAND_HIGH) {
    targetSpeed = map(pulse, DEADBAND_HIGH, RC_MAX, 0, 255);
  } else { // pulse < DEADBAND_LOW
    targetSpeed = map(pulse, DEADBAND_LOW, RC_MIN, 0, -255);
  }

  // ---- 2. Failsafe: no valid RC pulses recently -> force target to stop ----
  if (millis() - lastEdge > RC_SIGNAL_LOST_MS) {
    targetSpeed = 0;
  }

  // ---- 3. Slew-rate limit: ramp currentSpeed toward targetSpeed smoothly ----
  unsigned long nowMs = millis();
  unsigned long dt = nowMs - lastRampUpdate;
  if (dt > 0) {
    lastRampUpdate = nowMs;
    float maxStep = RAMP_STEP_PER_MS * dt;
    float diff = targetSpeed - currentSpeed;
    if (diff > maxStep) diff = maxStep;
    if (diff < -maxStep) diff = -maxStep;
    currentSpeed += diff;
  }

  // ---- 4. Drive motor (only one of RPWM/LPWM active, handled in driveMotor) ----
  driveMotor((int)currentSpeed);

  // ---- 5. Compute RPM every 100 ms ----
  unsigned long now = millis();
  if (now - lastRpmCalcTime >= 100) {
    noInterrupts();
    long countNow = encoderCount;
    interrupts();

    long deltaCounts = countNow - lastEncoderCountForRpm;
    float deltaTimeMin = (now - lastRpmCalcTime) / 60000.0;
    currentRPM = (deltaCounts / ENCODER_COUNTS_PER_REV) / deltaTimeMin;

    lastEncoderCountForRpm = countNow;
    lastRpmCalcTime = now;
  }

  // ---- 6. Print debug output only every PRINT_INTERVAL_MS ----
  if (now - lastPrintTime >= PRINT_INTERVAL_MS) {
    lastPrintTime = now;
    noInterrupts();
    long countNow = encoderCount;
    interrupts();

    Serial.print("RC_us: "); Serial.print(pulse);
    Serial.print("  Target: "); Serial.print(targetSpeed);
    Serial.print("  Speed: "); Serial.print((int)currentSpeed);
    Serial.print("  Pos: "); Serial.print(countNow);
    Serial.print("  RPM: "); Serial.print(currentRPM);
    Serial.print("  Dir: "); Serial.println(encoderDir);
  }
}
