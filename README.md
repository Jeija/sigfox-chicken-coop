# Sigfox-Enabled Chicken Coop Controller
![PCB with annotations](images/banner.jpg)


This repository contains hardware design files as well as firmware source code for a chicken coop controller board. The tiny controller (approx. 50x50mm) automatically opens and closes the chicken coop's door at scheduled times of day. It also transmits environmental data (e.g. temperature, humidity) via the Sigfox network and uses a solar chell to recharge a connected lead-acid battery.

The device operates in conjunction with a web service that uses the Sigfox API to retrieve the latest messages and that provides the current configuration and time of day to the controller. The web service has an end user-facing frontend that displays environmental data, device status information and lets the user configure door opening / closing times.

As a whole, this project probably isn't particularly useful for you since it represents a rather customized solution. Parts of it though may be of interest, e.g. the S2-LP transmitter design, the solar charger design or the firmware, which can be used as reference for how to integrate [renard-phy-s2lp](https://github.com/jeija/renard-phy-s2lp), an open source Sigfox PHY based on the open-source Sigfox stack [librenard](https://github.com/jeija/librenard) into an ESP32 project.

## Hardware Design
The controller mainly consists of the following components:

* An ESP32 microcontroller with integrated WiFi. WiFi functionality is only used for Over-the-Air (OTA) updating
* An S2-LP transceiver for Sigfox uplink and downlink, the RX balun / impedance matching is implemented with discrete components
* A LT3652 solar charging controller for lead-acid battery charging
* A RV-3029-C2 Real Time Clock (RTC)
* A BME280 environmental sensor for measuring temperature, pressure and humidity
* A DRV8872 as DC motor driver with an INA219 for motor current measurement (stall detection) for the door, as well as an input for a reed contact

Everything is mounted on a tiny 50x50mm PCB which fits nicely into an off-the-shelf casing (Strapubox A515)

![The PCB in its case](images/casing.jpg)

The system is is powered by a 12V/7.2Ah lead-acid battery, which is recharged by the solar panel on top of the chicken coop. The Sigfox antenna is also mounted outside for an increased link budget.

![The PCB in its case](images/external.jpg)

[**Download PDF Schematic**](hardware_pdf/schematic.pdf)
[**Download PDF PCB Layout**](hardware_pdf/pcb.pdf)
[**KiCad Sources**](hardware)

## Firmware

## Web Component
This component is currently not public.