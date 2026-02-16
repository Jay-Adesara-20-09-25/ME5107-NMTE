import matplotlib.pyplot as plt   # Import plotting library

# y–coordinates of interior nodes (in meters)
# Grid spacing Δy = 0.25 m
y = [0.25, 0.50, 0.75, 1.00]

# Temperatures obtained from Gaussian elimination solution
# Column at x = 0.25 m  →  T1, T3, T5, T7
Tx_025 = [489.305, 472.065, 436.950, 356.995]

# Column at x = 0.50 m  →  T2, T4, T6, T8
Tx_050 = [485.154, 462.006, 418.739, 339.052]

# Plot temperature vs y for x = 0.25 m
plt.plot(Tx_025, y, marker='o', label='x = 0.25 m')

# Plot temperature vs y for x = 0.50 m
plt.plot(Tx_050, y, marker='s', label='x = 0.50 m')

# Axis labels
plt.xlabel("Temperature (K)")
plt.ylabel("y (m)")

# Plot title
plt.title("Temperature Variation vs y")

# Add grid lines for clarity
plt.grid(True)

# Show legend indicating both curves
plt.legend()

# Save figure as image file
plt.savefig("temperature_vs_y.png")

# Display plot window
plt.show()
