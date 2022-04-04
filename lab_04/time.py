from cProfile import label
import time
import matplotlib.pyplot as plt

times = []

with open("../lab_04/time_res.txt", "r") as f:
    params = f.readline().split()
    params = [int(x) for x in params]
    for line in f:
        times.append(float(line.replace(",", ".")) * 1000000)

n = params[3]
r = params[1]
step = params[2]

y1 = times[:n]
y2 = times[n:2*n]
y3 = times[2*n:3*n]
y4 = times[3*n:4*n]
x = [y for y in range(r, r + n * step, step)]

plt.plot(x, y1, label="Каноническое уравнение")
plt.plot(x, y2, label="Параметрическое уравнение")
plt.plot(x, y3, label="Алгоритм Брезенхема")
plt.plot(x, y4, label="Алгоритм средней точки")
plt.legend(loc='upper left') 
if params[0] == 0:
    plt.title("Замеры времени для окружности")
    plt.xlabel('Радиус окружности\n Начальный радиус - {}, шаг - {}, количество - {}'.format(r, step, n))
else:
    plt.title("Замеры времени для эллипса\n Начальный радиус - {}, шаг - {}, количество - {}".format(r, step, n))
    plt.xlabel('Радиус полуоси a')
plt.ylabel('Время в мкс.') 
plt.show()