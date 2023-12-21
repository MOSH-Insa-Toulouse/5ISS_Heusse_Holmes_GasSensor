# M-OSH - Gas Sensor
Developed by Emily Holmes and Cyprien Heusse as part of the Microcontroller and Open Source Hardware course of Innovative Smart Systems.

## Project Aim
As part of the Innovative Smart Systems major, we worked on a gas sensor. For this project, our work started with the conception and creation of the sensor in the AIME laboratory. We then had to work on the schematics, the routing and wiring, programming, wireless communication and network aspects. 

This makes this project an excellent way of testing our abilities in multiple of the key expertise developed in this field: network knowledge, programming skills, electronics... 

In this report, we will describe all of the steps we undertook to obtain the final results. 

## GitHub Content

### Delivrables 

In this repository are the following files:
- On the main branch: 
    - LTSpice simulation files
    - KiCad schematics and routing files 
    - Arduino code for the smart sensor
    - Datasheet for the sensor
    - README.md
- On the FakeGasSensor branch: 
    - Arduino code to run a simpler version of the project, as described below, with a commercialized sensor. 

### Material required to run the project

To run the project, we used an Arduino UNO, a LoRa module, and the gas sensors conceived at the AIME laboratory. A more specific list of components is specified below. 

An access to a LoRa gateway and a network server (ChirpStack here) are also needed.

## Introduction to LoRa and Gas Sensors
To become more familiar with both gas sensors and the LoRa protocol, that we hadn't used up to that point, we decided to make a simple sample program. This sample program is available on the FakeGasSensor branch. 

