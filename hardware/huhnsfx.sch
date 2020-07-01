EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 700  800  1100 850 
U 5D389AA1
F0 "SolarCharger" 50
F1 "solarcharger.sch" 50
F2 "SOLARP" B R 1800 1200 50 
F3 "SOLARM" B R 1800 1300 50 
F4 "BATTP" B R 1800 1450 50 
F5 "BATTM" B R 1800 1550 50 
F6 "~CHRG" O R 1800 900 50 
F7 "~FAULT" O R 1800 1000 50 
$EndSheet
$Comp
L power:GND #PWR06
U 1 1 5D3E2852
P 2350 7100
F 0 "#PWR06" H 2350 6850 50  0001 C CNN
F 1 "GND" H 2355 6927 50  0000 C CNN
F 2 "" H 2350 7100 50  0001 C CNN
F 3 "" H 2350 7100 50  0001 C CNN
	1    2350 7100
	1    0    0    -1  
$EndComp
$Comp
L huhnsfx-rescue:C-Device C2
U 1 1 5D3E2F0B
P 2350 6750
F 0 "C2" H 2465 6796 50  0000 L CNN
F 1 "10µF" H 2465 6705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2388 6600 50  0001 C CNN
F 3 "~" H 2350 6750 50  0001 C CNN
	1    2350 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 6500 2350 6500
Wire Wire Line
	2350 6500 2350 6600
Wire Wire Line
	2350 7100 2350 7000
Connection ~ 2350 7000
Wire Wire Line
	2350 7000 2350 6900
$Comp
L huhnsfx-rescue:C-Device C1
U 1 1 5D3E3D2C
P 1100 6750
F 0 "C1" H 1215 6796 50  0000 L CNN
F 1 "10µF" H 1215 6705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1138 6600 50  0001 C CNN
F 3 "~" H 1100 6750 50  0001 C CNN
	1    1100 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 6500 1100 6600
Wire Wire Line
	1100 6900 1100 7000
$Comp
L power:+BATT #PWR02
U 1 1 5D3E4E05
P 1100 6400
F 0 "#PWR02" H 1100 6250 50  0001 C CNN
F 1 "+BATT" H 1115 6573 50  0000 C CNN
F 2 "" H 1100 6400 50  0001 C CNN
F 3 "" H 1100 6400 50  0001 C CNN
	1    1100 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 6500 1100 6400
Connection ~ 1100 6500
$Comp
L power:+3V3 #PWR05
U 1 1 5D3E5654
P 2350 6400
F 0 "#PWR05" H 2350 6250 50  0001 C CNN
F 1 "+3V3" H 2365 6573 50  0000 C CNN
F 2 "" H 2350 6400 50  0001 C CNN
F 3 "" H 2350 6400 50  0001 C CNN
	1    2350 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 6400 2350 6500
Connection ~ 2350 6500
$Sheet
S 700  1900 1100 400 
U 5D3F2DAA
F0 "RTC + BME280" 50
F1 "rtc.sch" 50
F2 "SCL" I R 1800 2000 50 
F3 "SDA" B R 1800 2100 50 
F4 "INT" O R 1800 2200 50 
$EndSheet
$Comp
L power:GND #PWR018
U 1 1 5D4168D7
P 9500 4750
F 0 "#PWR018" H 9500 4500 50  0001 C CNN
F 1 "GND" H 9505 4577 50  0000 C CNN
F 2 "" H 9500 4750 50  0001 C CNN
F 3 "" H 9500 4750 50  0001 C CNN
	1    9500 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 4750 9500 4650
$Comp
L power:+3V3 #PWR016
U 1 1 5D41752A
P 8400 950
F 0 "#PWR016" H 8400 800 50  0001 C CNN
F 1 "+3V3" H 8415 1123 50  0000 C CNN
F 2 "" H 8400 950 50  0001 C CNN
F 3 "" H 8400 950 50  0001 C CNN
	1    8400 950 
	1    0    0    -1  
$EndComp
$Comp
L huhnsfx-rescue:SW_Push-Switch SW1
U 1 1 5D41A3F9
P 4250 6550
F 0 "SW1" V 4204 6698 50  0000 L CNN
F 1 "RESET" V 4295 6698 50  0000 L CNN
F 2 "Button_Switch_SMD:SW_SPST_CK_RS282G05A3" H 4250 6750 50  0001 C CNN
F 3 "~" H 4250 6750 50  0001 C CNN
	1    4250 6550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5D41B67A
