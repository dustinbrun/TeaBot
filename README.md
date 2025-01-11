# Arduino TeaBot - automatic Tea Maker
The design uses the 3D model created by SnakeP (licensed under Creative Commons - Attribution, see link below). I adapted the TeaBot_Base to be able to fit the electronics inside it. This eliminates the need for a separate electronics enclosure. The parts `TeaBot_Arm_LH` and `TeaBot_Arm_RH` need to be printed two times each.

I removed the battery from the device, it is powered directly from the USB Type-C connector.

<img src="images/Tea_Bot_1.jpg" width="300px">
<img src="images/Tea_Bot_2.jpg" width="200px">
<img src="images/Tea_Bot_3.jpg" width="300px">
<img src="images/Tea_Bot_4.jpg" width="300px">
<img src="images/Tea_Bot_5.jpg" width="300px">

# Hardware

## parts list
- Arduino Nano
- OLED display I2C 128x32 with SSD1306 controller
- rotary encoder
- optional: Buzzer
- MG996R Servo
- 8 x M3x25mm screws
- 1 x M3x12mm screw
- 9 x M3 nuts (self locking)
- 1 x alligator clip
- 4x 3,0x12 screws

## Schematic
<img src="TeaBot_Schaltplan.png" width="600px">

<img src="TeaBot_Steckplatine.png" width="600px">

## 3D-printed parts
The 3D parts can be found in the folder [`3D`](3D).


# Software
The software can be found in the folder [`TeaBot_software`](TeaBot_software)

Key features:
- the rotary encoder is used to set the time, the time value is stored in the EEPROM of the Arduino, so at the next start the previous time value is restored
- Button operation: 
    - single click: start/stop
    - double click: enable/disable dipping (setting is stored in EEPROM), options: OFF, every 10 seconds, every 30 seconds
- Buzzer sound after timer expires



# References/Sources
- 3D model: https://www.thingiverse.com/thing:5250153
- used libraries are linked in the code
- OLED display example: https://www.instructables.com/Arduino-and-the-SSD1306-OLED-I2C-128x64-Display/

<br>
<p xmlns:dct="http://purl.org/dc/terms/" xmlns:cc="http://creativecommons.org/ns#" class="license-text">This work by <span property="cc:attributionName">Dustin Brunner</span> is licensed under <a rel="license" href="https://creativecommons.org/licenses/by/4.0">CC BY 4.0<img style="height:15px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1" /><img style="height:15px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1" /></a></p>

<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Creative Commons Lizenzvertrag" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />Dieses Werk von <span xmlns:cc="http://creativecommons.org/ns#" property="cc:attributionName">Dustin Brunner</span> ist lizenziert unter einer <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Namensnennung 4.0 International Lizenz</a>.