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
Wire Wire Line
	1350 1950 1050 1950
$Comp
L power:GND #PWR03
U 1 1 60B14C39
P 1950 3200
F 0 "#PWR03" H 1950 2950 50  0001 C CNN
F 1 "GND" H 1955 3027 50  0000 C CNN
F 2 "" H 1950 3200 50  0001 C CNN
F 3 "" H 1950 3200 50  0001 C CNN
	1    1950 3200
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR06
U 1 1 60B15876
P 1350 5350
F 0 "#PWR06" H 1350 5200 50  0001 C CNN
F 1 "+3V3" H 1365 5523 50  0000 C CNN
F 2 "" H 1350 5350 50  0001 C CNN
F 3 "" H 1350 5350 50  0001 C CNN
	1    1350 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 60B16984
P 1200 1400
F 0 "R1" H 1270 1446 50  0000 L CNN
F 1 "10k" H 1270 1355 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1130 1400 50  0001 C CNN
F 3 "~" H 1200 1400 50  0001 C CNN
	1    1200 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR01
U 1 1 60B18933
P 1200 1150
F 0 "#PWR01" H 1200 1000 50  0001 C CNN
F 1 "+3V3" H 1215 1323 50  0000 C CNN
F 2 "" H 1200 1150 50  0001 C CNN
F 3 "" H 1200 1150 50  0001 C CNN
	1    1200 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 3050 1950 3200
Wire Wire Line
	1350 1750 1200 1750
Wire Wire Line
	1200 1750 1200 1550
Wire Wire Line
	1200 1250 1200 1150
Text Label 1050 1950 0    50   ~ 0
CH_PD
Text Label 5000 2000 0    50   ~ 0
CH_PD
$Comp
L Device:R R5
U 1 1 60B19B02
P 5350 2250
F 0 "R5" H 5420 2296 50  0000 L CNN
F 1 "10k" H 5420 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5280 2250 50  0001 C CNN
F 3 "~" H 5350 2250 50  0001 C CNN
	1    5350 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2000 5350 2000
Wire Wire Line
	5350 2000 5350 2100
$Comp
L power:GND #PWR08
U 1 1 60B1B637
P 5350 2550
F 0 "#PWR08" H 5350 2300 50  0001 C CNN
F 1 "GND" H 5355 2377 50  0000 C CNN
F 2 "" H 5350 2550 50  0001 C CNN
F 3 "" H 5350 2550 50  0001 C CNN
	1    5350 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 2550 5350 2400
Wire Wire Line
	2550 2650 2800 2650
Wire Wire Line
	2800 2650 2800 2900
$Comp
L Device:R R3
U 1 1 60B1CEA6
P 2800 3050
F 0 "R3" H 2870 3096 50  0000 L CNN
F 1 "10k" H 2870 3005 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2730 3050 50  0001 C CNN
F 3 "~" H 2800 3050 50  0001 C CNN
	1    2800 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 60B1D6C8
P 2800 3300
F 0 "#PWR04" H 2800 3050 50  0001 C CNN
F 1 "GND" H 2805 3127 50  0000 C CNN
F 2 "" H 2800 3300 50  0001 C CNN
F 3 "" H 2800 3300 50  0001 C CNN
	1    2800 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 3300 2800 3200
Text Label 2600 2350 0    50   ~ 0
GPIO12
Text Label 2600 2550 0    50   ~ 0
GPIO14
Wire Wire Line
	2550 2350 2900 2350
Wire Wire Line
	2550 2450 2900 2450
Wire Wire Line
	2550 2550 2900 2550
Text Label 2600 2450 0    50   ~ 0
GPIO13
$Comp
L Device:D D2
U 1 1 60B221AD
P 5600 1400
F 0 "D2" H 5600 1617 50  0000 C CNN
F 1 "1N4007" H 5600 1526 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 5600 1400 50  0001 C CNN
F 3 "~" H 5600 1400 50  0001 C CNN
	1    5600 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:D D3
