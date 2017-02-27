#Symbiolab centrifuge

##Overview
Symbiolab centrifuge is a DIY project still in development for Symbiolab at institute IRNAS. The centrifuge is based on our very own Symbiolab driver PCB on which a repository will be created in a short future. The whole centrifuge, besides the PCB, is made of cheap and easy accsesible parts, like brushless motors, and lasercut or 3D printed parts.

##Table of contents
 - owerview
 - Part list
 - Parts
 - Centrifuge schematic
 - Centrifuge housing
 - arduino code

 
##Required parts list
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

##Parts

###Symbiolab driver
Symbiolab driver is a project developed by Institute IRNAS, for DIY laboratory equipment. this is its first finished installment. Symbiolab driver is based on [NODEMCU](http://www.nodemcu.com/index_en.html) firmware, that enables future expansion to wireless controll.

###Brushless motor/ ESC
For the Symbiolab centrifuge we are using a [A2212 KV1000](http://www.robotdigg.com/product/710/A2212-brushless-drone-motor) brushless motor controlled by a [Skywalker 40A ESC controller](http://www.robotdigg.com/product/802/Skywalker-40A-ESC)

###IR tachometer
IR tachometer is used for RPM sensing and is crucial to achieving a self speed regulating controll. The sensor works with the help of reflecting light from the brushless motor therefore it is important to tape up a part of the motor, with a black(dark) tape, to create different levels of light reflecting from the motor. A LMV 761 comparator is added to transform the analog value from the photodiode to a rectangle pulse when it exceeds a reference value set via potentiometer. With this it is possible to connect the photodiode to a interrupt pin, making shure we do not miss out any positive pulses resoulting a foulty rpm reading. this method is also fater than ADC reading, thus making the whole controller faster.

#### Tachometer schematic 

###Three state switch
the switch is essentially used to turn on/off the centrifuge, however you can switch between power from the DC jack or LiPo battery.

##Centrifuge schematic

##Centrifuge housing
All the components for the housing are lasercut from plexiglas (approx 500mm x500mm) and screwed together with M3 15mm nuts and bolts. The DXF files can be found in the [DXF_Files](https://github.com/Ubercic/Symbiolab_Centrifuge/tree/master/DXF_Files) folder.
