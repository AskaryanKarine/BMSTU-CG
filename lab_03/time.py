import matplotlib.pyplot as plt

plt.figure(1, figsize=(12, 7))

times = []
r = 0
with open("../lab_03/time_res.txt", "r") as f:
    r = float(f.readline().replace(",", "."))

    for line in f:
        times.append(float(line.replace(",", ".")) * 1000000)

y = range(len(times))

labels = ("ЦДА", "Брезенхем\nдействительный", "Брезенхем\nцелочисленный", "Брезенхем\nсглаживающий", "Ву")

plt.bar(y, times)
plt.xticks(y, labels)

plt.ylabel(f"Время в мкс.\nДлина отрезка - {r}")
plt.show()