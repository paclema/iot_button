EESchema Schematic File Version 4
LIBS:radar_d1_mini_pcb-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Optimun_radar"
Date "2021-01-28"
Rev "v0.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Screw_Terminal_01x03 InputPower1
U 1 1 60118A5D
P 7250 2050
F 0 "InputPower1" H 7170 1817 50  0000 C CNN
F 1 "Screw_Terminal_01x03" H 7330 2001 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-3-5.08_1x03_P5.08mm_Horizontal" H 7250 2050 50  0001 C CNN
F 3 "~" H 7250 2050 50  0001 C CNN
	1    7250 2050
	-1   0    0    1   
$EndComp
Text GLabel 5050 1900 0    50   Input ~ 0
Servo1_control
Text GLabel 3150 1900 2    50   Input ~ 0
Servo1_control
Text GLabel 3150 2050 2    50   Input ~ 0
Servo2_control
Text GLabel 6350 1900 0    50   Input ~ 0
Servo2_control
$Comp
L power:GND #PWR03
U 1 1 60119BEE
P 6150 2550
F 0 "#PWR03" H 6150 2300 50  0001 C CNN
F 1 "GND" H 6155 2377 50  0000 C CNN
F 2 "" H 6150 2550 50  0001 C CNN
F 3 "" H 6150 2550 50  0001 C CNN
	1    6150 2550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 60119CFC
P 3100 2300
F 0 "#PWR02" H 3100 2050 50  0001 C CNN
F 1 "GND" H 3105 2127 50  0000 C CNN
F 2 "" H 3100 2300 50  0001 C CNN
F 3 "" H 3100 2300 50  0001 C CNN
	1    3100 2300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR04
U 1 1 60119F0D
P 7700 1950
F 0 "#PWR04" H 7700 1800 50  0001 C CNN
F 1 "+5V" H 7715 2123 50  0000 C CNN
F 2 "" H 7700 1950 50  0001 C CNN
F 3 "" H 7700 1950 50  0001 C CNN
	1    7700 1950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 60119F27
P 2900 2300
F 0 "#PWR01" H 2900 2150 50  0001 C CNN
F 1 "+5V" H 2915 2473 50  0000 C CNN
F 2 "" H 2900 2300 50  0001 C CNN
F 3 "" H 2900 2300 50  0001 C CNN
	1    2900 2300
	-1   0    0    1   
$EndComp
Wire Wire Line
	6150 2450 6150 2150
Wire Wire Line
	6150 2150 6550 2150
Wire Wire Line
	5100 2450 5100 2150
Wire Wire Line
	5100 2150 5300 2150
Connection ~ 6150 2450
Wire Wire Line
	6150 2550 6150 2450
$Comp
L power:VCC #PWR0101
U 1 1 6011B6A5
P 7900 1950
F 0 "#PWR0101" H 7900 1800 50  0001 C CNN
F 1 "VCC" H 7917 2123 50  0000 C CNN
F 2 "" H 7900 1950 50  0001 C CNN
F 3 "" H 7900 1950 50  0001 C CNN
	1    7900 1950
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0102
U 1 1 6011B87B
P 6450 2050
F 0 "#PWR0102" H 6450 1900 50  0001 C CNN
F 1 "VCC" V 6468 2177 50  0000 L CNN
F 2 "" H 6450 2050 50  0001 C CNN
F 3 "" H 6450 2050 50  0001 C CNN
	1    6450 2050
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR0103
U 1 1 6011B89A
P 5200 2050
F 0 "#PWR0103" H 5200 1900 50  0001 C CNN
F 1 "VCC" V 5218 2177 50  0000 L CNN
F 2 "" H 5200 2050 50  0001 C CNN
F 3 "" H 5200 2050 50  0001 C CNN
	1    5200 2050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6450 2050 6550 2050
Wire Wire Line
	5300 2050 5200 2050
$Comp
L wemos_mini:WeMos_mini WemosD1Mini1
U 1 1 6011D5CA
P 2400 1850
F 0 "WemosD1Mini1" H 2400 1000 60  0000 C CNN
F 1 "WeMos_D1_mini" H 2400 2593 60  0001 C CNN
F 2 "wemos_d1_mini:D1_mini_board" H 2400 2600 60  0001 C CNN
F 3 "https://www.wemos.cc/en/latest/d1/d1_mini.html" H 2400 1200 60  0000 C CNN
	1    2400 1850
	-1   0    0    1   