P 4250 6950
F 0 "#PWR09" H 4250 6700 50  0001 C CNN
F 1 "GND" H 4255 6777 50  0000 C CNN
F 2 "" H 4250 6950 50  0001 C CNN
F 3 "" H 4250 6950 50  0001 C CNN
	1    4250 6950
	1    0    0    -1  
$EndComp
Text Label 8350 2050 0    50   ~ 0
ESP32_EN
Text Label 3400 6250 0    50   ~ 0
ESP32_EN
Wire Wire Line
	4250 6350 4250 6250
$Comp
L huhnsfx-rescue:R-Device R6
U 1 1 5D420051
P 4250 6000
F 0 "R6" H 4320 6046 50  0000 L CNN
F 1 "10K" H 4320 5955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4180 6000 50  0001 C CNN
F 3 "~" H 4250 6000 50  0001 C CNN
	1    4250 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 6250 4250 6150
Connection ~ 4250 6250
$Comp
L huhnsfx-rescue:C-Device C3
U 1 1 5D4216BF
P 3850 6550
F 0 "C3" H 3965 6596 50  0000 L CNN
F 1 "100nF" H 3965 6505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3888 6400 50  0001 C CNN
F 3 "~" H 3850 6550 50  0001 C CNN
	1    3850 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 6750 4250 6850
Connection ~ 4250 6850
Wire Wire Line
	4250 6850 4250 6950
Wire Wire Line
	3400 6250 3850 6250
Wire Wire Line
	3850 6250 3850 6400
Connection ~ 3850 6250
Wire Wire Line
	3850 6250 4250 6250
Wire Wire Line
	3850 6700 3850 6850
Wire Wire Line
	3850 6850 4250 6850
$Comp
L power:+3V3 #PWR08
U 1 1 5D4276CA
P 4250 5750
F 0 "#PWR08" H 4250 5600 50  0001 C CNN
F 1 "+3V3" H 4265 5923 50  0000 C CNN
F 2 "" H 4250 5750 50  0001 C CNN
F 3 "" H 4250 5750 50  0001 C CNN
	1    4250 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 5750 4250 5850
Wire Wire Line
	8400 950  8400 1050
Wire Wire Line
	8400 1050 8900 1050
Wire Wire Line
	9500 1050 9500 1850
$Comp
L power:GND #PWR017
U 1 1 5D42D3CE
P 8900 1650
F 0 "#PWR017" H 8900 1400 50  0001 C CNN
F 1 "GND" H 8905 1477 50  0000 C CNN
F 2 "" H 8900 1650 50  0001 C CNN
F 3 "" H 8900 1650 50  0001 C CNN
	1    8900 1650
	1    0    0    -1  
$EndComp
$Comp
L huhnsfx-rescue:C-Device C4
U 1 1 5D42D62E
P 8900 1300
F 0 "C4" H 9015 1346 50  0000 L CNN
F 1 "100nF" H 9015 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8938 1150 50  0001 C CNN
F 3 "~" H 8900 1300 50  0001 C CNN
	1    8900 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 1650 8900 1550
Wire Wire Line
	8900 1550 8400 1550
Wire Wire Line
	8400 1550 8400 1450
Wire Wire Line
	8400 1050 8400 1150
Connection ~ 8400 1050
$Comp
L huhnsfx-rescue:C-Device C5
U 1 1 5D42F66C
P 8400 1300
F 0 "C5" H 8515 1346 50  0000 L CNN
F 1 "10µF" H 8515 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8438 1150 50  0001 C CNN
F 3 "~" H 8400 1300 50  0001 C CNN
	1    8400 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 1450 8900 1550
Connection ~ 8900 1550
Wire Wire Line
	8900 1150 8900 1050
Connection ~ 8900 1050
Wire Wire Line
	8900 1050 9500 1050
NoConn ~ 8900 2350
$Comp
L huhnsfx-rescue:Conn_01x06-Connector_Generic J6
U 1 1 5D434209
P 9800 5850
F 0 "J6" H 9880 5842 50  0000 L CNN
F 1 "UART" H 9880 5751 50  0000 L CNN
F 2 "Connector_PinHeader_1.27mm:PinHeader_2x03_P1.27mm_Vertical" H 9800 5850 50  0001 C CNN
F 3 "~" H 9800 5850 50  0001 C CNN
	1    9800 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 5650 9500 5650
