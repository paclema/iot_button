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
L wemos_mini:WeMos_mini U?
U 1 1 60685267
P 2450 1600
F 0 "U?" H 2450 2343 60  0000 C CNN
F 1 "WeMos_mini" H 2450 2237 60  0000 C CNN
F 2 "" H 3000 900 60  0000 C CNN
F 3 "http://www.wemos.cc/Products/d1_mini.html" H 2450 2131 60  0000 C CNN
	1    2450 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 60685B83
P 3500 2250
F 0 "R1" H 3570 2296 50  0000 L CNN
F 1 "10k" H 3570 2205 50  0000 L CNN
F 2 "" V 3430 2250 50  0001 C CNN
F 3 "~" H 3500 2250 50  0001 C CNN
	1    3500 2250
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Reed SW?
U 1 1 60687B72
P 3500 1850
F 0 "SW?" H 3500 2072 50  0000 C CNN
F 1 "SW_Reed" H 3500 1981 50  0000 C CNN
F 2 "" H 3500 1850 50  0001 C CNN
F 3 "~" H 3500 1850 50  0001 C CNN
	1    3500 1850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 60688FB5
P 3750 2250
F 0 "R2" H 3820 2296 50  0000 L CNN
F 1 "10k" H 3820 2205 50  0000 L CNN
F 2 "" V 3680 2250 50  0001 C CNN
F 3 "~" H 3750 2250 50  0001 C CNN
	1    3750 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:LED ldr
U 1 1 6068A202
P 3750 1850
F 0 "ldr" V 3697 1930 50  0000 L CNN
F 1 "LED" V 3788 1930 50  0000 L CNN
F 2 "" H 3750 1850 50  0001 C CNN
F 3 "~" H 3750 1850 50  0001 C CNN
	1    3750 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	3750 1700 3750 1250
Wire Wire Line
	3750 2100 3750 2000
$Comp
L power:GND #PWR?
U 1 1 6068DD9C
P 3750 2650
F 0 "#PWR?" H 3750 2400 50  0001 C CNN
F 1 "GND" H 3755 2477 50  0000 C CNN
F 2 "" H 3750 2650 50  0001 C CNN
F 3 "" H 3750 2650 50  0001 C CNN
	1    3750 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2650 3750 2400
Wire Wire Line
	2950 1250 3750 1250
Wire Wire Line
	3500 2100 3500 2050
$Comp
L power:GND #PWR?
U 1 1 6068FDBC
P 3500 2650
F 0 "#PWR?" H 3500 2400 50  0001 C CNN
F 1 "GND" H 3505 2477 50  0000 C CNN
F 2 "" H 3500 2650 50  0001 C CNN
F 3 "" H 3500 2650 50  0001 C CNN
	1    3500 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 2650 3500 2400
Wire Wire Line
	1250 2400 1250 1250
Wire Wire Line
	1250 1250 1950 1250
$Comp
L power:GND #PWR?
U 1 1 60691CA5
P 1250 3200
F 0 "#PWR?" H 1250 2950 50  0001 C CNN
F 1 "GND" H 1255 3027 50  0000 C CNN
F 2 "" H 1250 3200 50  0001 C CNN
F 3 "" H 1250 3200 50  0001 C CNN
	1    1250 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 3200 1250 3150
$Comp
L LED:WS2812B D?
U 1 1 60690390
P 1250 2700
F 0 "D?" H 1594 2746 50  0000 L CNN
F 1 "WS2812B" H 1594 2655 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 1300 2400 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 1350 2325 50  0001 L TNN
	1    1250 2700
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Distance:VL53L1CXV0FY1 U?
U 1 1 60692D8B
P 2550 3150
F 0 "U?" V 2925 3150 50  0000 C CNN
F 1 "VL53L1CXV0FY1" V 3016 3150 50  0000 C CNN
F 2 "Sensor_Distance:ST_VL53L1x" H 3225 2600 50  0001 C CNN
F 3 "https://www.st.com/resource/en/datasheet/vl53l1x.pdf" H 2650 3150 50  0001 C CNN
	1    2550 3150
	0    1    1    0   
$EndComp
Wire Wire Line
	2250 2750 2250 2350
Wire Wire Line
	2250 2350 3100 2350
Wire Wire Line
	3100 2350 3100 1650
Wire Wire Line
	3100 1650 2950 1650
Wire Wire Line
	2950 1550 3500 1550
Wire Wire Line
	3500 1550 3500 1650
Wire Wire Line
	1950 3150 1250 3150
Connection ~ 1250 3150
Wire Wire Line
	1250 3150 1250 3000
Wire Wire Line
	3150 3150 4200 3150
Wire Wire Line
	4200 3150 4200 1250
Wire Wire Line
	4200 1250 3750 1250
Connection ~ 3750 1250
Wire Wire Line
	2450 2750 2450 2500
Wire Wire Line
	2450 2500 1700 2500
Wire Wire Line
	1700 2500 1700 1750
Wire Wire Line
	1700 1750 1950 1750
Wire Wire Line
	2550 2750 2550 2250
Wire Wire Line
	2550 2250 1500 2250
Wire Wire Line
	1500 2250 1500 1650
Wire Wire Line
	1500 1650 1950 1650
Wire Wire Line
	950  750  3200 750 
Wire Wire Line
	3200 750  3200 1450
Wire Wire Line
	3200 1450 2950 1450
Wire Wire Line
	950  750  950  2700
$EndSCHEMATC