$EndComp
Wire Wire Line
	3150 2050 3000 2050
Wire Wire Line
	3000 2050 3000 2000
Wire Wire Line
	3000 2000 2900 2000
Wire Wire Line
	3150 1900 2900 1900
Wire Wire Line
	3100 2300 3100 2100
Wire Wire Line
	3100 2100 2900 2100
Wire Wire Line
	2900 2300 2900 2200
Wire Wire Line
	7450 2450 7450 2150
Wire Wire Line
	6150 2450 7450 2450
Wire Wire Line
	7450 2050 7900 2050
Wire Wire Line
	7900 2050 7900 1950
Wire Wire Line
	7700 1950 7450 1950
$Comp
L Connector:Conn_01x04_Male Servo1
U 1 1 60119A90
P 5500 2050
F 0 "Servo1" H 5650 1650 50  0000 C CNN
F 1 "Conn_01x04_Male" H 5606 2237 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5500 2050 50  0001 C CNN
F 3 "https://www.parallax.com/product/parallax-feedback-360-high-speed-servo/" H 5100 1500 50  0000 C CNN
	1    5500 2050
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Male Servo2
U 1 1 6011A082
P 6750 2050
F 0 "Servo2" H 6900 1650 50  0000 C CNN
F 1 "Conn_01x04_Male" H 6856 2237 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 6750 2050 50  0001 C CNN
F 3 "~" H 6750 2050 50  0001 C CNN
	1    6750 2050
	-1   0    0    1   
$EndComp
Wire Wire Line
	6350 1900 6400 1900
Wire Wire Line
	6400 1900 6400 1950
Wire Wire Line
	6400 1950 6550 1950
Wire Wire Line
	5300 1950 5100 1950
Wire Wire Line
	5100 1950 5100 1900
Wire Wire Line
	5100 1900 5050 1900
Text GLabel 5050 1750 0    50   Input ~ 0
Servo1_feedback
Text GLabel 6350 1750 0    50   Input ~ 0
Servo2_feedback
Wire Wire Line
	5300 1850 5150 1850
Wire Wire Line
	5150 1850 5150 1750
Wire Wire Line
	5150 1750 5050 1750
Wire Wire Line
	6350 1750 6450 1750
Wire Wire Line
	6450 1750 6450 1850
Wire Wire Line
	6450 1850 6550 1850
Text GLabel 1650 1900 0    50   Input ~ 0
Servo1_feedback
Text GLabel 1650 2050 0    50   Input ~ 0
Servo2_feedback
Wire Wire Line
	1900 2000 1800 2000
Wire Wire Line
	1800 2000 1800 2050
Wire Wire Line
	1800 2050 1650 2050
Wire Wire Line
	1650 1900 1900 1900
$Comp
L Connector:Conn_01x05_Female VL53L1X_1
U 1 1 6011D140
P 2250 3500
F 0 "VL53L1X_1" H 1800 3950 50  0000 L CNN
F 1 "Conn_01x05_Female" H 2277 3435 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 2250 3500 50  0001 C CNN
F 3 "https://www.pololu.com/product/3415" H 2350 2850 50  0000 C CNN
	1    2250 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 6011EC3D
P 1700 3800
F 0 "#PWR0104" H 1700 3550 50  0001 C CNN
F 1 "GND" H 1705 3627 50  0000 C CNN
F 2 "" H 1700 3800 50  0001 C CNN
F 3 "" H 1700 3800 50  0001 C CNN
	1    1700 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0105
U 1 1 6011ECAD
P 1950 3800
F 0 "#PWR0105" H 1950 3650 50  0001 C CNN
F 1 "+3.3V" H 1965 3973 50  0000 C CNN
F 2 "" H 1950 3800 50  0001 C CNN
F 3 "" H 1950 3800 50  0001 C CNN
	1    1950 3800
	-1   0    0    1   
$EndComp
Text GLabel 1750 3550 0    50   Input ~ 0
SDA
Text GLabel 1750 3400 0    50   Input ~ 0
SCL
Text GLabel 1850 3250 0    50   Input ~ 0
XSHUT_1
Text GLabel 1550 1600 0    50   Input ~ 0
XSHUT_1
Wire Wire Line
	1950 3800 1950 3700
