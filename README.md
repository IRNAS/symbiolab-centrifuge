# Symbiolab centrifuge

## Overview	<a id="OvER"></a>
Symbiolab centrifuge is a DIY project still in development for Symbiolab at institute IRNAS. The centrifuge is based on our very own Symbiolab driver PCB on which a repository will be created in a short future. The whole centrifuge, besides the PCB, is made of cheap and easy accsesible parts, like brushless motors, and lasercut or 3D printed parts.

## Table of contents
 - [overview](#OvER)
 - [Part list](#LIST)
 - [Parts](#PARTS)
 - [Centrifuge schematic](#SCH)
 - [Centrifuge housing](#DXF)
 - [arduino code](#CODE)

 
## Required parts list	<a id="LIST"></a>
 - Symbiolab driver
 - Brushless motor 
 - Electronic speed contoller "ESC"
 - IR tachometer
  * LMV 761
  * IR LED
  * IR photodiode
  * 33K resistor
  * 25 ohm resistor
 - Three state switch
 - DC-025 DC power jack connector
 - XT60 female connector
 - Electric cables and jumper wires
 - Acrylic glass "plexiglas"
 - M3 15mm nuts and bolts

## Parts	<a id="PARTS"></a>

### Symbiolab driver
Symbiolab driver is a project developed by Institute IRNAS, for DIY laboratory equipment. this is its first finished installment. Symbiolab driver is based on [NODEMCU](http://www.nodemcu.com/index_en.html) firmware, that enables future expansion to wireless controll.

### Brushless motor/ ESC
For the Symbiolab centrifuge we are using a [A2212 KV1000](http://www.robotdigg.com/product/710/A2212-brushless-drone-motor) brushless motor controlled by a [Skywalker 40A ESC controller](http://www.robotdigg.com/product/802/Skywalker-40A-ESC)

### IR tachometer
IR tachometer is used for RPM sensing and is crucial to achieving a self speed regulating controll. The sensor works with the help of reflecting light from the brushless motor therefore it is important to tape up a part of the motor, with a black(dark) tape, to create different levels of light reflecting from the motor. A LMV 761 comparator is added to transform the analog value from the photodiode to a rectangle pulse when it exceeds a reference value set via potentiometer. With this it is possible to connect the photodiode to a interrupt pin, making shure we do not miss out any positive pulses resoulting a foulty rpm reading. this method is also fater than ADC reading, thus making the whole controller faster.

#### Tachometer schematic 
![Tachometer schematic](https://github.com/symbiolab/Symbiolab_Centrifuge/blob/master/IR_Sens.png)

### Three state switch
the switch is essentially used to turn on/off the centrifuge, however you can switch between power from the DC jack or LiPo battery.

## Centrifuge schematic	<a id="SCH"></a>
![Centrifuge schematic](https://github.com/Ubercic/Symbiolab_Centrifuge/blob/master/Symbiolab_Centrifuge.jpg)
To choose the power supply source, both the dc jack and XT60 are connected the 3 state switch on pins IN1 and IN2. The switch output is connected to the 12V IN terminal block on the Biolab driver. For IR tachomer we need to cennect 3v3 power suply from the Biolab driver, in return the IR tachomer sends back IR Sens data(HIGH or LOW) depending on the motor position.  The ESC is connnected to the Biolab driver via 12v power supply and the PWM GPIO.

The default pins on the Biolab drivers are:

FUNCTION|PIN NO.
--------|-------
ESC PWM | D0
IR Sens | D8

Or you could also change the pin definitions in the [Code](https://github.com/Ubercic/Symbiolab_Centrifuge/blob/ArduinoCode/ArduinoCode/CentrifugaV1RPM_No_PID/CentrifugaV1RPM_No_PID.ino)

## Centrifuge housing	<a id="DXF"></a>
All the components for the housing are lasercut from plexiglas (approx 500mm x500mm) and screwed together with M3 15mm nuts and bolts. The DXF files can be found in the [DXF_Files](https://github.com/Ubercic/Symbiolab_Centrifuge/tree/master/DXF_Files) folder.

## Arduino code	<a id="CODE"></a>
The code is developed in the Arduino IDE environment using [Arduino core](https://github.com/esp8266/Arduino) for use of the lua based NODEMCU with the ease of arduino coding. A working arduino code is uploaded, but it still needs some fine tuning and testing.

