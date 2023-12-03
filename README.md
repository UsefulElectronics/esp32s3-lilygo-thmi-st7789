[![Build Status](https://img.shields.io/badge/USEFUL%20ELECTRONICS-YOUTUBE-red)](https://www.youtube.com/user/wardzx1)

# Sponsors

### PCBWay
Tutorial Series Sponsor PCBWay Website. You can get extra free coupons using the link below:

[<img src="https://github.com/UsefulElectronics/esp32s3-gc9a01-lvgl/blob/main/banner/banner(320x100).gif">](https://www.pcbway.com/setinvite.aspx?inviteid=582640)

***
# ESP32S3 Lilygo T HMI Board Review
***
### [Tutorial Link](https://youtu.be/12MyKaSyPj4) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 

In this Tutorial, I am reviewing T-HMI ESP32-S3 based Development board from Lilygo and its hardware design to see what it is capable of achieving ot see whether it is a good candidate for future project !

The board has dual core ESP32-S3 MCU and internal 8MB PSRAM, 16MB  NOR Flash and 2.8 inch TFT display controlled over 8080 parallel interface. The board also has resistive touch panel mounted.

![Circuit Diagram](https://github.com/UsefulElectronics/esp32s3-gc9a01-lvgl/blob/main/circuit%20diagram/cover1.png)
***

# ESP32S3 THMI Board Interfacing RYUW122 UWB Module
***
### [Tutorial Link](https://youtu.be/anwFOHaNnUQ) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 

In this tutorial, ESP32S3 micro-controller is used to interface RYUW122 UWB module from REYAX over UART with AT commands and a 2.8 Inch TFT display over I80 parallel interface and LVGL utilized for nicely looking user interface design.

In the used set up, 2 UWM module are used. One is configured as Acnchor and the other one is configured as Tag. the Anchor module reports communication distance when it receives packet from the other end TAG device over UART and the ESP32S3 processes the received packet and prints the distance information on the TFT screen. 

RYUW122 UWB module packets are analyzed using logic analyzer to demonstrate the module and hosting MCU communication flow.

Squareline studio is used for nicely looking UI deisgn that outputs LVGL library components to be integrated in the ESP32S3 firmware.

![Circuit Diagram](https://github.com/UsefulElectronics/esp32s3-gc9a01-lvgl/blob/main/circuit%20diagram/cover1.png)