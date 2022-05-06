<?php
	//Klasa databaseSelector wyposażona w funkcję select pobiera z bazy danych wartości pomiarów i umieszcza je na stronie
	class databaseSelector
	{
		public function select()
		{
			$mysqli = new mysqli("localhost","fromBase","Arduino4","weather_station");

			if ($mysqli -> connect_errno) {
				echo "Failed to connect to MySQL: " . $mysqli -> connect_error;
				exit();
			}

			$sql="SELECT * FROM register";
			if ($result = $mysqli -> query($sql)) {
				$num_rows = $result -> num_rows;
				while($row = $result -> fetch_assoc()){
				echo "<div class='row'><div>".$row['id']."</div><div>".$row['temp']."</div><div>".$row['hum']."</div><div>".$row['fall']."</div><div>".$row['quality']."</div><div>".$row['light']."</div><div>".$row['vvDb']."</div><div>".$row['date']."</div></div>";
				}
				$result -> close();
			}
			$mysqli -> close();
		}
	}
?>
