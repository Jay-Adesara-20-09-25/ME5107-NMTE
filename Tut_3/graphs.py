import numpy as np
import matplotlib.pyplot as plt

Nvalues = [15,25,50]

# -------- Individual Graphs --------

i = 1

for N in Nvalues:

    data = np.loadtxt(f"solution_{N}.dat")

    x = data[:,0]
    y = data[:,1]

    plt.figure()
    plt.plot(x,y,marker='o')
    plt.xlabel("x_j")
    plt.ylabel("y_j")
    plt.title(f"Solution y_j vs x_j (N = {N})")
    plt.grid()

    plt.savefig(f'plot{i}.png')   # corrected
    i = i + 1

# -------- Combined Graph --------

plt.figure()

for N in Nvalues:

    data = np.loadtxt(f"solution_{N}.dat")

    x = data[:,0]
    y = data[:,1]

    plt.plot(x,y,marker='o',label=f"N={N}")

plt.xlabel("x_j")
plt.ylabel("y_j")
plt.title("Combined Plot for N = 15, 25, 50")
plt.legend()
plt.grid()

plt.savefig('combined_plot.png')

plt.show()