### How to run the program
For this, we had to use the LoRa gateway at INSA via Chirpstack. To replicate this program, it is necessary to add the LoRa chip you use to the INSA network [here](https://srv-chirpstack.insa-toulouse.fr/). 

You will also need: 
- An Arduino UNO
- A RN2483 LoRa module and an antenna
- A MQ2 gas sensor (or a MQX sensor, but ensure you have gas you can test it with available. MQ2 is easy to test with a lighter)
- Cables 

### Troubleshooting
* If the LoRa module does not connect, ensure you have generated the right keys on Chirpstack. We use OTAA connection, so you need to run the program once to retrieve your chip's EUI.
* If the gas sensor does not seem to detect gas, you might need to adjust the potentiometer on the sensor, and/or alter the threshold value in the program.
* LoRa can be a bit slow. It is normal to wait a few seconds before receiving the message.

### Results
The program sends a message when gas is detected. This message is a "1" (which might read as "0x31" in ASCII). No more messages are sent until the gas level fall below the threshold and we send a "0" (which might read as "0x30" in ACSII).

![final circuit](https://media.discordapp.net/attachments/478677987831578645/1181505875479904286/PXL_20231205_080117070.MP2.jpg)
Left: Gas sensor, top: RN2483 with antenna, bottom: Arduino UNO.

## AIME Gas Sensor and KiCad
After getting warmed up with the industrial sensor, we started working on the final project.

### Project Goals
As we want to create a smart sensor, it should be capable of performing various tasks without human intervention. Some of those tasks are:
- Calibrating itself
- Measuring its temperature and, if outside of operating temperature ranges, regenerate itself
- Recognizing a gas by comparing measured values to known characteristics
- Send data via LoRa to a gateway

### AIME Sensor
The sensor made at AIME has the following schematics:

<img src="https://cdn.discordapp.com/attachments/478677987831578645/1181508059709853696/image.png" height="400" />

There are 10 pins, which are:
- Two N.C. 
- The heating resistor (up to 20V).
- The temperature sensor.
- The gas sensors themselves (2 of them, so we could theorically have two areas for detecting and evene detect different gases).

## Amplifying the Signal from the Sensor
The gas sensor made has a huge resistance which varies depending on the presence of gas and on the nature of said gas. The value of the resistance will be around a G&Omega; meaning the current that will flow through it and to the ADC will be extremely small (around a nA). Since we will use an Arduino with only a 10-bit ADC, we need to amplify the signal exiting the gas sensor. To do so, we designed an amplifier circuit based on an operational amplifier using LTSpice. 

### LTSpice Simulation
The amplifier is based on 3 different filters, each with their own cutoff frequency. The first filter is centered around C1, R1 and R5, the second one from C4 and R3 and the last one from C2 and R4.
To better simulate the behaviour of the circuit, we created a component to simulate the gas sensor behavior as closely as possible. The sensor component is actually a circuit itself:

![Circuit for simulating the sensor](https://cdn.discordapp.com/attachments/638778639109980178/1184408570310172742/image-2.png)

The circuit as a whole is as follows:

![LTSpice circuit of the amplifier](https://cdn.discordapp.com/attachments/638778639109980178/1184406077937299456/image-1.png)

### Caracterizing the Amplification

Below are the cutoff frequencies of each filter.

| Filter | Cutoff frequency |
|--------|------------------|
| 1      | 16 Hz            |
| 2      | 1.6 Hz           |
| 3      | 1600 Hz          |

These filters have different purposes:
- The first filter is used to filter the sensor's noise itself
- The second filter is used to filter the 50Hz frequency from the power grid.
- The last filter is an anti-aliasing filter.

From this circuit we can deduce the resistance value of the gas sensor from the voltage on the ADC using the following formula:

![Formula for the sensor resistance](https://cdn.discordapp.com/attachments/638778639109980178/1187344050660065300/equation.png)

Where R is the resistance of the sensor and V<sub>ADC</sub> is the voltage detected by the ADC (between 0 and +5V, coded between 0 and 1023).


## Printed Circuit Board (PCB) design using KiCad

As the PCB had to be printed following the university's requirements and technological limitations, we could only use one layer to route the circuit. 

The required components are: 
- 100nF capacitors (3)
- 1uF capacitor (1) 
- 100k&Omega; resistors (2)
- 10k&Omega; resistors (2)
- 1k&Omega; resistors (2)
- 100&Omega; resistor (1)
- LTC1050 pp-amp (1)
- MCP41100 potentiometer (1) 
- SLW12 two-position switch (1)
- Arduino Uno (1)
- IRF520 NMOS (1)
- 7-pin header (1) for a LoRa RN2483 module

And the gas sensor conceived at AIME in a TO-5 10-pin package. The PCB is made to work with an Arduino shield. 

Below is a view of the routed PCB on KiCad.

![kicad](https://cdn.discordapp.com/attachments/638778639109980178/1186673919462219837/image.png)

### Soldered PCB

The PCB, once soldered with the LoRa module and its antenna, is pictured below. As explained above, it is printed on the bottom layer. From top to bottom, you can see: the antenna (on the left, cutoff), the LoRa module, the PCB, and the Arduino shield.

![final pcb](https://media.discordapp.net/attachments/638778639109980178/1186666419555930233/Sujet.png)

## Programming the Sensor

The programming was done by creating functions for each of the states we defined above: regeneration, calibration, measuring, etc. The goal is for the system to be as autonomous as possible. 

To better get a grasp of the system, we conceived a finite state machine describing the various functions to be used. 

![state machine](https://cdn.discordapp.com/attachments/692420982576513065/1186660316260597801/image.png)

The core of the Arduino code is quite simple: 
- In the setup: 
    - We prepare the LoRa communication (initializeRadio)
    - We calibrate the gain via the potentiometer (calibration)
    - We let the user choose the operating temperature 
- In the core of the program: 
    - We check that the temperature is within the operating range (checkTemperature)
        - If not, we regenerate (regeneration) 
    - We measure the gas (measureGas) and send it (commLoRa)

### Gas Identification

As explained above, this sensor is capable to recognise between two different gases: Ethanol, and Ammonia. This is done by comparing the sensitibility regression curve of the measured gas (in Volts) to known values for both of these gases. 

### Improving Energy Consumption

Energy savings are one of the key metrics to keep track of in IoT. Here are some ways to improve the current systems to ensure low consumption: 
- Replacing the Arduino by a low-power microcontroller and programming at a lower level
- Optimizing the gas sensor so it does not require a 12V power supply
- Using a low energy protocol such as Bluetooth Low Energy, or conceiving a protocol for this specific application as we have done in other ISS courses.  

## Node-RED

To finalize this project, we created a dashboard using Node-RED to show the values sent via LoRa. 
A simple dashboard indicating the last message received along with the gas type, and the presence of gas. The flow is visible below, as well as a GIF of the dashboard in function.

![nodered](https://cdn.discordapp.com/attachments/638778639109980178/1187350230602088529/dashboard.png?ex=65969112&is=65841c12&hm=38876362430c4593d9f59f59c712c4cdcecf35a58fd2cdda9cdf3de9ebd0d929&)

[![dashboard](https://i.gyazo.com/8564e397834e893862e17c3f704f2910.gif)](https://gyazo.com/8564e397834e893862e17c3f704f2910)


## Conclusion and Future Work
By the end of the project, we have the core of a smart sensor system with the ability to monitor it on an external application. To finalize this work, it would be necessary to edit the program with accurate values for gas sensitivity from the sensor. 

Nevertheless, this repository provides a solid basis to conceive and use this smart sensor, with an approach from a physical, electronical, and software point of view. 