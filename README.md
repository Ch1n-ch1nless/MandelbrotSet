<h1 align="center"> Лабораторная работа </h1>
<h1 align="center"> "Исследование зависимости времени работы программы от инструкций SIMD" </h1>
<br>

<h3 align="center"> Александров Олег </h3>
<h3 align="center"> Б05-331 </h3>
<h3 align="center"> 28.03.2024 </h3>

<br>

<h1 align="center"> Цель работы </h1>
Выяснить насколько сильно ускоряют время работы программы инструкции SIMD и ключи оптимизации O3, Ofast и O0

<h1 align="center"> Оборудование </h1>
<b>Программа</b> : Рисование множества Мандельброта. <br>
<b>Процессор</b> : AMD Ryzen 7 5800H, поддерживающий инструкции: SMT, MMX(+), SSE(1, 2, 3, 3S, 4.1, 4.2, 4A), AVX(1, 2), FMA(3), AES, CLMUL, RdRand, SHA, AMD-V, x86-64. <br>
<b>ОС</b> : Linux Ubuntu 22.04.4 LTS (64-bit). Все тесты были проведены в режиме "Perfomance". <br>

<h1 align="center"> Немного о программе и как она работает </h1>

Для запуска программы необходимо ввести следующие команды:
```
    git clone git@github.com:Ch1n-ch1nless/MandelbrotSet.git 
```
<br>
<b>Функционал</b>
<br>
При нажатии левой стрелки   -- Картинка сдвигается влево. <br>
При нажатии правой стрелки  -- Картинка сдвигается вправо. <br>
При нажатии верхней стрелки -- Картинка сдвигается вверх. <br>
При нажатии нижней стрелки  -- Картинка сдвигается вниз. <br>
При нажатии Escape          -- Закрывается окно. <br>
При нажатии + на NumPad     -- Картинка увеличивается. <br>
При нажатии - на NumPad     -- Картинка уменьшается. <br>

<h1 align="center"> Ход работы </h1>

Было решено взять алгоритм расчета множества Мандельброта, который будет нагружать процессор. <br>
Так же была подключена библиотека "sfml" для рисования множества. <br>
Время решено было замерять через функцию '__rdtsc()', возвращающую количество тактов c момента последнего сброса процессора. И встроенную в библиотеку "sfml" функцию clock(). <br>
Всего было реализовано 3 версии программы:

### 1 версия
Файл: "simple_realization.cpp". <br>
В данном файле функция расчета множества Mандельброта построена следующим образом: выбираем точку (x0, y0), где x0 = Re(z0) и y0 = Im(z0). и вычисляется все по формуле:
``` LaTeX
    x_{n+1} = x_{n}^2 - y_{n}^2 + x0;
    y_{n+1} = 2 * x_{n} * y_{n} + y0;
```

И так алгоритм вычисляет следующие значения z_{n}, пока |z_{n}| < max_raduis или n <= 255. <br>

| Флаг компиляции |  FPS |    Тики    |
|:---------------:|:----:|:----------:|
|       -O0       | 3.24 | 1180109382 |
|       -O3       | 9.86 |  366472583 |
|:---------------:|:----:|:----------:|

### 2 версия 
Файл: 8_point_realization.cpp. <br>
Алгоритм так же вычисляет точки множества Мандельброта по формуле: 
``` LateX
    z_{n+1} = z_{n}^2 + C_0
```
Однако, теперь алгоритм считает не 1 точку, а массив из 8 точек.
Результаты работы:

| Флаг компиляции |  FPS  |    Тики    |
|:---------------:|:-----:|:----------:|
|       -O0       |  1.27 | 2634282477 |
|       -O3       | 16.86 |  195826479 |
|:---------------:|:-----:|:----------:|

Можно заметить, что при O0 алгоритм версии 2 работает плохо. Это происходит из-за того, что компилятор не видит возможности замены команды на SIMD. В то время. как с флагом -O3, она обгоняет её! Так как компилятор видит возможность оптимизации кода и частичной замены на SIMD.

### 3 версия
Файл: simd_realization.cpp <br>


| Версия |         Имя файла       | Флаги компиляции |   FPS   |     Тики    | Прирост производительности |
|:------:|:-----------------------:|:----------------:|:--------|:-----------:|:--------------------------:|
|   1.0  | simple_realization.cpp  |      -O0         |   3.24  |  1180109382 |                            |
|   1.0  | simple_realization.cpp  |      -O3         |   9.86  |   366472583 |                            |
|   2.0  | 8_point_realization.cpp |      -O0         |   1.27  |  2634282477 |                            |
|   2.0  | 8_point_realization.cpp |      -O3         |  16.09  |   195826479 |                            |
|   3.0  |   simd_realization.cpp  |      -O0         |   8.78  |   377003918 |                            |
|   3.0  |   simd_realization.cpp  |      -O3         |  55.23  |    55183899 |                            |
|:------:|:-----------------------:|:----------------:|:-------:|:-----------:|:--------------------------:|


