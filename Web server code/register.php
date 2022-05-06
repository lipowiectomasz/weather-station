<!DOCTYPE=html>
<html lang="pl">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
	<meta desctiption="Full information about your weather every day."/>
	<meta name="keywords" content="weather, temperature, pressure, downfall"/>
	
	<title>WeatherStationRegister</title>
	<link rel="stylesheet" href="reg_style.css" type="text/css" />
	<link rel="icon" type="image/png" href="images/icon.png"/>
	
	<link href="https://fonts.googleapis.com/css?family=Source+Sans+Pro&display=swap" rel="stylesheet"/>
	
</head>
<body>
	<h1> Weather Register </h1>
	<nav> 
		<div>ID</div>
		<div>Temp</div>
		<div>Hum</div>
		<div>Fall</div>
		<div>Quality</div>
		<div>Light</div>
		<div>vvDb</div>
		<div>Date</div>
	</nav>
	<div id="register">
		<?php
			include 'fromBase.php';
			
			$base = new databaseSelector();
			$base->select();
		?>
	</div>
</body>

</html>
