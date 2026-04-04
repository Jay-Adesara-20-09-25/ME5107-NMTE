import numpy as np
import matplotlib.pyplot as plt

# -----------------------------
# Load data (skip header)
# -----------------------------
x, direct, transform, exact = np.loadtxt(
    "nonuniform_results.txt",
    skiprows=1,
    unpack=True
)

# =========================================================
# 🔹 PLOT 1: Exact vs Direct
# =========================================================
plt.figure(figsize=(8,6))

plt.plot(x, exact, 'k-', label='Exact')
plt.plot(x, direct, 'o--', label='Direct Method')

plt.xlabel("x")
plt.ylabel("f'(x)")
plt.title("Exact vs Direct Method")

plt.legend()
plt.grid(True)

plt.savefig("plot_direct.png", dpi=300)
plt.show()


# =========================================================
# 🔹 PLOT 2: Exact vs Transform
# =========================================================
plt.figure(figsize=(8,6))

plt.plot(x, exact, 'k-', label='Exact')
plt.plot(x, transform, 's--', label='Transform Method')

plt.xlabel("x")
plt.ylabel("f'(x)")
plt.title("Exact vs Transform Method")

plt.legend()
plt.grid(True)

plt.savefig("plot_transform.png", dpi=300)
plt.show()


# =========================================================
# 🔹 PLOT 3: Direct vs Transform
# =========================================================
plt.figure(figsize=(8,6))

plt.plot(x, direct, 'o--', label='Direct Method')
plt.plot(x, transform, 's--', label='Transform Method')

plt.xlabel("x")
plt.ylabel("f'(x)")
plt.title("Direct vs Transform Comparison")

plt.legend()
plt.grid(True)

plt.savefig("plot_methods.png", dpi=300)
plt.show()


# =========================================================
# 🔹 PLOT 4: ALL TOGETHER
# =========================================================
plt.figure(figsize=(8,6))

plt.plot(x, exact, 'k-', label='Exact')
plt.plot(x, direct, 'o--', label='Direct')
plt.plot(x, transform, 's--', label='Transform')

plt.xlabel("x")
plt.ylabel("f'(x)")
plt.title("All Methods Comparison")

plt.legend()
plt.grid(True)

plt.savefig("plot_all.png", dpi=300)

plt.show()