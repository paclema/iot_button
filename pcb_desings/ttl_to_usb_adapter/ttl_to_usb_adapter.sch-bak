EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L ttl_to_usb_adapter:USB_to_TTL U1
U 1 1 60E490DC
P 1400 2050
F 0 "U1" H 1400 1900 50  0000 C CNN
F 1 "USB_to_TTL" H 1400 2000 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 1400 2050 50  0001 C CNN
F 3 "" H 1400 2050 50  0001 C CNN
	1    1400 2050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2600 1100 3050 1100
Wire Wire Line
	3050 1100 3050 1450
Wire Wire Line
	2600 2050 3050 2050
Wire Wire Line
	3050 2050 3050 1550
$Comp
L Transistor_BJT:UMH3N Q1
U 1 1 60E48787
P 2400 1300
F 0 "Q1" H 2691 1346 50  0000 L CNN
F 1 "UMH3N" H 2691 1255 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 2405 860 50  0001 C CNN
F 3 "http://rohmfs.rohm.com/en/products/databook/datasheet/discrete/transistor/digital/emh3t2r-e.pdf" H 2550 1300 50  0001 C CNN
	1    2400 1300
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:UMH3N Q1
U 2 1 60E46BD1
P 2400 1850
F 0 "Q1" H 2691 1804 50  0000 L CNN
F 1 "UMH3N" H 2691 1895 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 2405 1410 50  0001 C CNN
F 3 "http://rohmfs.rohm.com/en/products/databook/datasheet/discrete/transistor/digital/emh3t2r-e.pdf" H 2550 1850 50  0001 C CNN
	2    2400 1850
	1    0    0    1   
$EndComp
Wire Wire Line
	1650 1400 1850 1400
Wire Wire Line
	1850 1400 1850 1850
Wire Wire Line
	1850 1850 2150 1850
Wire Wire Line
	1650 1500 1950 1500
Wire Wire Line
	2050 1500 2050 1300
Wire Wire Line
	2050 1300 2200 1300
Wire Wire Line
	2600 1500 2150 1500
Wire Wire Line
	2150 1500 2150 1850
Connection ~ 2150 1850
Wire Wire Line
	2150 1850 2200 1850
Wire Wire Line
	2600 1650 1950 1650
Wire Wire Line
	1950 1650 1950 1500
Connection ~ 1950 1500
Wire Wire Line
	1950 1500 2050 1500
Wire Wire Line
	1650 1650 1800 1650
Wire Wire Line
	1650 1750 1800 1750
Text Label 1800 1650 2    50   ~ 0
TXD
Text Label 1800 1750 2    50   ~ 0
RXD
Text Label 3200 1800 0    50   ~ 0
TXD
Text Label 3200 1700 0    50   ~ 0
RXD
Wire Wire Line
	1650 1250 1800 1250
Text Label 1800 1250 2    50   ~ 0
3V3
Wire Wire Line
	1650 1900 1800 1900
Text Label 1800 1900 2    50   ~ 0
GND
Text Label 3200 1950 0    50   ~ 0
GND
Text Label 3150 1200 0    50   ~ 0
3V3
Text Label 3150 1350 0    50   ~ 0
CH_PD
Wire Wire Line
	3150 1200 3450 1200
Wire Wire Line
	3150 1350 3450 1350
Wire Wire Line
	3050 1450 3450 1450
Wire Wire Line
	3050 1550 3450 1550
Wire Wire Line
	3200 1700 3450 1700
Wire Wire Line
	3200 1800 3450 1800
Wire Wire Line
	3200 1950 3450 1950
Text Label 5200 1750 0    50   ~ 0
3V3
Text Label 5200 1600 0    50   ~ 0
CH_PD
Text Label 5200 1450 0    50   ~ 0
GND
$Comp
L Connector:Conn_01x03_Male J1
U 1 1 60E4CC7C
P 5850 1600
F 0 "J1" H 5822 1624 50  0000 R CNN
F 1 "CH_PD_selector" H 5822 1533 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 5850 1600 50  0001 C CNN
F 3 "~" H 5850 1600 50  0001 C CNN
	1    5850 1600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5550 1450 5550 1500
