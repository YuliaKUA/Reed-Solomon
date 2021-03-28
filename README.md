# Reed-Solomon

Передо мной стояла задача изучить Коды Рида-Соломона и реализовать декодер, на основе алгоритма Берлекемпа-Месси. 
Для этого я начала с изучения теории вопроса и реализации кодировщика.

[Отчет к моей работе можно посмотреть тут](https://github.com/YuliaKUA/Reed-Solomon/blob/main/ReedSolomon.pdf)

#### Табличные коды -> Линейно-блочные коды -> Циклические коды -> Коды Рида-Соломона

В кодах Рида-Соломона сообщение представляется в виде набора символов некоторого алфавита. Собственно говоря, в качестве алфавита используется поле Галуа. 
То есть, если мы хотим закодировать сообщение, представленное двоичным кодом, то мы разбиваем его (в моем случае, используем поле Галуа из 256 элементов) на группы по 8 битов и дальше работаем с каждой группой как с числом из этого поля Галуа. 

### Общий план:

В целом, решение поставленной задачи можно разбить на 3 основных пункта:
1. Нахождение поля Галуа и определение математических операций (я выбрала полином 285, GF(2^8))
2. Кодирование сообщения
3. Декодирование сообщения

### Поле Галуа ([файл help_gf.h](https://github.com/YuliaKUA/Reed-Solomon/blob/main/ReedSolomon/help_gf.h))
Конечные поля Галуа существуют не при любом числе элементов, а только в случае, если число элементов является простым числом р или степенью q=p^m простого числа. Также на них должно выполняться ряд математических правил и условий.

Поэтому для построения такого поля нам необходим особенный полином. Делается это не просто так, ведь если мы будем вычислять поле используя многочлен из диапазона 0..255 (и др. числа) то при (xor 256) у нас обязательно появятся дубликаты.
Поэтому, прежде, чем кодировать и декодировать сообщения, используя поле Галуа, нам необходимо найти неприводимый многочлен, чтобы в нашем исходном поле у нас было 0,1,2,..., p-1 различных уникальных значений.
Функция find_prime_polys(). Мы генерируем все возможные простые многочлены (т.е. все целые числа от 256..512), строим поле Галуа и проверяем, чтобы не было дубликатов.

#### Математические операции определены в [файле gf.h](https://github.com/YuliaKUA/Reed-Solomon/blob/main/ReedSolomon/gf.h)

### Кодирование Рида-Соломона([файл rs_encoding.h](https://github.com/YuliaKUA/Reed-Solomon/blob/main/ReedSolomon/rs_encoding.h))
Если коротко, то смысл кодирования заключается в том, чтобы к исходной информации добавить избыточную, по которой с помощью определенных математических правил мы могли бы восстановить исходное сообщение.

Существует две разновидности кодирования: систематический и несистематический код. 
Систематический код строится следующим образом: Сначала полином сдвигается на K коэффициентов влево               p’(x) = p(x) * x^((N-k)) 
а потом вычисляется его остаток от деления на порождающий полином и прибавляется к p’(x):                         C(x) = p’(x) + p’(x) mod g(x) 
Другими словами, сообщение «сдвигается» на N-K символов - так, что его полином имеет такие коэффициенты: mk, .. m2, m1, m0, 0,0,0,0,0,0 (m0.. mk - символы сообщения) 

Закодированное сообщение C(x) обладает очень важным свойством: оно без остатка делится на порождающий многочлен g(x)! (это не так очевидно, как в несистематическом)

Обобщая, можно сказать, что наш генераторный полином – словарь кодирования, а полиномиальное деление = оператор преобразования нашего сообщения в код Рида Соломона.
Программно это выглядит очень просто: генерируем полином, вызываем метод деления (определенный для полиномов, алгоритм синтетического деления) и возвращаем исходное сообщение + остаток. (мне показалось интересным реализовать метод синтетического деления, к тому же оно должно работать быстрее, в теории).

### Декодирование Рида-Соломона([файл rs_decoding.h](https://github.com/YuliaKUA/Reed-Solomon/blob/main/ReedSolomon/rs_decoding.h))
Самая интересная и объёмная часть – это декодирование.

#### Коротко шаги декодирования (функция rs_decode_msg()):
1. Вычислить полином синдрома Si = С(ai+1) (функция rs_calc_syndromes()).
2. Если все элементы нулевые – значит исходное сообщение не искажено. Вывести его. 
3. Иначе вычислить L(x) (при условии что мы не знаем в каком месте находятся ошибки), используя алгоритм Берлекемпа-Месси (функция rs_find_error_locator()).
4. Определить локаторы ошибок (функция rs_find_errors())
5. Декодировать сообщение (функция rs_correct_errata())

    5.1 Найти корни L(x) – они будут обратны к локаторам ошибок.
    
    5.2 Находим L2(x), для ошибок, чье местоположение мы знаем. (функция rs_find_errarta_locator()).
    
    5.3 Находим W(x) для L2(x) (функция rs_find_error_evaluator()).
    
    5.4 Используем алгоритм Форни для вычисления значения ошибки. Для этого находим формальную производную многочлена и значения ошибок еi по формулам.   
    
    5.5 Сформировать многочлен ошибок E(X) на основе локаторов и значений ошибок. Деление W на производную локатора ошибок дает нам величину ошибки (т. е. значение для 	восстановления) i-го символа.
    
    5.6 Скорректировать C(x) = C(x) + E(x).

6. Вычислить полином синдрома Si = С(ai+1) (функция rs_calc_syndromes()).
7. Проверить, чтобы все значения равнялись 0.
  
### Алгоритм Берлекемпа-Месси
Алгоритм Берлекемпа-Месси является наиболее эффективным для решения множества линейных уравнений и нахождения L(x). Я поняла основную идею следующим образом – алгоритм итеративно вычисляет полином локатора ошибок. Для этого он вычисляет дельта-расхождение, по которому мы можем определить, нужно ли нам обновлять локатор ошибок или нет. 

### Пример работы программы
N-ричный код с кодовым расстоянием D
| № | Код | Результат |
| :---: | :---: | :---: |
| 1 | (11, 12) | ![alt text](https://github.com/YuliaKUA/Reed-Solomon/blob/main/data/Image2.jpg)  |
| 2 | (9, 31)  | ![alt text](https://github.com/YuliaKUA/Reed-Solomon/blob/main/data/Image1.jpg)  |
| 3 | (20, 31) | ![alt text](https://github.com/YuliaKUA/Reed-Solomon/blob/main/data/Image3.jpg)  |
