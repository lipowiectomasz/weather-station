<?php

//Skrypt przyjmujący poprzez metodę GET parametry pogodowe, które następnie
//zapisuje w pliku toWeb.txt, z którego wartości pobierane są na serwis webowy.

$temp = ($_GET['temp']);
$hum = ($_GET['hum']);
$fall = ($_GET['fall']);
$quality = ($_GET['quality']);
$light = ($_GET['light']);
$vvDb = ($_GET['vvDb']);

$file = fopen("toWeb.txt","w") or die ("Cannot open!");
$txt = $temp."\n".$hum."\n".$fall."\n".$quality."\n".$light."\n".$vvDb."\n";
fwrite($file, $txt);
fclose($file);

?>