Wire Wire Line
	9500 5650 9500 5550
$Comp
L power:+3V3 #PWR020
U 1 1 5D434FBC
P 9500 5550
F 0 "#PWR020" H 9500 5400 50  0001 C CNN
F 1 "+3V3" H 9515 5723 50  0000 C CNN
F 2 "" H 9500 5550 50  0001 C CNN
F 3 "" H 9500 5550 50  0001 C CNN
	1    9500 5550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5D4351D2
P 9500 6250
F 0 "#PWR021" H 9500 6000 50  0001 C CNN
F 1 "GND" H 9505 6077 50  0000 C CNN
F 2 "" H 9500 6250 50  0001 C CNN
F 3 "" H 9500 6250 50  0001 C CNN
	1    9500 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 6250 9500 6150
Wire Wire Line
	9500 6150 9600 6150
Text Label 9100 5750 0    50   ~ 0
ESP32_TXD
Text Label 9100 5850 0    50   ~ 0
ESP32_RXD
Text Label 9100 5950 0    50   ~ 0
ESP32_IO0
Text Label 9100 6050 0    50   ~ 0
ESP32_EN
Wire Wire Line
	10100 2050 10650 2050
Text Label 10650 2050 2    50   ~ 0
ESP32_IO0
Text Notes 8900 5750 0    50   ~ 0
TX
Text Notes 8900 5850 0    50   ~ 0
RX
Text Notes 8900 5950 0    50   ~ 0
DTR
Text Notes 8900 6050 0    50   ~ 0
RTS
Text Notes 8900 6150 0    50   ~ 0
GND
Text Notes 8900 5650 0    50   ~ 0
3V3
Wire Wire Line
	8350 2050 8900 2050
$Sheet
S 700  2500 1100 1000
U 5D401D05
F0 "Motor" 50
F1 "motor.sch" 50
F2 "MOTOR_REV" I R 1800 2700 50 
F3 "MOTOR_FWD" I R 1800 2600 50 
F4 "MOTORP" B R 1800 2900 50 
F5 "MOTORM" B R 1800 3000 50 
F6 "SDA" B R 1800 3150 50 
F7 "SCL" I R 1800 3250 50 
$EndSheet
Wire Wire Line
	10100 2150 10650 2150
Text Label 10650 2150 2    50   ~ 0
ESP32_TXD
Wire Wire Line
	10100 2350 10650 2350
Text Label 10650 2350 2    50   ~ 0
ESP32_RXD
Wire Wire Line
	9100 6050 9600 6050
Wire Wire Line
	9100 5950 9600 5950
Wire Wire Line
	9100 5850 9600 5850
Wire Wire Line
	9100 5750 9600 5750
$Comp
L huhnsfx-rescue:Buzzer-Device BZ1
U 1 1 5D4E542A
P 6300 6150
F 0 "BZ1" H 6452 6179 50  0000 L CNN
F 1 "Buzzer" H 6452 6088 50  0000 L CNN
F 2 "huhnsfx:CMT-4023S" V 6275 6250 50  0001 C CNN
F 3 "~" V 6275 6250 50  0001 C CNN
	1    6300 6150
	1    0    0    -1  
$EndComp
$Comp
L huhnsfx-rescue:BSS138-Transistor_FET Q1
U 1 1 5D4E7226
P 6000 6600
F 0 "Q1" H 6206 6646 50  0000 L CNN
F 1 "BSS138" H 6206 6555 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 6200 6525 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS138.pdf" H 6000 6600 50  0001 L CNN
	1    6000 6600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5D4ED70B
P 6100 7200
F 0 "#PWR015" H 6100 6950 50  0001 C CNN
F 1 "GND" H 6105 7027 50  0000 C CNN
F 2 "" H 6100 7200 50  0001 C CNN
F 3 "" H 6100 7200 50  0001 C CNN
	1    6100 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 6400 6100 6250
Wire Wire Line
	6100 6250 6200 6250
$Comp
L power:+3V3 #PWR014
U 1 1 5D4F1B73
P 6100 5550
F 0 "#PWR014" H 6100 5400 50  0001 C CNN
F 1 "+3V3" H 6115 5723 50  0000 C CNN
F 2 "" H 6100 5550 50  0001 C CNN
F 3 "" H 6100 5550 50  0001 C CNN
	1    6100 5550
	1    0    0    -1  
