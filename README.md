[![Build Status](https://img.shields.io/badge/USEFUL%20ELECTRONICS-YOUTUBE-red)](https://www.youtube.com/user/wardzx1)

# Sponsors

### PCBWay
Tutorial Series Sponsor PCBWay Website. You can get extra free coupons using the link below:

[<img src="https://github.com/UsefulElectronics/esp32s3-lilygo-thmi-ryuw122/blob/main/pictures/pcbwaybanner.jpg">](https://www.pcbway.com/setinvite.aspx?inviteid=582640)

***
| Project | Tag |
| ------ | ------ |
| RYUW122 UWB module  | [RYUW122](https://github.com/UsefulElectronics/esp32s3-lilygo-thmi-st7789/tree/RYUW122)|
| SGP40 Air Quality Sensor | [SGP40][https://github.com/UsefulElectronics/esp32s3-lilygo-thmi-st7789/tree/SGP40] |

***
# ESP32S3 Lilygo T HMI Board Review
***
### [Tutorial Link](https://youtu.be/12MyKaSyPj4) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 

In this Tutorial, I am reviewing T-HMI ESP32-S3 based Development board from Lilygo and its hardware design to see what it is capable of achieving ot see whether it is a good candidate for future project !

The board has dual core ESP32-S3 MCU and internal 8MB PSRAM, 16MB  NOR Flash and 2.8 inch TFT display controlled over 8080 parallel interface. The board also has resistive touch panel mounted.

![Circuit Diagram](https://github.com/UsefulElectronics/esp32s3-lilygo-thmi-ryuw122/blob/main/pictures/THMI.png)
***

# ESP32S3 THMI Board Interfacing RYUW122 UWB Module
***
### [Tutorial Link](https://youtu.be/anwFOHaNnUQ) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 

In this tutorial, ESP32S3 micro-controller is used to interface RYUW122 UWB module from REYAX over UART with AT commands and a 2.8 Inch TFT display over I80 parallel interface and LVGL utilized for nicely looking user interface design.

In the used set up, 2 UWM module are used. One is configured as Acnchor and the other one is configured as Tag. the Anchor module reports communication distance when it receives packet from the other end TAG device over UART and the ESP32S3 processes the received packet and prints the distance information on the TFT screen. 

RYUW122 UWB module packets are analyzed using logic analyzer to demonstrate the module and hosting MCU communication flow.

Squareline studio is used for nicely looking UI deisgn that outputs LVGL library components to be integrated in the ESP32S3 firmware.

![Circuit Diagram](https://github.com/UsefulElectronics/esp32s3-lilygo-thmi-ryuw122/blob/main/pictures/uwb%20cover.png)

***
# ESP32S3 SGP40 Air Quality Sensor
***
### [Tutorial Link](https://youtu.be/_cIWVNCPDKA) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 

Having an air quality sensor at home helps you monitor indoor air quality, ensuring a healthier and safer living environment.

In this video we are going to use SGP40 air quality sensor and interface it over I2C using ESP32S3 Micro controller which is driving 2.8 inch TFT display over parallel interface.

The dual core MCU that we are using reads the sensor data processes it using sensirion algorithm to obtain VOC index Volatile Organic Compound an air quality measure and drives the LCD on separate cores using FreeRTOS Operation system.

The MCU Prints the the sensor data on the LCD with a nicely looking User interface designed with the help of LVGL library and square line studio. We are going to learn about the surrounding air quality today so lets put the show on the road.

![Circuit Diagram](https://github.com/UsefulElectronics/esp32s3-lilygo-thmi-ryuw122/blob/main/pictures/spg40%20thumnail.png)
***

# ESP32S3 SGP40 Air Quality Sensor
***
### [Tutorial Link](https://youtu.be/A8EMmGQPYvc) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 

Design Stunning GUIs for Your Microcontroller Projects with Squareline Studio & LVGL!

Ever dreamed of creating your own graphical user interface (GUI) on your PC and seamlessly importing it to your hardware? In this video, we're diving deep into the process of designing a dynamic user interface using Squareline Studio and then integrating it with an ESP32-S3 project powered by the LVGL library.

We'll break down every element of the UI, including the round value gauge, the smoothly shifting bar chart, and the eye-catching background gradient. You'll learn how to initialize and control each component with your microcontroller, ensuring your sensor data is displayed in a sleek and professional manner.

![Circuit Diagram](https://github.com/UsefulElectronics/esp32s3-lilygo-thmi-ryuw122/blob/main/pictures/AIR%20QUALITY%20SENSOR%20GUI.png)
***