import numpy as np
import matplotlib.pyplot as plt

# ==========================================================
# Methods and step sizes used in C++ code
# ==========================================================
methods = [
    "ExplicitEuler","ImplicitEuler","Trapezoidal",
    "RK2","RK4","RKF45"
]

h_vals = ["0.100000","0.050000","0.025000","0.012500","0.006250"]

# ==========================================================
# PLOT 1: Compare all methods (h = 0.1)
# ==========================================================
plt.figure(figsize=(10,6))

for method in methods:
    # Load data from file
    data = np.loadtxt(f"{method}_h_0.100000.txt", skiprows=1)

    time = data[:,0]
    numerical = data[:,1]
    exact = data[:,2]

    # Plot numerical solution
    plt.plot(time, numerical, label=method)

# Plot exact solution once
plt.plot(time, exact, 'k--', linewidth=2, label="Exact")

plt.title("Comparison of Numerical Methods (h = 0.1)")
plt.xlabel("Time (t)")
plt.ylabel("Solution y(t)")
plt.legend()
plt.grid()

plt.savefig("all_methods.png", dpi=300)
plt.show()

# ==========================================================
# PLOT 2: Convergence plot for RK4 (different h)
# ==========================================================
plt.figure(figsize=(10,6))

for h in h_vals:
    data = np.loadtxt(f"RK4_h_{h}.txt", skiprows=1)

    time = data[:,0]
    numerical = data[:,1]

    plt.plot(time, numerical, label=f"h={h}")

# Plot exact solution (from finest grid)
data = np.loadtxt(f"RK4_h_{h_vals[-1]}.txt", skiprows=1)
plt.plot(data[:,0], data[:,2], 'k--', linewidth=2, label="Exact")

plt.title("RK4 Convergence with Step Size")
plt.xlabel("Time (t)")
plt.ylabel("Solution y(t)")
plt.legend()
plt.grid()

plt.savefig("RK4_convergence.png", dpi=300)
plt.show()

# ==========================================================
# OPTIONAL: Convergence plots for all methods
# ==========================================================
for method in methods:

    plt.figure(figsize=(8,5))

    for h in h_vals:
        data = np.loadtxt(f"{method}_h_{h}.txt", skiprows=1)

        plt.plot(data[:,0], data[:,1], label=f"h={h}")

    # Exact solution
    data = np.loadtxt(f"{method}_h_{h_vals[-1]}.txt", skiprows=1)
    plt.plot(data[:,0], data[:,2], 'k--', label="Exact")

    plt.title(f"{method} Convergence")
    plt.xlabel("Time (t)")
    plt.ylabel("Solution y(t)")
    plt.legend()
    plt.grid()

    plt.savefig(f"{method}_convergence.png", dpi=300)
    plt.close()

print("All plots generated successfully!")