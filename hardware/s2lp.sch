EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L s2lp:S2-LP U8
U 1 1 5D5FB9D7
P 4900 2050
F 0 "U8" H 5500 2950 50  0000 C CNN
F 1 "S2-LP" H 5500 1150 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-24-1EP_4x4mm_P0.5mm_EP2.7x2.7mm_ThermalVias" H 5100 1700 50  0001 C CNN
F 3 "" H 5100 1700 50  0001 C CNN
	1    4900 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal_GND24 Y1
U 1 1 5D5FFD18
P 6350 1500
F 0 "Y1" V 6400 1250 50  0000 L CNN
F 1 "50MHz" V 6250 1100 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm" H 6350 1500 50  0001 C CNN
F 3 "~" H 6350 1500 50  0001 C CNN
	1    6350 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	5900 1250 6350 1250
Wire Wire Line
	6350 1250 6350 1350
Wire Wire Line
	6350 1650 6350 1750
Wire Wire Line
	6350 1750 5900 1750
Wire Wire Line
	6150 1500 6050 1500
$Comp
L Device:C C42
U 1 1 5D60A7FE
P 6600 1250
F 0 "C42" V 6650 1400 50  0000 C CNN
F 1 "12pF" V 6450 1250 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6638 1100 50  0001 C CNN
F 3 "~" H 6600 1250 50  0001 C CNN
	1    6600 1250
	0    1    1    0   
$EndComp
Wire Wire Line
	6450 1250 6350 1250
Connection ~ 6350 1250
$Comp
L Device:C C43
U 1 1 5D60B6BA
P 6600 1750
F 0 "C43" V 6650 1900 50  0000 C CNN
F 1 "12pF" V 6450 1750 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6638 1600 50  0001 C CNN
F 3 "~" H 6600 1750 50  0001 C CNN
	1    6600 1750
	0    1    1    0   
$EndComp
Wire Wire Line
	6450 1750 6350 1750
Connection ~ 6350 1750
$Comp
L power:GND #PWR0107
U 1 1 5D60D852
P 6850 3700
F 0 "#PWR0107" H 6850 3450 50  0001 C CNN
F 1 "GND" H 6855 3527 50  0000 C CNN
F 2 "" H 6850 3700 50  0001 C CNN
F 3 "" H 6850 3700 50  0001 C CNN
	1    6850 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1750 6750 1750
Wire Wire Line
	6850 1500 6850 1750
Wire Wire Line
	6550 1500 6850 1500
Connection ~ 6850 1750
Wire Wire Line
	6750 1250 6850 1250
Wire Wire Line
	6850 1250 6850 1500
Connection ~ 6850 1500
$Comp
L power:+3V3 #PWR0108
U 1 1 5D616F04
P 1400 900
F 0 "#PWR0108" H 1400 750 50  0001 C CNN
F 1 "+3V3" H 1415 1073 50  0000 C CNN
F 2 "" H 1400 900 50  0001 C CNN
F 3 "" H 1400 900 50  0001 C CNN
	1    1400 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 1000 5050 1100
Wire Wire Line
	4950 1100 4950 1000
Connection ~ 4950 1000
Wire Wire Line
	4950 1000 5050 1000
Wire Wire Line
	4850 1100 4850 1000
Connection ~ 4850 1000
Wire Wire Line
	4850 1000 4950 1000
Wire Wire Line
	4750 1100 4750 1000
$Comp
L Device:C_Small C16
U 1 1 5D61BA25
P 1400 2200
F 0 "C16" V 1500 2250 50  0000 L CNN
F 1 "4.7uF" V 1500 1900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1438 2050 50  0001 C CNN
F 3 "~" H 1400 2200 50  0001 C CNN
	1    1400 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C20
U 1 1 5D61C2FE
P 1850 2200
F 0 "C20" V 1950 2250 50  0000 L CNN
F 1 "150nF" V 1950 1900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1888 2050 50  0001 C CNN
F 3 "~" H 1850 2200 50  0001 C CNN
	1    1850 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C22