Wire Wire Line
	1950 3700 2050 3700
Wire Wire Line
	1700 3800 1700 3700
Wire Wire Line
	1700 3700 1900 3700
Wire Wire Line
	1900 3700 1900 3600
Wire Wire Line
	1900 3600 2050 3600
Wire Wire Line
	1750 3550 1850 3550
Wire Wire Line
	1850 3550 1850 3500
Wire Wire Line
	1850 3500 2050 3500
Wire Wire Line
	1750 3400 2050 3400
Wire Wire Line
	1850 3250 2050 3250
Wire Wire Line
	2050 3250 2050 3300
$Comp
L Connector:Conn_01x05_Female VL53L1X_2
U 1 1 601243E2
P 3250 3500
F 0 "VL53L1X_2" H 2800 3950 50  0000 L CNN
F 1 "Conn_01x05_Female" H 3277 3435 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 3250 3500 50  0001 C CNN
F 3 "~" H 3250 3500 50  0001 C CNN
	1    3250 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 601243E9
P 2700 3800
F 0 "#PWR0106" H 2700 3550 50  0001 C CNN
F 1 "GND" H 2705 3627 50  0000 C CNN
F 2 "" H 2700 3800 50  0001 C CNN
F 3 "" H 2700 3800 50  0001 C CNN
	1    2700 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0107
U 1 1 601243EF
P 2950 3800
F 0 "#PWR0107" H 2950 3650 50  0001 C CNN
F 1 "+3.3V" H 2965 3973 50  0000 C CNN
F 2 "" H 2950 3800 50  0001 C CNN
F 3 "" H 2950 3800 50  0001 C CNN
	1    2950 3800
	-1   0    0    1   
$EndComp
Text GLabel 3150 1750 2    50   Input ~ 0
SDA
Text GLabel 3150 1600 2    50   Input ~ 0
SCL
Text GLabel 2850 3250 0    50   Input ~ 0
XSHUT_2
Wire Wire Line
	2950 3800 2950 3700
Wire Wire Line
	2950 3700 3050 3700
Wire Wire Line
	2700 3800 2700 3700
Wire Wire Line
	2700 3700 2900 3700
Wire Wire Line
	2900 3700 2900 3600
Wire Wire Line
	2900 3600 3050 3600
Wire Wire Line
	2750 3550 2850 3550
Wire Wire Line
	2850 3550 2850 3500
Wire Wire Line
	2850 3500 3050 3500
Wire Wire Line
	2750 3400 3050 3400
Wire Wire Line
	2850 3250 3050 3250
Wire Wire Line
	3050 3250 3050 3300
Text GLabel 1550 1750 0    50   Input ~ 0
XSHUT_2
Wire Wire Line
	1900 1800 1700 1800
Wire Wire Line
	1700 1800 1700 1750
Wire Wire Line
	1700 1750 1550 1750
Wire Wire Line
	1900 1700 1750 1700
Wire Wire Line
	1750 1700 1750 1600
Wire Wire Line
	1750 1600 1550 1600
Wire Wire Line
	3150 1750 3050 1750
Wire Wire Line
	3050 1750 3050 1800
Wire Wire Line
	3050 1800 2900 1800
Wire Wire Line
	2900 1700 3000 1700
Wire Wire Line
	3000 1700 3000 1600
Wire Wire Line
	3000 1600 3150 1600
Text GLabel 2750 3550 0    50   Input ~ 0
SDA
Text GLabel 2750 3400 0    50   Input ~ 0
SCL
Wire Wire Line
	5100 2450 6150 2450
$Comp
L power:+3.3V #PWR?
U 1 1 60134EA2
P 1750 2250
F 0 "#PWR?" H 1750 2100 50  0001 C CNN
F 1 "+3.3V" H 1765 2423 50  0000 C CNN
F 2 "" H 1750 2250 50  0001 C CNN
F 3 "" H 1750 2250 50  0001 C CNN
	1    1750 2250
	-1   0    0    1   
$EndComp
Wire Wire Line
	1900 2200 1750 2200
Wire Wire Line
	1750 2200 1750 2250
$EndSCHEMATC
