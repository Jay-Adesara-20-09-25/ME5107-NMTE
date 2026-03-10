import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("solution.dat")

Nvalues = [10,20,30]

# -------------------------
# Individual plots
# -------------------------

for N in Nvalues:

    d = data[data[:,0] == N]

    x = d[:,1]
    y = d[:,2]

    plt.figure()
    plt.plot(x, y, marker='o')
    plt.xlabel("j")
    plt.ylabel("x(i,j)")
    plt.title(f"x(i,j) vs j  (N = {N})")
    plt.grid()

    plt.savefig(f"plot_N_{N}.png", dpi=300)
    plt.close()


# -------------------------
# Combined plot
# -------------------------

plt.figure()

for N in Nvalues:

    d = data[data[:,0] == N]

    x = d[:,1]
    y = d[:,2]

    plt.plot(x, y, marker='o', label=f"N={N}")

plt.xlabel("j")
plt.ylabel("x(i,j)")
plt.title("Combined Plot for N = 10,20,30")
plt.legend()
plt.grid()

plt.savefig("combined_plot_2.png", dpi=300)
plt.close()


# -------------------------
# Time plot
# -------------------------

time_data = np.loadtxt("time.dat")

plt.figure()
plt.plot(time_data[:,0], time_data[:,1], marker='o')
plt.xlabel("N")
plt.ylabel("Time (seconds)")
plt.title("Runtime vs N")
plt.grid()

plt.savefig("time_plot.png", dpi=300)
plt.close()