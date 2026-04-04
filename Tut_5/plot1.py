import numpy as np
import matplotlib.pyplot as plt

# Load data
dx1, err1 = np.loadtxt("first_order.txt", unpack=True)
dx2, err2 = np.loadtxt("second_order.txt", unpack=True)
dx4, err4 = np.loadtxt("fourth_order.txt", unpack=True)

# Create plot
plt.figure(figsize=(8,6))

plt.loglog(dx1, err1, 'o-', label='First Order')
plt.loglog(dx2, err2, 's-', label='Second Order')
plt.loglog(dx4, err4, '^-', label='Fourth Order')

plt.xlabel("Grid size (dx)")
plt.ylabel("∞-norm Error")
plt.title("Error vs Grid Size (Log-Log Plot)")

plt.grid(True, which="both", linestyle='--')
plt.legend()

# -----------------------------
# SAVE FIGURE (IMPORTANT)
# -----------------------------
plt.savefig("error_plot.png", dpi=300)   # High-quality image

plt.show()