U 1 1 60B23F6F
P 5600 1750
F 0 "D3" H 5600 1967 50  0000 C CNN
F 1 "1N4007" H 5600 1876 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 5600 1750 50  0001 C CNN
F 3 "~" H 5600 1750 50  0001 C CNN
	1    5600 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:D D1
U 1 1 60B242E1
P 5600 1050
F 0 "D1" H 5600 1267 50  0000 C CNN
F 1 "1N4007" H 5600 1176 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123" H 5600 1050 50  0001 C CNN
F 3 "~" H 5600 1050 50  0001 C CNN
	1    5600 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 1400 6050 1400
Text Label 5750 1050 0    50   ~ 0
GPIO12
Wire Wire Line
	5750 1750 6050 1750
Wire Wire Line
	5750 1050 6050 1050
Text Label 5750 1400 0    50   ~ 0
GPIO13
Text Label 5750 1750 0    50   ~ 0
GPIO14
Wire Wire Line
	5350 2000 5350 1750
Wire Wire Line
	5350 1400 5450 1400
Connection ~ 5350 2000
Wire Wire Line
	5450 1750 5350 1750
Connection ~ 5350 1750
Wire Wire Line
	5350 1050 5450 1050
Wire Wire Line
	5350 1050 5350 1400
Connection ~ 5350 1400
Wire Wire Line
	5350 1400 5350 1750
$Comp
L power:+3V3 #PWR09
U 1 1 60B398B9
P 6850 1050
F 0 "#PWR09" H 6850 900 50  0001 C CNN
F 1 "+3V3" H 6865 1223 50  0000 C CNN
F 2 "" H 6850 1050 50  0001 C CNN
F 3 "" H 6850 1050 50  0001 C CNN
	1    6850 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1300 6850 1500
Wire Wire Line
	6350 1500 6850 1500
Text Label 6700 1500 2    50   ~ 0
GPIO13
$Comp
L Device:R R6
U 1 1 60B3CBFB
P 6850 1800
F 0 "R6" H 6920 1846 50  0000 L CNN
F 1 "10k" H 6920 1755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6780 1800 50  0001 C CNN
F 3 "~" H 6850 1800 50  0001 C CNN
	1    6850 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1650 6850 1500
Connection ~ 6850 1500
$Comp
L power:GND #PWR010
U 1 1 60B3DA5A
P 6850 2150
F 0 "#PWR010" H 6850 1900 50  0001 C CNN
F 1 "GND" H 6855 1977 50  0000 C CNN
F 2 "" H 6850 2150 50  0001 C CNN
F 3 "" H 6850 2150 50  0001 C CNN
	1    6850 2150
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR011
U 1 1 60B42577
P 8150 1050
F 0 "#PWR011" H 8150 900 50  0001 C CNN
F 1 "+3V3" H 8165 1223 50  0000 C CNN
F 2 "" H 8150 1050 50  0001 C CNN
F 3 "" H 8150 1050 50  0001 C CNN
	1    8150 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 1300 8150 1500
Wire Wire Line
	7650 1500 8150 1500
Text Label 8000 1500 2    50   ~ 0
GPIO14
$Comp
L Device:R R7
U 1 1 60B42581
P 8150 1800
F 0 "R7" H 8220 1846 50  0000 L CNN
F 1 "10k" H 8220 1755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8080 1800 50  0001 C CNN
F 3 "~" H 8150 1800 50  0001 C CNN
	1    8150 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 1650 8150 1500
Connection ~ 8150 1500
$Comp
L power:GND #PWR012
U 1 1 60B42589
P 8150 2150
F 0 "#PWR012" H 8150 1900 50  0001 C CNN
F 1 "GND" H 8155 1977 50  0000 C CNN
F 2 "" H 8150 2150 50  0001 C CNN
F 3 "" H 8150 2150 50  0001 C CNN
	1    8150 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 1950 8150 2150
