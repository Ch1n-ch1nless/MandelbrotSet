<h1 align="center"> Лабораторная работа </h1>
<h1 align="center"> "Исследование зависимости времени работы программы от использования флагов компиляции, машинно зависимых оптимизаций и векторизации вычислений" </h1>
<br>

<h3 align="center"> Александров Олег </h3>
<h3 align="center"> Б05-331 </h3>
<h3 align="center"> 28.03.2024 </h3>

<br>

<h1 align="center"> Цель работы </h1>
Выяснить насколько сильно уменьшается время работы программы от использования флагов компиляции, машинно зависимых оптимизаций и векторизации вычислений.
<h1 align="center"> Оборудование </h1>
<b>Процессор</b> : AMD Ryzen 7 5800H, поддерживающий инструкции: AVX(1, 2)<br>
<b>ОС</b> : Linux Ubuntu 22.04.4 LTS (64-bit). Все тесты были проведены в режиме "Perfomance". <br>

<h1 align="center"> Ход работы </h1>

## Выбор программы

Я решил написать программу, которая рисует [множество Мандельброта](https://ru.wikipedia.org/wiki/%D0%9C%D0%BD%D0%BE%D0%B6%D0%B5%D1%81%D1%82%D0%B2%D0%BE_%D0%9C%D0%B0%D0%BD%D0%B4%D0%B5%D0%BB%D1%8C%D0%B1%D1%80%D0%BE%D1%82%D0%B0). Данная программа подходит по критериям, так как она может быть векторизована и можно внедрить машинно зависимые оптимизации.

## Зависимости

Для графики была использована библиотека: [SFML](https://www.sfml-dev.org/) <br>
Для машинно зависимых инструкций была использована библиотека: immintrin.h <br>
Другие стандартные библиотеки <br>

## Измерение времени 
Всего было реализовано 3 версии программы. Время я измерял время через функцию [__rdtsc()](https://github.com/MicrosoftDocs/cpp-docs/blob/main/docs/intrinsics/rdtsc.md), которая возвращает количество тактов с последнего сброса процессора(CPU reset). Рассчитывается разность и суммируется, после вычисляется среднее арифметическое<br>
Так же измерение времени было произведено с [clock](https://www.sfml-dev.org/documentation/2.6.1/classsf_1_1Clock.php). <br>
Всего было произведено 5 запусков каждой версии и в таблицах находится их среднее арифметическое.

### 1 версия
Файл: "simple_realization.cpp". <br>
В данном файле функция расчета множества Mандельброта построена следующим образом: выбираем точку ($x_0$, $y_0$), где $x_0$ = $Re(z_0)$ и $y_0 = Im(z_0)$. и вычисляется все по формуле:
    $$x_{n+1} = x_{n}^2 - y_{n}^2 + x_0$$
    $$y_{n+1} = 2 * x_{n} * y_{n} + y_0$$

И так алгоритм вычисляет следующие значения $z_{n}$, пока $|z_{n}| < max radius$ и $n <= 255$. <br>

| Флаг компиляции |  FPS |   Такты    | 
|:---------------:|:----:|:----------:|
|       -O0       | 3.24 | 1180109382 |
|       -O3       | 9.86 |  366472583 |

### 2 версия 
Файл: "vector_realization.cpp". <br>
Алгоритм так же вычисляет точки множества Мандельброта по формуле: 
    $$z_{n+1} = z_{n}^2 + C_0$$
Однако, теперь алгоритм считает не 1 точку, а массив из 8 точек.
Результаты работы:

| Флаг компиляции |  FPS  |   Такты    |
|:---------------:|:-----:|:----------:|
|       -O0       |  1.27 | 2634282477 |
|       -O3       | 16.86 |  195826479 |

Можно заметить, что при O0 алгоритм версии 2 работает плохо. Это происходит из-за того, что компилятор не видит возможности замены команды на SIMD. В то время. как с флагом -O3, она обгоняет её! Так как компилятор видит возможность оптимизации кода и частичной замены на SIMD.

### 3 версия
Файл: "intrinsic_realization.cpp". <br>
В данной реализации для вычисления точек в множестве Мандельброта использовались SIMD-инструкции. Так как мой процессор поддерживает AVX2, то я использовал инструкции:
```
_mm256_add_ps();
_mm256_sub_ps();
_mm256_mul_ps();
_mm256_cmp_ps();
 и т.д. ...
```

В таблице ниже видно, что благодаря SIMD-инструкциям программа стала работать гораздо быстрее:

| Флаг компиляции |  FPS  |   Такты   |
|:---------------:|:-----:|:---------:|
|       -O0       |  8.78 | 377003918 |
|       -O3       | 55.23 |  55183899 |

## Финальная табличка

| Версия |         Имя файла         | Флаги компиляции |   FPS   |    Такты    | Прирост производительности(FPS) | Прирост производительности(Такты) |
|:------:|:-------------------------:|:----------------:|:--------|:-----------:|:-------------------------------:|:---------------------------------:|
|   1.0  | simple_realization.cpp    |      -O0         |   3.24  |  1180109382 |        1,00                     | 1,00                              |
|   1.0  | simple_realization.cpp    |      -O3         |   9.86  |   366472583 |        3,04                     | 3,22                              |
|   2.0  | vector_realization.cpp    |      -O0         |   1.27  |  2634282477 |        0,38                     | 0,45                              |
|   2.0  | vector_realization.cpp    |      -O3         |  16.09  |   195826479 |        4,97                     | 6,03                              |
|   3.0  | intrinsic_realization.cpp |      -O0         |   8.78  |   377003918 |        2,71                     | 3,13                              |
|   3.0  | intrinsic_realization.cpp |      -O3         |  55.23  |    55183899 |       17,05                     | 21,39                             |

<h1 align="center"> Вывод </h1>
Используя данные из финальной таблички, можно сделать выводы, что 
при компилировании с флагом -O3 время работы программы уменьшается в ~3 раза. <br>
При внедрении "векторизации" с флагом -O3 время работы программы уменьшается в ~6 раз. <br>
При использовании intrinsic функций с флагом O3 время работы уменьшается в ~21 раз <br>


