# M-OSH - Gas Sensor
Developed by Emily Holmes and Cyprien Heusse :D as part of the Microcontroller and Open Source Hardware course of Innovative Smart Systems.

## Project Aim
As part of the Innovative Smart Systems major, we worked on a gas sensor. For this project, our work started with the conception and creation of the sensor in the AIME laboratory. We then had to work on the schematics, the routing and wiring, programming, wireless communication and network aspects. 

This makes this project an excellent way of testing our abilities in multiple of the key expertise developed in this field: network knowledge, programming skills, electronics... 

In this report, we will describe all of the steps we undertook to obtain the final results. 

## GitHub Content

### Delivrables 

### Material required to run the project

## Introduction to LoRa and Gas Sensors
To become more familiar with both gas sensors and the LoRa protocol, that we hadn't used up to that point, we decided to make a simple sample program. This sample program is available on the FakeGasSensor branch. 

### How to run the program
For this, we had to use the LoRa gateway at INSA via Chirpstack. To replicate this program, it is necessary to add the LoRa chip you use to the INSA network [here](https://srv-chirpstack.insa-toulouse.fr/). 

You will also need: 
- An Arduino UNO
- An RN2483 LoRa module and an antenna
- A MQ2 gas sensor (or a MQX sensor, but ensure you have gas you can test it with available. MQ2 is easy to test with a lighter)
- Cables 

### Troubleshooting
* If the LoRa module does not connect, ensure you have generated the right keys on Chirpstack. We use OTAA connection, so you need to run the program once to retrieve your chip's EUI.
* If the gas sensor does not seem to detect gas, you might need to adjust the potentiometer on the sensor, and/or alter the threshold value in the program.
* LoRa can be a bit slow. It is normal to wait a few seconds before receiving the message.

### Results
The program sends a message when gas is detected. This message is a "1" (which might read as "0x31" in ASCII). No more messages are sent until the gas level fall below the threshold and we send a "0" (which might read as "0x30" in ACSII).

![final circuit](https://media.discordapp.net/attachments/478677987831578645/1181505875479904286/PXL_20231205_080117070.MP2.jpg?ex=65814e18&is=656ed918&hm=08d3848640994ea0a08ba0beea36754597e01953486f734bd7251ad09fec1c00&=&format=webp&width=1026&height=577)
Left: Gas sensor, top: RN2483 with antenna, bottom: Arduino UNO.

## AIME Gas Sensor and KiCad
After getting warmed up with the industrial sensor, we started working on the final project.

### AIME sensor
The sensor made at AIME has the following schematics:

<img src="https://cdn.discordapp.com/attachments/478677987831578645/1181508059709853696/image.png?ex=65815020&is=656edb20&hm=6460b3e81fa263fb95ca48d8f5e4c76be8bfa6773b01d3a3d7b7afe4b3b43e5e&" height="400" />

There are 10 pins, which are:
- Two N.C. 
- The power (up to 20V).
- The temperature sensor.
- The gas sensors themselves (2 of them, so we could theorically deposit two different gases).   

### End goal: How should the system work?
To be continued another day because I'm tired