$Comp
L Device:R R2
U 1 1 60B5027F
P 3050 1600
F 0 "R2" H 3120 1646 50  0000 L CNN
F 1 "10k" H 3120 1555 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2980 1600 50  0001 C CNN
F 3 "~" H 3050 1600 50  0001 C CNN
	1    3050 1600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R4
U 1 1 60B50B9E
P 3350 1800
F 0 "R4" H 3420 1846 50  0000 L CNN
F 1 "10k" H 3420 1755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3280 1800 50  0001 C CNN
F 3 "~" H 3350 1800 50  0001 C CNN
	1    3350 1800
	-1   0    0    1   
$EndComp
Wire Wire Line
	3050 1450 3050 1350
Wire Wire Line
	3350 1350 3350 1650
Wire Wire Line
	3050 1350 3200 1350
$Comp
L power:+3V3 #PWR05
U 1 1 60B58812
P 3200 1150
F 0 "#PWR05" H 3200 1000 50  0001 C CNN
F 1 "+3V3" H 3215 1323 50  0000 C CNN
F 2 "" H 3200 1150 50  0001 C CNN
F 3 "" H 3200 1150 50  0001 C CNN
	1    3200 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 1150 3200 1350
Connection ~ 3200 1350
Wire Wire Line
	3200 1350 3350 1350
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 60B5CB01
P 8350 1200
F 0 "J2" H 8430 1192 50  0000 L CNN
F 1 "Switch_2" H 8430 1101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8350 1200 50  0001 C CNN
F 3 "~" H 8350 1200 50  0001 C CNN
	1    8350 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1200 6850 1050
Wire Wire Line
	8150 1050 8150 1200
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 60B69ADF
P 7050 1200
F 0 "J1" H 7130 1192 50  0000 L CNN
F 1 "Switch_1" H 7130 1101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7050 1200 50  0001 C CNN
F 3 "~" H 7050 1200 50  0001 C CNN
	1    7050 1200
	1    0    0    -1  
$EndComp
Text Label 1350 5550 1    50   ~ 0
VCC
$Comp
L Device:C C2
U 1 1 60B72B1E
P 1600 5850
F 0 "C2" H 1715 5896 50  0000 L CNN
F 1 "100nF" H 1715 5805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1638 5700 50  0001 C CNN
F 3 "~" H 1600 5850 50  0001 C CNN
	1    1600 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 5350 1350 5650
Wire Wire Line
	1600 5700 1600 5650
Wire Wire Line
	1600 6050 1600 6000
$Comp
L power:GND #PWR07
U 1 1 60B760F1
P 1350 6100
F 0 "#PWR07" H 1350 5850 50  0001 C CNN
F 1 "GND" H 1355 5927 50  0000 C CNN
F 2 "" H 1350 6100 50  0001 C CNN
F 3 "" H 1350 6100 50  0001 C CNN
	1    1350 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 6100 1350 6050
Wire Wire Line
	1100 6000 1100 6050
Wire Wire Line
	1100 5700 1100 5650
$Comp
L Device:C C1
U 1 1 60B6FA1D
P 1100 5850
F 0 "C1" H 1215 5896 50  0000 L CNN
F 1 "10uF" H 1215 5805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1138 5700 50  0001 C CNN
F 3 "~" H 1100 5850 50  0001 C CNN
	1    1100 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 6050 1350 6050
Wire Wire Line
	1100 5650 1350 5650
Wire Wire Line
	1350 5650 1600 5650
Connection ~ 1350 5650
Wire Wire Line
	1350 6050 1600 6050
Connection ~ 1350 6050
$Comp
L power:+3V3 #PWR02
U 1 1 60B815E9
P 1950 1150
F 0 "#PWR02" H 1950 1000 50  0001 C CNN
F 1 "+3V3" H 1965 1323 50  0000 C CNN
F 2 "" H 1950 1150 50  0001 C CNN
F 3 "" H 1950 1150 50  0001 C CNN
	1    1950 1150
	1    0    0    -1  
