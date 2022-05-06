window.onload=function(){
	czas();
	
	//Pobranie wartości daty
	let data = new Date();	
	let dzien = document.getElementById("date");

	//Wstawienie aktualnej daty na strone
	let miesiac = data.getMonth()+1;
	if	(miesiac < 10)	miesiac="0"+miesiac;
		dzien.innerHTML=(data.getDate()+"/"+miesiac+"/"+data.getFullYear());
}
function czas(){
	//Pobranie wartości daty
	let data=new Date();
	
	//Wstawienie aktualnej godziny na strone
	let zegar = document.getElementById("clock");
	zegar.innerHTML=(data.getHours()+":"+data.getMinutes()+":"+data.getSeconds());
	
	setTimeout(czas,1000);
}
