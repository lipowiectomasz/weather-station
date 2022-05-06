window.onload=function(){
	jgetText();													//Wywolanie funkcji jgetText pobierającej zawartość pliku toWeb.txt, 
																//w której zawarte są aktualne wskaźniki ze stacji pogodowej.
}
function jgetText(){
	jQuery.get("toWeb.txt").done(res => {						//Pobranie zawartości pliku toWeb.txt.
		
	let index = 0;
	let space = [];
	for(let i=0; i<res.length; i++){							//Wykrywanie znaków białych oddzielających wartości.
		if(res[i].match(/\s/g)=="\n"){
			space[index]=i;
			index++;
		}
	}
	let temp1 = res.substring(0, space[0]);						//Przypisanie wartości do zmiennych na podstawie
	let hum1= res.substring(space[0]+1, space[1]);				//położenia wykrytych znaków białych. (Wszystkie wartości oddzielone sa spacja).
	let fall1 = res.substring(space[1]+1, space[2]);
	let quality1 = res.substring(space[2]+1, space[3]);
	let light1 = res.substring(space[3]+1, space[4]);
	let vvDb1 = res.substring(space[4]+1, space[5]);

	document.getElementById("temp").innerHTML = temp1;			//Umieszczenie wartości w dokumencie HTML.
	document.getElementById("hum").innerHTML = hum1;
	document.getElementById("fall").innerHTML = fall1;
	document.getElementById("quality").innerHTML = quality1;
	document.getElementById("light").innerHTML = light1;
	document.getElementById("vvDb").innerHTML = vvDb1;
	});
	setTimeout(jgetText,15000);									//Częstotliwość wykonywania funkcji.
}