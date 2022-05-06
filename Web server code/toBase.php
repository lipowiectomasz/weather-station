<?php

//Skrypt przyjmujący poprzez metode GET parametry zmierzone przez stację, wartości następnie przekazywane są do bazy danych.

$temp = ($_GET['temp']);
$hum = ($_GET['hum']);
$fall = ($_GET['fall']);
$quality = ($_GET['quality']);
$light = ($_GET['light']);
$vvDb = ($_GET['vvDb']);
$inputDate = date("Y-m-d h:i:s");

$mysqli = new mysqli("localhost","toBase","Arduino8","weather_station");

	if ($mysqli -> connect_errno) {
	  echo "Failed to connect to MySQL: " . $mysqli -> connect_error;
	  exit();
	}

	$sql="INSERT INTO register (temp, hum, fall, quality, light, vvDb, date) VALUES (".$temp.", ".$hum.", ".$fall.", ".$quality.", ".$light.", ".$vvDb.", '".$inputDate."');";
	
	//Return name of current default database
	if ($mysqli -> query($sql) === TRUE){
		echo "Data sent.";
	} else {
		echo "Something goes wrong.";
	}
	
$mysqli -> close();

?>