$EndComp
$Comp
L huhnsfx-rescue:R-Device R7
U 1 1 5D4F21F4
P 6100 5800
F 0 "R7" H 6170 5846 50  0000 L CNN
F 1 "0" H 6170 5755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6030 5800 50  0001 C CNN
F 3 "~" H 6100 5800 50  0001 C CNN
	1    6100 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 5550 6100 5650
Wire Wire Line
	6100 5950 6100 6050
Wire Wire Line
	6100 6050 6200 6050
Text Label 5200 6600 0    50   ~ 0
ESP32_BUZZ
Text Label 10650 3150 2    50   ~ 0
ESP32_BUZZ
Text Label 10650 2250 2    50   ~ 0
REED_TOP
NoConn ~ 8900 3250
NoConn ~ 8900 3350
NoConn ~ 8900 3450
NoConn ~ 8900 3550
NoConn ~ 8900 3650
NoConn ~ 8900 3750
Wire Wire Line
	10100 2550 10650 2550
Text Label 10650 2550 2    50   ~ 0
S2LP_CS
Wire Wire Line
	10100 3250 10650 3250
Text Label 10650 3250 2    50   ~ 0
S2LP_SCK
Wire Wire Line
	10650 3350 10100 3350
Text Label 10650 3350 2    50   ~ 0
S2LP_MISO
Wire Wire Line
	10650 3650 10100 3650
Text Label 10650 3650 2    50   ~ 0
S2LP_MOSI
Text Label 10650 3050 2    50   ~ 0
S2LP_SDN
Text Label 10650 2450 2    50   ~ 0
S2LP_INT
Text Label 10650 4350 2    50   ~ 0
BTN_DOWN
Text Label 2400 2600 2    50   ~ 0
MOTOR_FWD
Text Label 2400 2700 2    50   ~ 0
MOTOR_REV
Text Label 2400 2200 2    50   ~ 0
RTC_INT
Wire Wire Line
	1800 2200 2400 2200
Wire Wire Line
	1800 2700 2400 2700
Wire Wire Line
	1800 2600 2400 2600
Wire Wire Line
	1800 2100 2400 2100
Text Label 2400 2100 2    50   ~ 0
SDA
Wire Wire Line
	2400 2000 1800 2000
Text Label 2400 2000 2    50   ~ 0
SCL
Wire Wire Line
	1800 900  2400 900 
Text Label 2400 900  2    50   ~ 0
SOLAR_CHRG
Text Label 10650 3750 2    50   ~ 0
RTC_INT
Text Label 10650 3850 2    50   ~ 0
SDA
Text Label 10650 3950 2    50   ~ 0
SCL
Text Label 10650 4250 2    50   ~ 0
BTN_UP
Wire Wire Line
	10650 4150 10100 4150
Wire Wire Line
	10650 4250 10100 4250
Text Label 10650 4050 2    50   ~ 0
SOLAR_CHRG
Text Label 10650 3450 2    50   ~ 0
MOTOR_FWD
Text Label 10650 3550 2    50   ~ 0
MOTOR_REV
Wire Wire Line
	10650 4350 10100 4350
Wire Wire Line
	10650 3050 10100 3050
Wire Wire Line
	10100 3150 10650 3150
Wire Wire Line
	10650 3450 10100 3450
Wire Wire Line
	10650 3550 10100 3550
Wire Wire Line
	10100 2250 10650 2250
Wire Wire Line
	10650 2450 10100 2450
Wire Wire Line
	10650 3750 10100 3750
Wire Wire Line
	10100 3850 10650 3850
Wire Wire Line
	10650 3950 10100 3950
Wire Wire Line
	8900 2250 8350 2250
Text Label 8350 2250 0    50   ~ 0
BATT_SENSE
Text Label 6550 1700 2    50   ~ 0
BATT_SENSE
$Comp
L power:+BATT #PWR03
U 1 1 5D5C3D06
P 6000 1200
F 0 "#PWR03" H 6000 1050 50  0001 C CNN
F 1 "+BATT" H 6015 1373 50  0000 C CNN
F 2 "" H 6000 1200 50  0001 C CNN
F 3 "" H 6000 1200 50  0001 C CNN
	1    6000 1200
	1    0    0    -1  