U 1 1 5D61C618
P 2200 2200
F 0 "C22" V 2300 2250 50  0000 L CNN
F 1 "100pF" V 2300 1900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2238 2050 50  0001 C CNN
F 3 "~" H 2200 2200 50  0001 C CNN
	1    2200 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C24
U 1 1 5D625E62
P 2650 2200
F 0 "C24" V 2750 2250 50  0000 L CNN
F 1 "150nF" V 2750 1900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2688 2050 50  0001 C CNN
F 3 "~" H 2650 2200 50  0001 C CNN
	1    2650 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C26
U 1 1 5D62771F
P 2900 2200
F 0 "C26" V 3000 2250 50  0000 L CNN
F 1 "100pF" V 3000 1900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2938 2050 50  0001 C CNN
F 3 "~" H 2900 2200 50  0001 C CNN
	1    2900 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C30
U 1 1 5D628597
P 3400 2200
F 0 "C30" V 3500 2250 50  0000 L CNN
F 1 "100pF" V 3500 1900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3438 2050 50  0001 C CNN
F 3 "~" H 3400 2200 50  0001 C CNN
	1    3400 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 1000 4850 1000
Text HLabel 4050 2200 0    50   Output ~ 0
SDO
Wire Wire Line
	4050 2200 4150 2200
Text HLabel 4050 2300 0    50   Input ~ 0
SDI
Wire Wire Line
	4050 2300 4150 2300
Text HLabel 4050 2400 0    50   Input ~ 0
SCLK
Wire Wire Line
	4050 2400 4150 2400
Text HLabel 4050 2500 0    50   Input ~ 0
CSn
Wire Wire Line
	4050 2500 4150 2500
Text HLabel 4050 1450 0    50   Input ~ 0
SDN
Wire Wire Line
	4050 1450 4150 1450
Text HLabel 4050 1950 0    50   Output ~ 0
GPIO3
Wire Wire Line
	4050 1950 4150 1950
NoConn ~ 4150 1850
NoConn ~ 4150 1750
NoConn ~ 4150 1650
$Comp
L Device:C_Small C34
U 1 1 5D695AB3
P 4600 3400
F 0 "C34" V 4550 3450 50  0000 L CNN
F 1 "150nF" V 4600 2900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4600 3400 50  0001 C CNN
F 3 "~" H 4600 3400 50  0001 C CNN
	1    4600 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3500 4600 3600
Connection ~ 4600 3600
Wire Wire Line
	4600 3600 4750 3600
$Comp
L Device:C_Small C35
U 1 1 5D6A8643
P 4750 3400
F 0 "C35" V 4700 3450 50  0000 L CNN
F 1 "470pF" V 4750 2900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4750 3400 50  0001 C CNN
F 3 "~" H 4750 3400 50  0001 C CNN
	1    4750 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C36
U 1 1 5D6A91C5
P 4900 3400
F 0 "C36" V 4850 3450 50  0000 L CNN
F 1 "150nF" V 4900 2900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4900 3400 50  0001 C CNN
F 3 "~" H 4900 3400 50  0001 C CNN
	1    4900 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3500 4750 3600
Connection ~ 4750 3600
Wire Wire Line
	4750 3600 4900 3600
Wire Wire Line
	4900 3500 4900 3600
Connection ~ 4900 3600
Connection ~ 4750 1000
Wire Wire Line
	2650 1000 2900 1000
Connection ~ 2900 1000
Connection ~ 3400 1000
Wire Wire Line
	3400 1000 4750 1000
$Comp
L Device:L L8
U 1 1 5D70B2D1
P 5850 2450
F 0 "L8" V 5800 2250 50  0000 L CNN
F 1 "10uH" V 5800 2450 50  0000 L CNN
F 2 "Inductor_SMD:L_0805_2012Metric" H 5850 2450 50  0001 C CNN
F 3 "~" H 5850 2450 50  0001 C CNN
	1    5850 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 2300 5850 2200
Wire Wire Line
	5850 2200 5750 2200
Wire Wire Line
	5750 2200 5750 2400
Wire Wire Line
	5750 2400 5650 2400
Wire Wire Line
	5650 2500 5750 2500
