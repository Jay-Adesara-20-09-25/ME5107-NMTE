import numpy as np
import matplotlib.pyplot as plt

# ==========================================================
# Methods used
# ==========================================================
methods = [
    "ExplicitEuler",
    "ImplicitEuler",
    "Trapezoidal",
    "RK2",
    "RK4",
    "RKF45"
]

# ==========================================================
# PLOT 1: ALL METHODS TOGETHER (h = 0.1)
# ==========================================================
plt.figure(figsize=(10,6))

for method in methods:
    data = np.loadtxt(f"{method}_h_0.100000.txt", skiprows=1)

    time = data[:,0]
    numerical = data[:,1]
    exact = data[:,2]

    plt.plot(time, numerical, label=method)

# Plot exact once
plt.plot(time, exact, 'k--', linewidth=2, label="Exact")

plt.title("Comparison of Numerical Methods (h = 0.1)")
plt.xlabel("Time (t)")
plt.ylabel("Solution y(t)")
plt.legend()
plt.grid()

plt.savefig("all_methods_h_0.1.png", dpi=300)
plt.show()


# ==========================================================
# PLOT 2: EACH METHOD VS EXACT (SEPARATE PLOTS)
# ==========================================================
for method in methods:

    data = np.loadtxt(f"{method}_h_0.100000.txt", skiprows=1)

    time = data[:,0]
    numerical = data[:,1]
    exact = data[:,2]

    plt.figure(figsize=(8,5))

    # Numerical solution
    plt.plot(time, numerical, label=method)

    # Exact solution
    plt.plot(time, exact, 'k--', linewidth=2, label="Exact")

    plt.title(f"{method} vs Exact (h = 0.1)")
    plt.xlabel("Time (t)")
    plt.ylabel("Solution y(t)")
    plt.legend()
    plt.grid()

    # Save file
    plt.savefig(f"{method}_vs_exact.png", dpi=300)

    plt.close()  # prevent overlap

print("All plots generated successfully!")