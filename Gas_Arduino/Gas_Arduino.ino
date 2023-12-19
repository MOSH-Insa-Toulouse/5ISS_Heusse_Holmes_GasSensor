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
#include <SPI.h>

#define txPin 5
#define rxPin 6
#define mosiPin 11
#define sckPin 13
#define adc1 1
#define adc2 0
#define swCtrl 2
#define potAddr 0x11

#define activateGas digitalWrite(swCtrl, HIGH)
#define deactivateGas digitalWrite(swCtrl, LOW)

int potLastVal = 128;
int operatingTemp = 50;  

float measuresGas[10];
int measuresIndex = 0;

float refEthanol = 5.0;
float refAmmonia = 10.0;

SoftwareSerial mySerial(5, 6); 
rn2xx3 myLora(mySerial);


// --------------------
//
// HELPER FUNCTIONS
//
// --------------------

void potentiometerWrite(int addr, int val) {
  SPI.transfer(addr);
  SPI.transfer(val);
  Serial.print("Wrote");
  Serial.print(val);
  Serial.println("to potentiometer");
}

int ADCtoVolt(int adc) {
  if (adc == 1) {
    float val = analogRead(adc1);

  } else if (adc == 2) {
    float val = analogRead(adc2);
  } else { 
    Serial.print("ADC doesn't exist");
    val = -1;
  }
  return val/1024*5.0;
}

// --------------------
//
// SMART SENSOR FUNCTIONS
//
// --------------------

int measureGas() {
  for (int i = 0; i <= 10, i++) {
  valVolt = ADCtoVolt(1);
  measuresGas[i] = valVolt;
  delay(500); // Wait 0.5s between measures
  }

  // Taking first and last measures to find slope. Ideally, a regression curve should be computed...
  // a = (y_b-y_a)/(x_a-x_b)
  float sensitivitySlope = (measuresGas[10] - measuresGas[0])/10; 

  if (abs(sensitivitySlope - refAmmonia) < abs(sensitivitySlope - refEthanol)) {
    return 0;
  } else {
    return 1;
  }
}

void checkTemperature() {
  float coefficient = 0.5; // TODO: Deduce from resistivity abacus from datasheet 
  float margin = 5.0; // margin in degrees C around the operating temperature

  valVolt = ADCtoVolt(1);
  float temp = valVolt*coefficient;

  if !(operatingTemp-5 < temp && operating+5 > temp) { // Temperature outside of range
    regeneration();
  }
}

void calibration() {
  // Ping ADC
  valVolt = ADCtoVolt(1);

  if (valVolt <= 0.5) {
    // Measure too low -> Increase gain
    potLastVal += 10;
    potentiometerWrite(potAddr, potLastVal);
    delay(1000)
    calibration();  // Recursive call
  } else if (valVolt >= 4.5) {
    // Measure saturated -> Decrease gain 
    potLastVal -= 10;
    potentiometerWrite(potAddr, potLastVal)
    delay(1000)
    calibration(); // Recursive call
  }
}

void regeneration() {
  valVolt = ADCtoVolt(2);

  if (valVolt >= 4.75) {
    digitalWrite(swCtrl, HIGH);
    Serial.print("Waiting for sensor to heat up..."):
    delay(5000);
    Serial.print("Waiting for sensor to cool down...");
    delay(5000);
  }
}

void commLoRa(int bit) {
  myLora.tx(msg);
}

void initialize_radio() {
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

// --------------------
//
// ARDUINO
//
// --------------------

void setup()
{
  // LORA
  Serial.begin(57600); //serial port to computer
  mySerial.begin(9600); //serial port to radio
  Serial.println("Startup LoRa");
  initialize_radio();

  // SPI MCP41100
  SPI.begin();

  // 12V POWER
  pinMode(swCtrl, OUTPUT);

  // Calibrate 
  calibration();

  // Choose operational temperature
  operatingTemp = Serial.read();
  Serial.print("Temperature chosen:");
  Serial.println(operatingTemp);

  delay(1000);
}

void loop()
{
  checkTemperature();
  commLora(measureGas());

  delay(5000);
}