Wire Wire Line
	5750 2500 5750 2700
Wire Wire Line
	5750 2700 5850 2700
Wire Wire Line
	5850 2700 5850 2600
$Comp
L Device:C_Small C38
U 1 1 5D71665C
P 5850 3400
F 0 "C38" V 5800 3450 50  0000 L CNN
F 1 "1uF" V 5850 3000 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5850 3400 50  0001 C CNN
F 3 "~" H 5850 3400 50  0001 C CNN
	1    5850 3400
	1    0    0    -1  
$EndComp
Connection ~ 5850 2700
Wire Wire Line
	5850 3500 5850 3600
Connection ~ 5850 3600
Wire Wire Line
	5850 3600 6050 3600
$Comp
L Device:C_Small C39
U 1 1 5D71DCDD
P 6050 3400
F 0 "C39" V 6000 3450 50  0000 L CNN
F 1 "150nF" V 6050 2900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6050 3400 50  0001 C CNN
F 3 "~" H 6050 3400 50  0001 C CNN
	1    6050 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 3600 6050 3500
Connection ~ 6050 3600
Wire Wire Line
	6050 2700 5850 2700
$Comp
L Device:C_Small C40
U 1 1 5D724A90
P 6250 3400
F 0 "C40" V 6200 3450 50  0000 L CNN
F 1 "100nF" V 6250 2900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6250 3400 50  0001 C CNN
F 3 "~" H 6250 3400 50  0001 C CNN
	1    6250 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C41
U 1 1 5D724D5D
P 6450 3400
F 0 "C41" V 6400 3450 50  0000 L CNN
F 1 "100pF" V 6450 2900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6450 3400 50  0001 C CNN
F 3 "~" H 6450 3400 50  0001 C CNN
	1    6450 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 3500 6250 3600
Connection ~ 6250 3600
Wire Wire Line
	6250 3600 6450 3600
Wire Wire Line
	6250 2700 6050 2700
Connection ~ 6050 2700
Wire Wire Line
	6450 2700 6250 2700
Connection ~ 6250 2700
Wire Wire Line
	6450 3500 6450 3600
$Comp
L Device:L L9
U 1 1 5D732CC0
P 6050 2450
F 0 "L9" V 6000 2250 50  0000 L CNN
F 1 "12nH" V 6000 2450 50  0000 L CNN
F 2 "Inductor_SMD:L_0603_1608Metric" H 6050 2450 50  0001 C CNN
F 3 "~" H 6050 2450 50  0001 C CNN
	1    6050 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3000 4750 3300
$Comp
L Device:C_Small C37
U 1 1 5D799EC3
P 5050 3400
F 0 "C37" V 5000 3450 50  0000 L CNN
F 1 "150nF" V 5050 2900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5050 3400 50  0001 C CNN
F 3 "~" H 5050 3400 50  0001 C CNN
	1    5050 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3500 5050 3600
Wire Wire Line
	5850 2700 5850 3300
Wire Wire Line
	6050 2700 6050 3300
Wire Wire Line
	6250 2700 6250 3300
Wire Wire Line
	6450 2700 6450 3300
Wire Wire Line
	1850 1000 1850 2100
Wire Wire Line
	2200 1000 2200 2100
Wire Wire Line
	2650 1000 2650 2100
Wire Wire Line
	2900 1000 2900 2100
Wire Wire Line
	3400 1000 3400 2100
Wire Wire Line
	3400 2300 3400 3600
Connection ~ 3400 3600
Wire Wire Line
	2900 2300 2900 3600
Connection ~ 2900 3600
Wire Wire Line
	2650 2300 2650 3600
Wire Wire Line
	2650 3600 2900 3600
Wire Wire Line
	2200 2300 2200 3600
Wire Wire Line
	1850 2300 1850 3600
Wire Wire Line
	1400 2300 1400 3600
$Comp
L Device:C_Small C19
U 1 1 5D944A48
P 1500 6800
F 0 "C19" V 1700 6800 50  0000 C CNN
F 1 "100pF" V 1600 6800 50  0000 C CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 1500 6800 50  0001 C CNN
F 3 "~" H 1500 6800 50  0001 C CNN
	1    1500 6800
	0    1    1    0   
