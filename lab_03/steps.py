import matplotlib
import matplotlib.pyplot as plt
from itertools import count, takewhile
def frange(start, stop, step):
    return takewhile(lambda x: x< stop, count(start, step))

plt.figure(1)

r = 0
angle_step = 0
    
with open("../lab_03/steps_res.txt", "r") as f:
    r = float(f.readline().replace(",", "."))
    angle_step = float(f.readline().replace(",", "."))
    max_angle = float(f.readline().replace(",", "."))
    steps = []
    for line in f:
        steps.append(float(line.replace(",", ".")))
       
angles = list(frange(0, max_angle, angle_step))

i = 1
plt.figure(i)
plt.title("Анализ ступенчатости (ЦДА)")
plt.xlabel("Угол наклона прямой")
plt.ylabel(f"Число ступеней\nДлина отрезка - {r}")
plt.grid(True)
plt.plot(angles, steps[:len(angles)])
i += 1

plt.figure(i)
plt.title("Анализ ступенчатости (Брезенхем целочисленный)")
plt.xlabel("Угол наклона прямой")
plt.ylabel(f"Число ступеней\nДлина отрезка - {r}")
plt.grid(True)
plt.plot(angles, steps[len(angles):2*len(angles)])
i += 1

plt.figure(i)
plt.title("Анализ ступенчатости (Брезенхем действительный)")
plt.xlabel("Угол наклона прямой")
plt.ylabel(f"Число ступеней\nДлина отрезка - {r}")
plt.grid(True)
plt.plot(angles, steps[2*len(angles):3*len(angles)])
i += 1

plt.figure(i)
plt.title("Анализ ступенчатости (Брезенхем сглаживающий)")
plt.xlabel("Угол наклона прямой")
plt.ylabel(f"Число ступеней\nДлина отрезка - {r}")
plt.grid(True)
plt.plot(angles, steps[3*len(angles):4*len(angles)])
i += 1

plt.figure(i)
plt.title("Анализ ступенчатости (Ву)")
plt.xlabel("Угол наклона прямой")
plt.ylabel(f"Число ступеней\nДлина отрезка - {r}")
plt.grid(True)
plt.plot(angles, steps[4*len(angles):5*len(angles)])
i += 1

plt.show()