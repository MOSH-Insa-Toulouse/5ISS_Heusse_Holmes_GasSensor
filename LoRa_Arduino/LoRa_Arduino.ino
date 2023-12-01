/*
 * Original author: JP Meijers
 * Date: 2016-02-07
 *
 * Original test program edited by: Emily Holmes and Cyprien Heusse
 * Last modification: 2023-11-29
 *
 * RN2483 is connected as follows:
 * RN2483 -- Arduino
 * UART TX -- 10
 * UART RX -- 11
 * Reset -- 12
 * VCC -- 3.3V
 * GND -- GND
 *
 * Gas sensor is connected as follows:
 * Sensor -- Arduino
 * VCC -- 5V
 * GND -- GND
 * Sig -- A0
 */

#include <rn2xx3.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
rn2xx3 myLora(mySerial);

int gasPin = A0;
bool detectedGas = 0;

void setup()
{
  pinMode(gasPin, INPUT);

  //output LED pin
  pinMode(13, OUTPUT);
  led_on();

  Serial.begin(57600); //serial port to computer
  mySerial.begin(9600); //serial port to radio
  Serial.println("Startup");

  initialize_radio();

  led_off();
  delay(2000);
}

void initialize_radio()
{
  // Reset RN2483 -> Logic low
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(500);
  digitalWrite(12, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  mySerial.flush();

  //Autobaud the rn2483 module to 9600. The default would otherwise be 57600.
  myLora.autobaud();

  //Retreive EUI
  String hweui = myLora.hweui();
  while(hweui.length() != 16)
  {
    Serial.println("Communication with RN2483 unsuccessful. Power cycle the board.");
    Serial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  //print out the HWEUI 
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(myLora.hweui());
  Serial.println("RN2483 firmware version:");
  Serial.println(myLora.sysver());

  // Join via OTAA
  Serial.println("Trying to join Chirpstack");
  bool join_result = false;
  const char *appEui = "D724B71BA25FD924";
  const char *appKey = "6cb1d3c9c5903cd897ae167bc10514a8";
  join_result = myLora.initOTAA(appEui, appKey);


  while(!join_result)
  {
    Serial.println("Unable to join. Trying again...");
    delay(10000); 
    join_result = myLora.init();
  }
  Serial.println("Successfully joined Chirpstack!");

}

void loop()
{
    float sensor_volt;
    float sensorValue;
    String msg_low = "0";
    String msg_high = "1";

    led_on();
    
    sensorValue = analogRead(A0);
    sensor_volt = sensorValue/1024*5.0;

    Serial.print("sensor_volt = ");
    Serial.print(sensor_volt);
    Serial.println("V");

    if (sensor_volt > 3.0) {
      Serial.println("TXing high");
      myLora.tx(msg_high); 
      detectedGas = 1;
    } else if (detectedGas == 1) {
      Serial.println("TXing low");
      myLora.tx(msg_low); 
      detectedGas = 0;
    }

    led_off();
    delay(5000);
}

void led_on()
{
  digitalWrite(13, 1);
}

void led_off()
{
  digitalWrite(13, 0);
}