$EndComp
Wire Wire Line
	6050 3600 6250 3600
Wire Wire Line
	3400 3600 4600 3600
Wire Wire Line
	4600 3000 4600 3300
Wire Wire Line
	4900 3000 4900 3300
Wire Wire Line
	4900 3600 5050 3600
Connection ~ 5050 3600
Wire Wire Line
	5300 3000 5300 3600
Connection ~ 5300 3600
Wire Wire Line
	5300 3600 5850 3600
Wire Wire Line
	5050 3600 5300 3600
Wire Wire Line
	5050 3000 5050 3300
Text Notes 1650 3750 0    50   ~ 0
VDD_SMPS
Text Notes 2100 3750 0    50   ~ 0
VDD_VCO
Text Notes 2600 3750 0    50   ~ 0
VDD_ANA
Text Notes 3250 3750 0    50   ~ 0
VDD_DIG
Text Notes 5650 4200 0    50   ~ 0
Coil Models:\n12nH: LQG18HN12NJ00 (0603)\n10uH: LQM21FN100M70L (0805)
Wire Wire Line
	1850 1000 2200 1000
Wire Wire Line
	2900 1000 3400 1000
Wire Wire Line
	2900 3600 3400 3600
Connection ~ 2200 1000
Connection ~ 2650 1000
Connection ~ 2200 3600
Connection ~ 2650 3600
Wire Wire Line
	1400 900  1400 1000
Wire Wire Line
	2200 1000 2650 1000
Wire Wire Line
	2200 3600 2650 3600
Wire Wire Line
	1850 3600 2200 3600
Wire Wire Line
	1400 3600 1850 3600
Connection ~ 1850 3600
Wire Wire Line
	1850 1000 1400 1000
Connection ~ 1850 1000
Connection ~ 1400 1000
Wire Wire Line
	1400 1000 1400 2100
$Comp
L Connector:Conn_Coaxial J2
U 1 1 5E50BED3
P 4850 5800
F 0 "J2" H 4950 5775 50  0000 L CNN
F 1 "Antenna" H 4950 5684 50  0000 L CNN
F 2 "Connector_Coaxial:U.FL_Hirose_U.FL-R-SMT-1_Vertical" H 4850 5800 50  0001 C CNN
F 3 " ~" H 4850 5800 50  0001 C CNN
	1    4850 5800
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C18
U 1 1 5E50CDD1
P 1500 6300
F 0 "C18" V 1700 6300 50  0000 C CNN
F 1 "100pF" V 1600 6300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 1500 6300 50  0001 C CNN
F 3 "~" H 1500 6300 50  0001 C CNN
	1    1500 6300
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 5E519115
P 4850 6100
F 0 "#PWR0109" H 4850 5850 50  0001 C CNN
F 1 "GND" H 4855 5927 50  0000 C CNN
F 2 "" H 4850 6100 50  0001 C CNN
F 3 "" H 4850 6100 50  0001 C CNN
	1    4850 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 6100 4850 6000
Text Label 5850 2100 2    50   ~ 0
TX
Wire Wire Line
	5650 1950 5850 1950
Wire Wire Line
	5650 1850 5850 1850
Text Label 5850 1850 2    50   ~ 0
RXp
Text Label 5850 1950 2    50   ~ 0
RXn
Text Label 1600 4850 0    50   ~ 0
TX
Text Label 1250 6300 0    50   ~ 0
RXp
Wire Wire Line
	1250 6300 1400 6300
Text Label 1250 6800 0    50   ~ 0
RXn
Wire Wire Line
	1400 6800 1250 6800
Wire Wire Line
	6050 2300 6050 2100
Wire Wire Line
	6050 2100 5650 2100
Wire Wire Line
	6050 2600 6050 2700
Wire Wire Line
	5650 1450 5900 1450
Wire Wire Line
	5900 1250 5900 1450
Wire Wire Line
	5900 1750 5900 1550
Wire Wire Line
	5900 1550 5650 1550
