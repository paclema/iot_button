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
L power:GND #PWR?
U 1 1 607608E6
P 3200 2850
F 0 "#PWR?" H 3200 2600 50  0001 C CNN
F 1 "GND" H 3205 2677 50  0000 C CNN
F 2 "" H 3200 2850 50  0001 C CNN
F 3 "" H 3200 2850 50  0001 C CNN
	1    3200 2850
	1    0    0    -1  
$EndComp
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
P 3450 1450
F 0 "R1" H 3520 1496 50  0000 L CNN
F 1 "10k" H 3520 1405 50  0000 L CNN
F 2 "" V 3380 1450 50  0001 C CNN
F 3 "~" H 3450 1450 50  0001 C CNN
	1    3450 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 1250 3450 1250
Wire Wire Line
	3450 1250 3450 1300
Wire Wire Line
	3450 1600 3450 1950
Wire Wire Line
	3450 1950 3100 1950
Wire Wire Line
	2950 1750 3100 1750
Wire Wire Line
	3100 1750 3100 1950
Connection ~ 3100 1950
Wire Wire Line
	3100 1950 2950 1950
$Comp
L Connector:XLR3_Switched J?
U 2 1 6081D2E4
P 3200 2400
F 0 "J?" V 3246 2171 50  0000 R CNN
F 1 "XLR3_Switched" V 3155 2171 50  0000 R CNN
F 2 "" H 3200 2500 50  0001 C CNN
F 3 " ~" H 3200 2500 50  0001 C CNN
	2    3200 2400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3100 2100 3100 1950
Wire Wire Line
	3200 2850 3200 2700
$EndSCHEMATC
