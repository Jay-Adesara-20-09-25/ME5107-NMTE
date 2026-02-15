import matplotlib.pyplot as plt

# Your data
N = [256, 512, 1024, 2048, 4096]
times = [0.001186, 0.004711, 0.016759, 0.046647, 0.182107]

plt.plot(N, times, marker='o', linestyle='-', color='b')
plt.title("Matrix-Vector Multiplication Timing")
plt.xlabel("Matrix Size (N)")
plt.ylabel("Time (seconds)")
plt.grid(True)

# Save the plot as an image
plt.savefig('timing_plot.png')
plt.show()