Wire Wire Line
	5550 1500 5650 1500
Wire Wire Line
	5200 1450 5550 1450
Wire Wire Line
	5200 1600 5650 1600
Wire Wire Line
	5500 1750 5500 1700
Wire Wire Line
	5500 1700 5650 1700
$Comp
L Device:LED D1
U 1 1 60E53F08
P 2150 3200
F 0 "D1" H 2143 2945 50  0000 C CNN
F 1 "EN" H 2143 3036 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 2150 3200 50  0001 C CNN
F 3 "~" H 2150 3200 50  0001 C CNN
	1    2150 3200
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 60E5BC39
P 2650 3200
F 0 "R1" V 2443 3200 50  0000 C CNN
F 1 "68" V 2534 3200 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2580 3200 50  0001 C CNN
F 3 "~" H 2650 3200 50  0001 C CNN
	1    2650 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 3200 2500 3200
Wire Wire Line
	2800 3200 3050 3200
Text Label 1700 3200 0    50   ~ 0
CH_PD
Wire Wire Line
	1650 3200 2000 3200
Text Label 2850 3200 0    50   ~ 0
GND
$Comp
L Device:LED D2
U 1 1 60E61A03
P 2150 3600
F 0 "D2" H 2143 3345 50  0000 C CNN
F 1 "RST" H 2143 3436 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 2150 3600 50  0001 C CNN
F 3 "~" H 2150 3600 50  0001 C CNN
	1    2150 3600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 60E61A09
P 2650 3600
F 0 "R2" V 2443 3600 50  0000 C CNN
F 1 "68" V 2534 3600 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2580 3600 50  0001 C CNN
F 3 "~" H 2650 3600 50  0001 C CNN
	1    2650 3600
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 3600 2500 3600
Wire Wire Line
	2800 3600 3050 3600
Text Label 1700 3600 0    50   ~ 0
RST
Wire Wire Line
	1650 3600 2000 3600
Text Label 2850 3600 0    50   ~ 0
GND
$Comp
L Device:LED D3
U 1 1 60E6289D
P 2150 3950
F 0 "D3" H 2143 3695 50  0000 C CNN
F 1 "GPIO0" H 2143 3786 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 2150 3950 50  0001 C CNN
F 3 "~" H 2150 3950 50  0001 C CNN
	1    2150 3950
	-1   0    0    1   
$EndComp
$Comp
L Device:R R3
U 1 1 60E628A3
P 2650 3950
F 0 "R3" V 2443 3950 50  0000 C CNN
F 1 "68" V 2534 3950 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2580 3950 50  0001 C CNN
F 3 "~" H 2650 3950 50  0001 C CNN
	1    2650 3950
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 3950 2500 3950
Wire Wire Line
	2800 3950 3050 3950
Text Label 1700 3950 0    50   ~ 0
GPIO0
Wire Wire Line
	1650 3950 2000 3950
Text Label 2850 3950 0    50   ~ 0
GND
Text Notes 1100 4250 0    50   ~ 0
Resistances for Green led using https://www.hobby-hour.com/electronics/ledcalc.php
Text Label 3150 1450 0    50   ~ 0
RST
Text Label 3150 1550 0    50   ~ 0
GPIO0
Wire Wire Line
	5200 1750 5500 1750
Text Label 1800 1400 2    50   ~ 0
RTS
Text Label 1800 1500 2    50   ~ 0
DTR
$Comp
L ttl_to_usb_adapter:TTL_UART_adapter_iot_postbox U2
U 1 1 60E63523
P 3700 2050
F 0 "U2" H 3650 1800 50  0000 L CNN
F 1 "TTL_UART_adapter_iot_postbox" H 3100 1950 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x07_P2.54mm_Vertical" H 3500 2100 50  0001 C CNN
F 3 "" H 3500 2100 50  0001 C CNN
	1    3700 2050
	1    0    0    -1  
$EndComp
$EndSCHEMATC