Wire Wire Line
	6850 1750 6850 1900
Wire Wire Line
	6050 1500 6050 1900
Wire Wire Line
	6050 1900 6850 1900
Connection ~ 6850 1900
Wire Wire Line
	6850 1900 6850 3600
$Comp
L Device:L L3
U 1 1 5E560F49
P 2050 4850
F 0 "L3" V 2240 4850 50  0000 C CNN
F 1 "4.3nH" V 2149 4850 50  0000 C CNN
F 2 "Inductor_SMD:L_0402_1005Metric" H 2050 4850 50  0001 C CNN
F 3 "~" H 2050 4850 50  0001 C CNN
	1    2050 4850
	0    -1   -1   0   
$EndComp
$Comp
L Device:C_Small C23
U 1 1 5E564ED8
P 2300 5050
F 0 "C23" H 2392 5096 50  0000 L CNN
F 1 "3.9pF" H 2392 5005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 2300 5050 50  0001 C CNN
F 3 "~" H 2300 5050 50  0001 C CNN
	1    2300 5050
	1    0    0    -1  
$EndComp
$Comp
L Device:L L4
U 1 1 5E56524B
P 2550 4850
F 0 "L4" V 2740 4850 50  0000 C CNN
F 1 "6.2nH" V 2649 4850 50  0000 C CNN
F 2 "Inductor_SMD:L_0402_1005Metric" H 2550 4850 50  0001 C CNN
F 3 "~" H 2550 4850 50  0001 C CNN
	1    2550 4850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2300 4950 2300 4850
Wire Wire Line
	2300 4850 2400 4850
Wire Wire Line
	2300 4850 2200 4850
Connection ~ 2300 4850
$Comp
L Device:C_Small C25
U 1 1 5E56CA32
P 2800 5050
F 0 "C25" H 2892 5096 50  0000 L CNN
F 1 "5.6pF" H 2892 5005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 2800 5050 50  0001 C CNN
F 3 "~" H 2800 5050 50  0001 C CNN
	1    2800 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 4950 2800 4850
Wire Wire Line
	2800 4850 2700 4850
$Comp
L Device:L L5
U 1 1 5E5754D7
P 3050 4850
F 0 "L5" V 3000 4800 50  0000 C CNN
F 1 "5.6nH" V 3150 4850 50  0000 C CNN
F 2 "Inductor_SMD:L_0402_1005Metric" H 3050 4850 50  0001 C CNN
F 3 "~" H 3050 4850 50  0001 C CNN
	1    3050 4850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2900 4850 2800 4850
Connection ~ 2800 4850
$Comp
L Device:C_Small C27
U 1 1 5E5799FC
P 3050 4650
F 0 "C27" V 2821 4650 50  0000 C CNN
F 1 "1.2pF" V 2912 4650 50  0000 C CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3050 4650 50  0001 C CNN
F 3 "~" H 3050 4650 50  0001 C CNN
	1    3050 4650
	0    1    1    0   
$EndComp
Wire Wire Line
	2950 4650 2800 4650
Wire Wire Line
	2800 4650 2800 4850
Wire Wire Line
	3200 4850 3300 4850
Wire Wire Line
	3300 4850 3300 4650
Wire Wire Line
	3300 4650 3150 4650
$Comp
L Device:C_Small C29
U 1 1 5E582985
P 3300 5050
F 0 "C29" H 3392 5096 50  0000 L CNN
F 1 "2.7pF" H 3392 5005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3300 5050 50  0001 C CNN
F 3 "~" H 3300 5050 50  0001 C CNN
	1    3300 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 4950 3300 4850
Connection ~ 3300 4850
$Comp
L Device:L L2
U 1 1 5E59AE79
P 2000 6550
F 0 "L2" H 2053 6596 50  0000 L CNN
F 1 "27nH" H 2053 6505 50  0000 L CNN
F 2 "Inductor_SMD:L_0402_1005Metric" H 2000 6550 50  0001 C CNN
F 3 "~" H 2000 6550 50  0001 C CNN
	1    2000 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 6400 2000 6300
Wire Wire Line
	2000 6700 2000 6800
