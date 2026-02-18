import sys
import csv
import matplotlib.pyplot as plt

file = sys.argv[1] if len(sys.argv) > 1 else "results.csv"

step, e, u, y_lin, y_nonlin = [], [], [], [], []

with open(file, newline="") as f:
    reader = csv.DictReader(f)
    for row in reader:
        step.append(int(row["step"]))
        e.append(float(row["e"]))
        u.append(float(row["u"]))
        y_lin.append(float(row["y_lin"]))
        y_nonlin.append(float(row["y_nonlin"]))

plt.figure(figsize=(10, 6))
plt.plot(step, y_lin, label="Linear model", marker='o')
plt.plot(step, y_nonlin, label="Non-linear model", marker='s')
plt.xlabel("Step")
plt.ylabel("Output")
plt.title("PID Control Simulation")
plt.grid(True)
plt.legend()
plt.show()