$EndComp
NoConn ~ 2550 2150
NoConn ~ 2550 2250
NoConn ~ 2550 2750
NoConn ~ 1350 2350
NoConn ~ 1350 2450
NoConn ~ 1350 2550
NoConn ~ 1350 2650
NoConn ~ 1350 2750
NoConn ~ 1350 2850
NoConn ~ 1350 2150
Wire Wire Line
	6850 2150 6850 1950
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 60BC9D92
P 2850 4350
F 0 "J3" H 2930 4342 50  0000 L CNN
F 1 "Battery" H 2930 4251 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2850 4350 50  0001 C CNN
F 3 "~" H 2850 4350 50  0001 C CNN
	1    2850 4350
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR013
U 1 1 60BCAC94
P 3200 4200
F 0 "#PWR013" H 3200 4050 50  0001 C CNN
F 1 "+3V3" H 3215 4373 50  0000 C CNN
F 2 "" H 3200 4200 50  0001 C CNN
F 3 "" H 3200 4200 50  0001 C CNN
	1    3200 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 60BCB652
P 3200 4400
F 0 "#PWR014" H 3200 4150 50  0001 C CNN
F 1 "GND" H 3205 4227 50  0000 C CNN
F 2 "" H 3200 4400 50  0001 C CNN
F 3 "" H 3200 4400 50  0001 C CNN
	1    3200 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 4250 3200 4250
Wire Wire Line
	3200 4250 3200 4200
Wire Wire Line
	3200 4400 3200 4350
Wire Wire Line
	3200 4350 3050 4350
$Comp
L iot-postbox:TTL_to_USB U2
U 1 1 60B4E2F6
P 1550 4750
F 0 "U2" H 1728 5271 50  0000 L CNN
F 1 "TTL_to_USB" H 1728 5180 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 1350 4800 50  0001 C CNN
F 3 "" H 1350 4800 50  0001 C CNN
	1    1550 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 4100 1050 4100
Wire Wire Line
	1300 4250 1050 4250
Wire Wire Line
	1300 4350 1050 4350
Wire Wire Line
	1300 4500 1050 4500
Wire Wire Line
	1300 4650 1150 4650
NoConn ~ 1300 3900
Text Label 1200 1750 0    50   ~ 0
RST
Text Label 1050 4100 0    50   ~ 0
RST
$Comp
L RF_Module:ESP-12F U1
U 1 1 60AE687C
P 1950 2350
F 0 "U1" H 1950 3331 50  0000 C CNN
F 1 "ESP-12F" H 1950 3240 50  0000 C CNN
F 2 "RF_Module:ESP-12E" H 1950 2350 50  0001 C CNN
F 3 "http://wiki.ai-thinker.com/_media/esp8266/esp8266_series_modules_user_manual_v1.1.pdf" H 1600 2450 50  0001 C CNN
	1    1950 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 1850 2750 1850
Wire Wire Line
	2550 2050 2750 2050
Text Label 2600 1850 0    50   ~ 0
TX
Text Label 2600 2050 0    50   ~ 0
RX
Text Label 1050 4350 0    50   ~ 0
RX
Text Label 1050 4250 0    50   ~ 0
TX
Text Label 2600 1750 0    50   ~ 0
GPIO0
Wire Wire Line
	2550 1750 3050 1750
Wire Wire Line
	2550 1950 3350 1950
Text Label 1050 4500 0    50   ~ 0
GPIO0
$Comp
L power:GND #PWR0101
U 1 1 60B68C08
P 1150 4750
F 0 "#PWR0101" H 1150 4500 50  0001 C CNN
F 1 "GND" H 1155 4577 50  0000 C CNN
F 2 "" H 1150 4750 50  0001 C CNN
F 3 "" H 1150 4750 50  0001 C CNN
	1    1150 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 4750 1150 4650
Wire Wire Line
	1950 1550 1950 1150
$EndSCHEMATC