$EndComp
$Comp
L huhnsfx-rescue:R-Device R2
U 1 1 5D5C4736
P 6000 1450
F 0 "R2" H 6070 1496 50  0000 L CNN
F 1 "820K" H 6070 1405 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5930 1450 50  0001 C CNN
F 3 "~" H 6000 1450 50  0001 C CNN
	1    6000 1450
	1    0    0    -1  
$EndComp
$Comp
L huhnsfx-rescue:R-Device R3
U 1 1 5D5C49CC
P 6000 1950
F 0 "R3" H 6070 1996 50  0000 L CNN
F 1 "220K" H 6070 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5930 1950 50  0001 C CNN
F 3 "~" H 6000 1950 50  0001 C CNN
	1    6000 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 1300 6000 1200
Wire Wire Line
	6550 1700 6000 1700
Wire Wire Line
	6000 1700 6000 1600
Wire Wire Line
	6000 1700 6000 1800
Connection ~ 6000 1700
$Comp
L power:GND #PWR04
U 1 1 5D5CE52A
P 6000 2200
F 0 "#PWR04" H 6000 1950 50  0001 C CNN
F 1 "GND" H 6005 2027 50  0000 C CNN
F 2 "" H 6000 2200 50  0001 C CNN
F 3 "" H 6000 2200 50  0001 C CNN
	1    6000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2200 6000 2100
Text Label 2400 4300 2    50   ~ 0
S2LP_SDN
Text Label 2400 4400 2    50   ~ 0
S2LP_INT
Text Label 2400 3900 2    50   ~ 0
S2LP_MOSI
Text Label 2400 4000 2    50   ~ 0
S2LP_SCK
Text Label 2400 3800 2    50   ~ 0
S2LP_MISO
Text Label 2400 4100 2    50   ~ 0
S2LP_CS
$Sheet
S 700  3700 1100 800 
U 5D5FAE57
F0 "S2-LP" 50
F1 "s2lp.sch" 50
F2 "SDO" O R 1800 3800 50 
F3 "SDI" I R 1800 3900 50 
F4 "SCLK" I R 1800 4000 50 
F5 "CSn" I R 1800 4100 50 
F6 "SDN" I R 1800 4300 50 
F7 "GPIO3" O R 1800 4400 50 
$EndSheet
Wire Wire Line
	1800 3800 2400 3800
Wire Wire Line
	1800 3900 2400 3900
Wire Wire Line
	1800 4000 2400 4000
Wire Wire Line
	1800 4100 2400 4100
Wire Wire Line
	1800 4300 2400 4300
Wire Wire Line
	1800 4400 2400 4400
Text Label 5350 1800 2    50   ~ 0
SCL
Text Label 5350 1950 2    50   ~ 0
SDA
$Comp
L power:+3V3 #PWR07
U 1 1 5D9AC66B
P 4750 1150
F 0 "#PWR07" H 4750 1000 50  0001 C CNN
F 1 "+3V3" H 4765 1323 50  0000 C CNN
F 2 "" H 4750 1150 50  0001 C CNN
F 3 "" H 4750 1150 50  0001 C CNN
	1    4750 1150
	1    0    0    -1  
$EndComp
$Comp
L huhnsfx-rescue:R-Device R4
U 1 1 5D9ACDAD
P 4750 1500
F 0 "R4" H 4820 1546 50  0000 L CNN
F 1 "4.7K" H 4820 1455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4680 1500 50  0001 C CNN
F 3 "~" H 4750 1500 50  0001 C CNN
	1    4750 1500
	1    0    0    -1  
$EndComp
$Comp
L huhnsfx-rescue:R-Device R5
U 1 1 5D9AD055
P 5000 1500
F 0 "R5" H 5070 1546 50  0000 L CNN
F 1 "4.7K" H 5070 1455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4930 1500 50  0001 C CNN
F 3 "~" H 5000 1500 50  0001 C CNN
	1    5000 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 1150 4750 1250
Wire Wire Line
	4750 1250 5000 1250
Wire Wire Line
	5000 1250 5000 1350
Connection ~ 4750 1250
Wire Wire Line
	4750 1250 4750 1350
Wire Wire Line
	5000 1650 5000 1800
Wire Wire Line
	5000 1800 5350 1800
Wire Wire Line
	5350 1950 4750 1950
Wire Wire Line
	4750 1950 4750 1650
