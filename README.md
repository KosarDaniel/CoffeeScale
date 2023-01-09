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

|-GND--->GND
|-VCC--->5V (RAW)
|-SCL--->Arduino [PIN 3 (SCL)]
|-SDA--->Arduino [PIN 2 (SDA)]

### Momentary Switch:

|------->GND
|------->Arduino [PIN 7 (interrupt)]

### Arduino Pro Micro:

        1-| [USB] |-RAW--> <br />
        0-|       |-GND <br />
      GND-|       |-RST <br />
   <--GND-|       |-VCC <br />
SDA<----2-|       |-21 <br />
SCL<----3-|       |-20 <br />
        4-|       |-19 <br />
        5-|       |-18
        6-|       |-15
MSW<----7-|       |-14
 DT<----8-|       |-16
SCK<----9-|       |-10
