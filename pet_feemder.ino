
#define BLYNK_TEMPLATE_ID "TMPL7JJHSAQa"
#define BLYNK_DEVICE_NAME "Pet Feeder"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

#include "BlynkEdgent.h"
#include <Servo.h>

Servo myservo;
int sensorData;
bool notificationSent = false;
int calibrationTime = 30;
int pirPin = 14;

BlynkTimer timer;
BlynkTimer timer1;

void timerEvent() {
  sensorData = digitalRead(pirPin);
  if (sensorData == 1 and notificationSent == false) {
    Serial.println("Motion detected");
    Blynk.logEvent("motion_detection");
    Blynk.virtualWrite(V2 , 1);
    notificationSent = true;
  }
}

void checkSensor() {
  if (notificationSent == true) {
    notificationSent = false;
    digitalWrite(pirPin, LOW);
    Blynk.virtualWrite(V2 , 0);
  }
}


BLYNK_WRITE(V1) {
  int pinValue = param.asInt(); // assigning incoming value from pin V2 to a variable
  if (pinValue == 1) {
    for(int i = 0; i < 80; i++){
      myservo.write(2*i);
      delay(10);
    }
    delay(100);
    myservo.write(0);
    Serial.println("Servo moved on button press");
  }
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);
}

void setup()
{
  Serial.begin(115200);

  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }

  myservo.attach(2);
  myservo.write(0);
  delay(100);
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  BlynkEdgent.begin();
  timer.setInterval(1000L, timerEvent);
  timer1.setInterval(5000L, checkSensor);
}

void loop() {
  BlynkEdgent.run();
  timer.run();
  timer1.run();
}