$Comp
L power:GND #PWR0110
U 1 1 5E5AED3F
P 2000 7250
F 0 "#PWR0110" H 2000 7000 50  0001 C CNN
F 1 "GND" H 2005 7077 50  0000 C CNN
F 2 "" H 2000 7250 50  0001 C CNN
F 3 "" H 2000 7250 50  0001 C CNN
	1    2000 7250
	1    0    0    -1  
$EndComp
Connection ~ 2000 6800
$Comp
L power:GND #PWR0111
U 1 1 5E5C61E8
P 3300 5350
F 0 "#PWR0111" H 3300 5100 50  0001 C CNN
F 1 "GND" H 3305 5177 50  0000 C CNN
F 2 "" H 3300 5350 50  0001 C CNN
F 3 "" H 3300 5350 50  0001 C CNN
	1    3300 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 5150 3300 5250
Wire Wire Line
	2300 5150 2300 5250
Wire Wire Line
	2300 5250 2800 5250
Connection ~ 3300 5250
Wire Wire Line
	3300 5250 3300 5350
Wire Wire Line
	2800 5150 2800 5250
Connection ~ 2800 5250
Wire Wire Line
	2800 5250 3300 5250
$Comp
L Device:L L6
U 1 1 5E5DBA4C
P 3100 6800
F 0 "L6" H 3153 6846 50  0000 L CNN
F 1 "18nH" H 3153 6755 50  0000 L CNN
F 2 "Inductor_SMD:L_0402_1005Metric" H 3100 6800 50  0001 C CNN
F 3 "~" H 3100 6800 50  0001 C CNN
	1    3100 6800
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C21
U 1 1 5E5AF5FE
P 2000 7050
F 0 "C21" H 2092 7096 50  0000 L CNN
F 1 "1.8pF" H 2092 7005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 2000 7050 50  0001 C CNN
F 3 "~" H 2000 7050 50  0001 C CNN
	1    2000 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 6800 2000 6950
$Comp
L Device:L L7
U 1 1 5E5EE151
P 3500 6300
F 0 "L7" H 3553 6346 50  0000 L CNN
F 1 "18nH" H 3553 6255 50  0000 L CNN
F 2 "Inductor_SMD:L_0402_1005Metric" H 3500 6300 50  0001 C CNN
F 3 "~" H 3500 6300 50  0001 C CNN
	1    3500 6300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C31
U 1 1 5E5F7D61
P 3500 6800
F 0 "C31" H 3408 6754 50  0000 R CNN
F 1 "1.5pF" H 3408 6845 50  0000 R CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3500 6800 50  0001 C CNN
F 3 "~" H 3500 6800 50  0001 C CNN
	1    3500 6800
	-1   0    0    1   
$EndComp
Wire Wire Line
	6450 3600 6850 3600
Connection ~ 6450 3600
Connection ~ 6850 3600
Wire Wire Line
	6850 3600 6850 3700
$Comp
L Device:C_Small C28
U 1 1 5E62B6E4
P 3100 6300
F 0 "C28" H 3008 6254 50  0000 R CNN
F 1 "1.5pF" H 3008 6345 50  0000 R CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3100 6300 50  0001 C CNN
F 3 "~" H 3100 6300 50  0001 C CNN
	1    3100 6300
	-1   0    0    1   
$EndComp
Wire Wire Line
	2000 7150 2000 7250
Wire Wire Line
	3500 6050 3500 6150
Wire Wire Line
	3100 6200 3100 6050
Wire Wire Line
	3100 6050 3500 6050
Wire Wire Line
	3100 6400 3100 6550
Wire Wire Line
	3500 6450 3500 6550
$Comp
L power:GND #PWR0112
U 1 1 5E6A972D
P 3000 6550
F 0 "#PWR0112" H 3000 6300 50  0001 C CNN
F 1 "GND" V 3005 6422 50  0000 R CNN
F 2 "" H 3000 6550 50  0001 C CNN
F 3 "" H 3000 6550 50  0001 C CNN
	1    3000 6550
	0    1    1    0   