Text Label 4550 3550 0    50   ~ 0
REED_TOP
$Comp
L power:+3V3 #PWR01
U 1 1 5D5FFA71
P 4000 1450
F 0 "#PWR01" H 4000 1300 50  0001 C CNN
F 1 "+3V3" H 4015 1623 50  0000 C CNN
F 2 "" H 4000 1450 50  0001 C CNN
F 3 "" H 4000 1450 50  0001 C CNN
	1    4000 1450
	1    0    0    -1  
$EndComp
$Comp
L huhnsfx-rescue:R-Device R1
U 1 1 5D60044B
P 4000 1700
F 0 "R1" H 4070 1746 50  0000 L CNN
F 1 "1M" H 4070 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3930 1700 50  0001 C CNN
F 3 "~" H 4000 1700 50  0001 C CNN
	1    4000 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 1950 4000 1850
Text Label 4450 1950 2    50   ~ 0
S2LP_SDN
Wire Wire Line
	4450 1950 4000 1950
Wire Wire Line
	4000 1550 4000 1450
Wire Wire Line
	1100 6500 1450 6500
$Comp
L mic5239-3.3ymm:MIC5239-3.3YMM U1
U 1 1 5D5B1B4E
P 1800 6600
F 0 "U1" H 1800 6965 50  0000 C CNN
F 1 "MIC5239-3.3YMM" H 1800 6874 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223" H 1800 6600 50  0001 C CNN
F 3 "" H 1800 6600 50  0001 C CNN
	1    1800 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 7000 1700 7000
Wire Wire Line
	1700 6900 1700 7000
Connection ~ 1700 7000
Wire Wire Line
	1700 7000 1900 7000
Wire Wire Line
	1900 6900 1900 7000
Connection ~ 1900 7000
Wire Wire Line
	1900 7000 2350 7000
$Comp
L RF_Module:ESP32-WROOM-32U U2
U 1 1 5D6DB7BD
P 9500 3250
F 0 "U2" H 9900 4650 50  0000 C CNN
F 1 "ESP32-WROOM-32U" H 10000 1850 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32U" H 9500 1750 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 9200 3300 50  0001 C CNN
	1    9500 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1200 2400 1200
Text Label 2400 1200 2    50   ~ 0
SOLARP
Wire Wire Line
	1800 1300 2400 1300
Text Label 2400 1300 2    50   ~ 0
SOLARM
Wire Wire Line
	1800 1450 2400 1450
Text Label 2400 1450 2    50   ~ 0
BATTP
Wire Wire Line
	2400 1550 1800 1550
Text Label 2400 1550 2    50   ~ 0
BATTM
Wire Wire Line
	5650 2850 5250 2850
Wire Wire Line
	5650 3150 5250 3150
Wire Wire Line
	5650 3050 5250 3050
Wire Wire Line
	5650 3350 5250 3350
Wire Wire Line
	5650 3250 5250 3250
Text Label 5250 3250 0    50   ~ 0
SOLARP
Text Label 5250 3350 0    50   ~ 0
SOLARM
Text Label 5250 3050 0    50   ~ 0
BATTP
Text Label 5250 3150 0    50   ~ 0
BATTM
Wire Wire Line
	2400 2900 1800 2900
Wire Wire Line
	1800 3000 2400 3000
Text Label 2400 2900 2    50   ~ 0
MOTORP
Text Label 2400 3000 2    50   ~ 0
MOTORM
Text Notes 3600 7350 0    50   ~ 0
Switch: \nC&K RS-282G05A3-SM RT
$Comp
L Connector_Generic:Conn_01x12 J1
U 1 1 5D7CF9E4
P 5850 3350
F 0 "J1" H 5930 3342 50  0000 L CNN
F 1 "Conn_01x12" H 5930 3251 50  0000 L CNN
F 2 "huhnsfx:WAGO-233-512" H 5850 3350 50  0001 C CNN
F 3 "~" H 5850 3350 50  0001 C CNN
	1    5850 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 4050 10100 4050
Text Label 10650 4150 2    50   ~ 0
SOLAR_FAULT
Wire Wire Line
	1800 1000 2400 1000
Text Label 2400 1000 2    50   ~ 0
SOLAR_FAULT
Text Label 4550 3950 0    50   ~ 0
BTN_DOWN
Text Label 4550 3750 0    50   ~ 0
BTN_UP
Wire Wire Line
	5650 2950 5250 2950
