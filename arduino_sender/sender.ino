#include <Wire.h>
#include <RF24.h>
#include <MPU6050.h>

MPU6050 mpu;
RF24 radio(9, 10); // CE = 9, CSN = 10
const byte address[6] = "00001";

unsigned long lastUpdate = 0;
unsigned long lastMotionTime = 0;
const unsigned long idleThreshold = 10000;

bool wasIdle = false;

float velocity = 0;
float distance = 0;

unsigned long lastMoveTime = 0;
String status = "IDLE";

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address);
  radio.stopListening();

  lastUpdate = millis();
  lastMotionTime = millis();
}

void loop() {
  unsigned long now = millis();
  float dt = (now - lastUpdate) / 1000.0; // in seconds
  lastUpdate = now;

  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Convert to g's
  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  // Magnitude of acceleration
  float accel = sqrt(ax_g * ax_g + ay_g * ay_g + az_g * az_g) - 1.0; // remove gravity

  // Noise threshold
  if (abs(accel) < 0.5) accel = 0;

  // Decay velocity when there's no acceleration
  if (accel == 0) {
    velocity *= 0.9;  // reduce velocity gradually
    if (abs(velocity) < 0.05) velocity = 0;
  }

  // Integrate acceleration to get speed and distance
  velocity += accel * 9.81 * dt;      // m/s
  distance += velocity * dt;          // m

  // Motion threshold logic
  float motionThreshold = 0.5;  // 0.5 m/s is considered idle
  if (velocity > motionThreshold) {
    status = "MOVING";
    lastMoveTime = millis();
  } else {
    if ((millis() - lastMoveTime) > 10000) {
      status = "IDLE";
    }
  }

  // Send only when status changes or every 1 sec
  static unsigned long lastSent = 0;
  if (now - lastSent >= 1000 || (status == "IDLE" && !wasIdle) || (status == "MOVING" && wasIdle)) {
    wasIdle = (status == "IDLE");
    lastSent = now;

    // Prepare payload (for radio)
    char payload[64];
    snprintf(payload, sizeof(payload), "{\"s\":\"%s\",\"v\":%.2f,\"d\":%.2f}", status.c_str(), velocity, distance);

    // Print full info to Serial
    Serial.print("{\"s\":\"");
    Serial.print(status);
    Serial.print("\",\"v\":");
    Serial.print(velocity, 2);
    Serial.print(",\"d\":");
    Serial.print(distance, 2);
    Serial.println("}");

    bool ok = radio.write(&payload, strlen(payload) + 1);
    if (!ok) Serial.println("Failed to send");
  }

  delay(100); // adjust for stability
}
