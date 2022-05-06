Projekt stacji pogodowej stworzony na potrzeby przedmiotu o nazwie Mikrokomputery.

Za działanie stacji odpowiada płytka Arduino Uno R3, która przyjmuje dane z 5 podłączonych do niej czujników, które następnie przy pomocy modułu ESP-01 przesyła do lokalnego serwera webowego.

Wartości mierzone przy pomocy czujników:
- natężenie światła otoczenia,
- czystość powietrza,
- obecność opadów,
- natężenie dźwięku,
- temperatura i wilgotność.

Odnotowane wartości wyświetlane są na wyświetlaczu LCD 2'' umieszczonym w dwunastościennej obudowie wraz z pozostałymi komponentami oraz źródłem zasilania.

![Zasób 1](https://user-images.githubusercontent.com/105055717/167175601-c84a0cd4-256d-497b-8353-6d0d9ac97967.png)

Moduł ESP przesyła te same dane do serwera, który zapisuje je w bazie oraz wyświetla na lokalnej stronie. 
Proces ten opisuje rysunek:

![Diagram](https://user-images.githubusercontent.com/105055717/167177576-be5b002c-02ae-4e7d-b7fe-af5df91f298c.png)

Strona internetowa udostępnia dwie podstrony podglądu:

![site1s](https://user-images.githubusercontent.com/105055717/167178622-103552f1-0584-4208-a455-1565ec3a0b8a.png)

![site2s](https://user-images.githubusercontent.com/105055717/167178635-98ebacbd-6fde-435f-a821-7af8416b5038.png)