$EndComp
Wire Wire Line
	3000 6550 3100 6550
Connection ~ 3100 6550
Wire Wire Line
	3100 6550 3100 6650
Connection ~ 2000 6300
Wire Wire Line
	3100 6950 3100 7050
Wire Wire Line
	3100 7050 3500 7050
Wire Wire Line
	3500 7050 3500 6900
$Comp
L Device:C_Small C32
U 1 1 5E6F433D
P 4100 5600
F 0 "C32" H 4192 5646 50  0000 L CNN
F 1 "100pF" H 4192 5555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4100 5600 50  0001 C CNN
F 3 "~" H 4100 5600 50  0001 C CNN
	1    4100 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C33
U 1 1 5E6F4B01
P 4100 6000
F 0 "C33" H 4192 6046 50  0000 L CNN
F 1 "100pF" H 4192 5955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4100 6000 50  0001 C CNN
F 3 "~" H 4100 6000 50  0001 C CNN
	1    4100 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 5800 4100 5900
Wire Wire Line
	4100 5800 4650 5800
Wire Wire Line
	4100 5800 4100 5700
Connection ~ 4100 5800
Wire Wire Line
	3500 6550 4100 6550
Wire Wire Line
	4100 6550 4100 6100
Connection ~ 3500 6550
Wire Wire Line
	3500 6550 3500 6700
Text Notes 3400 7600 0    50   ~ 0
RX LC Balun
Wire Wire Line
	1600 6800 2000 6800
Wire Wire Line
	1600 6300 2000 6300
Wire Wire Line
	2750 6300 2750 6050
Wire Wire Line
	2750 6050 3100 6050
Wire Wire Line
	2000 6300 2750 6300
Connection ~ 3100 6050
Wire Wire Line
	2000 6800 2750 6800
Wire Wire Line
	2750 6800 2750 7050
Wire Wire Line
	2750 7050 3100 7050
Connection ~ 3100 7050
Wire Notes Line
	1800 5900 1800 7500
Wire Notes Line
	1800 7500 2400 7500
Wire Notes Line
	2400 7500 2400 5900
Wire Notes Line
	2400 5900 1800 5900
Wire Notes Line
	2500 5900 2500 7500
Wire Notes Line
	2500 7500 3850 7500
Wire Notes Line
	3850 7500 3850 5900
Wire Notes Line
	3850 5900 2500 5900
Text Notes 1950 7600 0    50   ~ 0
RX Matching
Wire Wire Line
	4100 4850 3300 4850
Wire Wire Line
	1600 4850 1900 4850
Wire Notes Line
	1800 5650 3850 5650
Wire Notes Line
	3850 5650 3850 4350
Wire Notes Line
	3850 4350 1800 4350
Wire Notes Line
	1800 4350 1800 5650
Wire Wire Line
	4100 4850 4100 5500
Text Notes 2950 5750 0    50   ~ 0
TX Matching & Filtering
Text Notes 6950 1550 0    50   ~ 0
XTAL Model:\n7M50072001
Text Notes 4900 5300 0    50   ~ 0
TX Matching & Filtering (all 0402)\n4.3nH LQW15AN4N3G00\n6.2nH LQW15AN6N2G00\n5.6nH LQW15AN5N6G00\n\n3.9pF GRM1555C1H3R9BA01\n5.6pF GRM1555C1H5R6DZ01\n1.2pF GRM1555C1H1R2CZ01\n2.7pF GRM1555C1H2R7CZ01
Text Notes 5550 5900 0    50   ~ 0
Antenna (all 0402)\n100pF GRM1555C1E101JA01 (one TX, one RX)
Text Notes 4450 7300 0    50   ~ 0
RXp / RXn (all 0402)\n100pF GRM1555C1H101JA01 (one TX, one RX)\n\nRX Matching (all 0402)\n27nH LQG15HS27NJ02\n1.8pF GRM1555C1H1R8CZ01\n\nRX LC Balun (all 0402)\n18nH LQG15HS18NJ02 (to GND, to out)\n1.5pF GRM1555C1H1R5CZ01 (to GND, to out)\n
$EndSCHEMATC