# CoffeeScale

This project is a coffee scale from a strain block and the HX711 chip.

## Components:

1. Arduino Pro Micro
2. Strain Block
3. HX711 IC
4. 128x32 OLED Display
5. Momentary switch

## Connections:

### HX711:

|-VCC--->5V (RAW) <br />
|-SCK--->Arduino [PIN 9] <br />
|-DT---->Arduino [PIN 8] <br />
|-GND--->GND <br />

### 128x32 OLED Display:

|-GND--->GND <br />
|-VCC--->5V (RAW) <br />
|-SCL--->Arduino [PIN 3 (SCL)] <br />
|-SDA--->Arduino [PIN 2 (SDA)] <br />

### Momentary Switch:

|------->GND <br />
|------->Arduino [PIN 7 (interrupt)]

### Arduino Pro Micro:

<pre>
        1-| [USB] |-RAW-->
        0-|       |-GND 
      GND-|       |-RST 
   <--GND-|       |-VCC 
SDA<----2-|       |-21 
SCL<----3-|       |-20 
        4-|       |-19 
        5-|       |-18
        6-|       |-15
MSW<----7-|       |-14
 DT<----8-|       |-16
SCK<----9-|       |-10
</pre>

##Things to add
1. Calibration
1. 3D files
1. Change grapf axes ticks
