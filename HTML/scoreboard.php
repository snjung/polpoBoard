<?php
	//
	// 01.05.2017
	// This file will send data to the Arduino via serial USB
	// It uses the php_serial class
	//	
	
	file_put_contents('save.txt', $_GET['daten']); // Save the scores! They will be automatically loaded when the scoreboard is turned on. Make sure save.txt is writable by the web server user!
	
	include "php_serial.class.php";

	//The webserver calls this php-script and sends the current standings on the scoreboard via "Get" as a string (TeamA[2].Counter[4].TeamB[2].Quarter[1]

	//First we separate the input string:

	$teamAscore=substr($_GET['daten'], 0, 2); //Get first 2 digits
	$minutes=substr($_GET['daten'], 2, 2); //Get next 2 digits
	$seconds=substr($_GET['daten'], 4, 2); //Get next 2 digits
	$teamBscore=substr($_GET['daten'], 6, 2); //Get next 2 digits
	$qtr=substr($_GET['daten'], 8, 1); //Get next 1 digits
	
	// For numbers <10 the two-digit-displays should only display the 2nd digit (-0 instead of 00)
	// A 0 should be displayed as -0
	//
	// First strip the leading zeros, then "pad" the number to the required length by adding dashes to turn off
	// LEDs that would be showing 0 before the actual score
	
	if($teamAscore==0) { 
		$teamAscore="-0"; 
		} 
	else { 
		$teamAscore = ltrim($teamAscore, '0'); //Removes all zeros at the start of the string
		$teamAscore = str_pad($teamAscore, 2, '-', STR_PAD_LEFT); //Adds '-' characters to the start of the string and brings it to length 2 
		}
	if($minutes==0) { 
		$minutes="-0"; 
		} 
	else { 
		$timer = ltrim($timer, '0'); 
		$timer = str_pad($timer, 2, '-', STR_PAD_LEFT); 
		}
	if($teamBscore==0) { 
		$teamBscore="-0"; 
		} 
	else { 
		$teamBscore = ltrim($teamBscore, '0'); 
		$teamBscore = str_pad($teamBscore, 2, '-', STR_PAD_LEFT); 
		}
	
	// Now we can transmit the data to the Arduino
	$serial = new phpSerial;
	$serial->deviceSet("/dev/ttyACM0");
	//$serial->deviceSet("/dev/ttyAMA0");
	$serial->confBaudRate(57600);
	$serial->confParity("none");
	$serial->confCharacterLength(8);
	$serial->confStopBits(1);
	
	//uncomment the following line is you want the second line of the scoreboard to be over, wickets target
	//$tempString="4,".$batsmana.",".$total.",".$batsmanb.",".$overs.",".$wicketsOnes.",".$target."#";
	//$tempString="4,".$batsmana.",".$total.",".$batsmanb.",".$wicketsOnes.",".$overs.",".$target."#";
	$tempString="4,".$teamAscore.",".$minutes.",".$seconds.",".$teamBscore."#";
	$serial->deviceOpen();
	$serial->sendMessage($tempString);
	$serial->deviceClose();
	
	echo "Team A: ".$teamAscore."Team B: ".$teamBscore."Timer: ".$timer."Quarter: ".$qtr;

?>
