# Space Sodomy Online
[Трейлер](#trailer)

[Как запустить клиент](#client)

[Как смотреть реплеи](#replay)

[Как поднять сервер](#server)

<b> <a id="trailer" />Трейлер: </b>

[![SPACE SODOMY](https://img.youtube.com/vi/WemOQuTlSWo/0.jpg)](https://www.youtube.com/watch?v=WemOQuTlSWo)
## <a id="client" />Как запустить клиент
Скачайте клиент игры по [ссылке](https://github.com/StarikTenger/SpaceSodomyOnline/raw/master/Client/ready_to_run.zip)
 
Перед запуском надо открыть файл **config.conf** <br> В нём указаны: 
1. Адрес 
2. Порт
3. Id
4. Имя
5. 2 номера установленных модулей

Адрес, порт и Id узнавайте у хозяина сервера <br>
Имя может содержать латинские символы и цифры, в нём не должно быть пробелов <br>

### Модули выбираются в соответствии с таблицей:

|  ID       |    | Модуль      |Описание              |
|---------- | - |:-------------:|:------               |
|    0 | ![](https://raw.githubusercontent.com/StarikTenger/SpaceSodomyOnline/master/Client/Client/textures/modules/hpUp.png)    | HP up         |+1 к здоровью         |
|    1  | ![](https://raw.githubusercontent.com/StarikTenger/SpaceSodomyOnline/master/Client/Client/textures/modules/energyUp.png)   | Energy Up     |+5 к энергии          |
|    2  | ![](https://raw.githubusercontent.com/StarikTenger/SpaceSodomyOnline/master/Client/Client/textures/modules/cascade.png)   | Grapeshot       | Выпускает веер из 5 медленных пуль  |
|    3   | ![](https://raw.githubusercontent.com/StarikTenger/SpaceSodomyOnline/master/Client/Client/textures/modules/impulse.png)  | Impulse       | Мгновенно придаёт большую скорость, направленную вперёд       |
|    4  |  ![](https://raw.githubusercontent.com/StarikTenger/SpaceSodomyOnline/master/Client/Client/textures/modules/rocket.png)  | Rocket        | Ракета, летящая с ускорением. Взрывается, нанося урон по области |
|    5  |  ![](https://raw.githubusercontent.com/StarikTenger/SpaceSodomyOnline/master/Client/Client/textures/modules/splash.png)  | Force        | Отталкивает все объекты вокруг игрока. Игрок отлетает в противоположном направлении|
|    6  |  ![](https://raw.githubusercontent.com/StarikTenger/SpaceSodomyOnline/master/Client/Client/textures/modules/immortality.png)  | Immortality   | Бессмертие на короткое время    |
|    7   | ![](https://raw.githubusercontent.com/StarikTenger/SpaceSodomyOnline/master/Client/Client/textures/modules/blink.png)  | Blink         |Телепортация вперёд                     |
|    8  |  ![](https://raw.githubusercontent.com/StarikTenger/SpaceSodomyOnline/master/Client/Client/textures/modules/invisibility.png)  | Invisibility   |Невидимость    |
|    9   | ![](https://raw.githubusercontent.com/StarikTenger/SpaceSodomyOnline/master/Client/Client/textures/modules/mass.png)  | Spitball         |Массивный снаряд, обладающий отдачей и приклеивающийся к противнику   |

### Управление
- **Left**, **Right**, **Up**, **Down** — линейные двигатели /влево/вправо/вперёд/назад
- **W**, **S** — линейные двигатели вперёд/назад
- **A**, **D** — поворотные двигатели
- **E**, **Q** — приближение/отдаление
- **Space** — стрелять
- **Shift** — стабилизация вращения
- **F** — активировать предмет в инвентаре
- **1**, **2** — модули
- Линейными двигателями можно управлять **мышкой** 

Поменять управление можно в файле **keys.conf** <br>
Напротив каждой команды указана соответсвующая ей клавиша <br>

##### Допустимые клавиши: <br>
*RIGHT, 
LEFT, 
DOWN, 
UP, <br>
SPACE, <br>
SHIFT, <br>
ESCAPE, <br>
ALT, <br>
Q, W, E, R, T, Y, U, I, O, P, A, S, D, F, G, H, J, K, L, Z, X, C, V, B, N, M,
 <br>
 NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0*

## <a id="replay" />Как смотреть реплеи
Для просмотра реплеев необходим клиент игры

1. Переименуйте файл реплея в **replay.rep** и положите его в папку клиента
2. В **config.conf** вместо имени напишите *REPLAY*
3. Запустите **Client.exe**

#### Управление
* Камера управляется **мышкой**
* Стрелками **вправо-влево** регулируется скорость
* Стрелками **вверх-вниз** меняется объект фокуса
* Пауза на **пробел**
* **S** - включить/выключить автоматический режим движения камеры
* **H** - скрыть/показать интерфейс

## <a id="server" />Как поднять сервер

Скачайте сервер по [ссылке](https://github.com/StarikTenger/SpaceSodomyOnline/raw/master/Server/Server_rtr.zip)

Настройте параметры, открыв **confing.conf**

Запустите **Server.exe**









