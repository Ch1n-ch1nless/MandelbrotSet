file_name = "time_file.txt"
with open("time_file.txt") as file:
    numbers = [float(line.strip()) for line in file]
    average = sum(numbers) / len(numbers)
    print(f"Среднее арифметическое чисел в файле {file_name}: {average}")