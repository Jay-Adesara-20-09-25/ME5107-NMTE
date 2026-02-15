import pandas as pd
import matplotlib.pyplot as plt

# Data
data = {
    'm': [256, 512, 1024, 2048, 4096],
    'Case A: x(y^T z)': [0.000196, 0.000277, 0.000368, 0.001139, 0.00153],
    'Case B: (xy^T)z': [0.015462, 0.049242, 0.14783, 0.637627, 2.32494]
}

df = pd.DataFrame(data)

# Save the table to a CSV file
df.to_csv('performance_data.csv', index=False)

# Plotting both lines in one plot
plt.figure(figsize=(10, 6))
plt.plot(df['m'], df['Case A: x(y^T z)'], marker='o', label='Case A: $x(y^T z)$', linewidth=2)
plt.plot(df['m'], df['Case B: (xy^T)z'], marker='s', label='Case B: $(xy^T)z$', linewidth=2)

plt.xlabel('m (Dimension)')
plt.ylabel('Time (seconds) - Log Scale')
plt.title('Performance Comparison: Case A vs Case B')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)

# Using log scale for y-axis due to the significant difference in execution times
plt.yscale('log')

plt.tight_layout()
plt.savefig('performance_plot.png')