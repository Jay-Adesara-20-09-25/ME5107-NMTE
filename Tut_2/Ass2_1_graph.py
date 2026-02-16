import matplotlib.pyplot as plt   # Import matplotlib plotting library

# Matrix sizes used in computation
N = [32, 128, 512, 1024]

# Corresponding values of 1/(summation of x_i^2)
sum_vals = [1024, 16384, 262144, 1.04858e+06]

# Create a line plot with circular markers
plt.plot(N, sum_vals, marker='o', linestyle='-')

# Title of the graph
plt.title("N vs 1/(Summation of x_i^2)")

# Label for x-axis
plt.xlabel("Matrix Size (N)")

# Label for y-axis
plt.ylabel("1/(Summation of x_i^2)")

# Force matplotlib to display only these x-axis tick values
plt.xticks(N)

# Add grid lines for better readability
plt.grid(True)

# Save the generated plot as an image file
plt.savefig('plot1.png')

# Display the plot on screen
plt.show()