$Comp
L power:+3V3 #PWR011
U 1 1 5D49513C
P 4400 3300
F 0 "#PWR011" H 4400 3150 50  0001 C CNN
F 1 "+3V3" H 4415 3473 50  0000 C CNN
F 2 "" H 4400 3300 50  0001 C CNN
F 3 "" H 4400 3300 50  0001 C CNN
	1    4400 3300
	1    0    0    -1  
$EndComp
Text Label 5250 2850 0    50   ~ 0
MOTORM
Text Label 5250 2950 0    50   ~ 0
MOTORP
$Comp
L Device:R R18
U 1 1 5EAAC242
P 5700 6850
F 0 "R18" H 5770 6896 50  0000 L CNN
F 1 "10K" H 5770 6805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5630 6850 50  0001 C CNN
F 3 "~" H 5700 6850 50  0001 C CNN
	1    5700 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 6600 5700 6600
Wire Wire Line
	6100 7200 6100 7100
Wire Wire Line
	6100 7100 5700 7100
Wire Wire Line
	5700 7100 5700 7000
Wire Wire Line
	6100 6800 6100 7100
Connection ~ 6100 7100
Wire Wire Line
	5700 6700 5700 6600
Connection ~ 5700 6600
Wire Wire Line
	5700 6600 5800 6600
Connection ~ 4400 3650
Wire Wire Line
	4400 3650 4400 3850
Wire Wire Line
	4400 3300 4400 3450
Connection ~ 4400 3450
Wire Wire Line
	4400 3450 4400 3650
$Comp
L Device:R R17
U 1 1 5EB20BE7
P 5100 4250
F 0 "R17" H 5170 4296 50  0000 L CNN
F 1 "100K" H 5170 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5030 4250 50  0001 C CNN
F 3 "~" H 5100 4250 50  0001 C CNN
	1    5100 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R19
U 1 1 5EB4E193
P 5350 4250
F 0 "R19" H 5420 4296 50  0000 L CNN
F 1 "100K" H 5420 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5280 4250 50  0001 C CNN
F 3 "~" H 5350 4250 50  0001 C CNN
	1    5350 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R20
U 1 1 5EB4E5CA
P 5600 4250
F 0 "R20" H 5670 4296 50  0000 L CNN
F 1 "100K" H 5670 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5530 4250 50  0001 C CNN
F 3 "~" H 5600 4250 50  0001 C CNN
	1    5600 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 3950 5100 3950
Wire Wire Line
	4400 3850 5650 3850
Wire Wire Line
	4550 3750 5350 3750
Wire Wire Line
	4400 3650 5650 3650
Wire Wire Line
	4550 3550 5600 3550
Wire Wire Line
	4400 3450 5650 3450
Wire Wire Line
	5600 4100 5600 3550
Connection ~ 5600 3550
Wire Wire Line
	5600 3550 5650 3550
Wire Wire Line
	5350 4100 5350 3750
Connection ~ 5350 3750
Wire Wire Line
	5350 3750 5650 3750
Wire Wire Line
	5100 4100 5100 3950
Connection ~ 5100 3950
Wire Wire Line
	5100 3950 5650 3950
$Comp
L power:GND #PWR0102
U 1 1 5EB9B3D0
P 5600 4600
F 0 "#PWR0102" H 5600 4350 50  0001 C CNN
F 1 "GND" H 5605 4427 50  0000 C CNN
F 2 "" H 5600 4600 50  0001 C CNN
F 3 "" H 5600 4600 50  0001 C CNN
	1    5600 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 4600 5600 4500
Wire Wire Line
	5600 4500 5350 4500
Wire Wire Line
	5350 4500 5350 4400
Connection ~ 5600 4500
Wire Wire Line
	5600 4500 5600 4400
Wire Wire Line
	5350 4500 5100 4500
Wire Wire Line
	5100 4500 5100 4400
Connection ~ 5350 4500
NoConn ~ 10100 2650
NoConn ~ 10100 2750
NoConn ~ 10100 2850
NoConn ~ 10100 2950
Wire Wire Line
	1800 3150 2400 3150
Wire Wire Line
	1800 3250 2400 3250
Text Label 2400 3150 2    50   ~ 0
SDA
Text Label 2400 3250 2    50   ~ 0
SCL
$EndSCHEMATC