#include <ESP32Servo.h>  

// Servo motor declarations
Servo horizontal; // Horizontal servo (left-right)
int servohori = 90; 
int servohoriLimitHigh = 175;
int servohoriLimitLow = 5;

Servo vertical; // Vertical servo (up-down)
int servovert = 45;
int servovertLimitHigh = 100;
int servovertLimitLow = 1;

// Photoresistor (LDR) PINOUT
int ldrlt = 32; // Top left LDR
int ldrrt = 33; // Top right LDR
int ldrld = 34; // Bottom left LDR
int ldrrd = 35; // Bottom right LDR

void setup() {
  Serial.begin(115200);
  Serial.println("Memulai Sistem Solar Tracker...");

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  horizontal.setPeriodHertz(50); 
  vertical.setPeriodHertz(50);

  // Connect servos to pins (2 and 12)
  horizontal.attach(2, 500, 2400);
  vertical.attach(12, 500, 2400);

  // Initial position
  horizontal.write(servohori);
  vertical.write(servovert);

  Serial.println("Servo berada di posisi awal. Menunggu stabilisasi...");
  delay(2500); 
}

void loop() {
  // Read LDR values
  int lt = analogRead(ldrlt); // Top left
  int rt = analogRead(ldrrt); // Top right
  int ld = analogRead(ldrld); // Bottom left
  int rd = analogRead(ldrrd); // Bottom right

  int dtime = 40; 
  int tol = 360;   

  int avt = (lt + rt) / 2; // Top average
  int avd = (ld + rd) / 2; // Bottom average
  int avl = (lt + ld) / 2; // Left average
  int avr = (rt + rd) / 2; // Right average

  // Vertical and horizontal differences
  int dvert = avt - avd;
  int dhoriz = avl - avr;

  
  // 1. VERTICAL CORRECTION (ATAS - BAWAH)
  if (abs(dvert) > tol) {
    if (avt < avd) { 
      servovert++;
      if (servovert > servovertLimitHigh) servovert = servovertLimitHigh;
    } else {
      servovert--;
      if (servovert < servovertLimitLow) servovert = servovertLimitLow;
    }
    vertical.write(servovert);
  }

  // 2. HORIZONTAL CORRECTION (KIRI - KANAN)
  if (abs(dhoriz) > tol) {
    if (avl < avr) { 
      servohori++; 
      if (servohori > servohoriLimitHigh) servohori = servohoriLimitHigh;
    } else {
      servohori--;
      if (servohori < servohoriLimitLow) servohori = servohoriLimitLow;
    }
    horizontal.write(servohori);
  }

  delay(dtime);
}