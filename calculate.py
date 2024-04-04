import numpy as np

# Чтение чисел из файла
with open("numbers.txt", "r") as file:
    numbers = [float(line.strip()) for line in file]

# Вычисление среднего значения
mean = np.mean(numbers)

# Вычисление дисперсии
variance = np.var(numbers)

# Вывод результатов
print(f"Среднее значение: {mean}")
print(f"Дисперсия: {variance}")
print(f"=======")
