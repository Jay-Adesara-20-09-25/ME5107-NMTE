import matplotlib.pyplot as plt
import pandas as pd
import os

def plot_individual(filename, title_prefix, save_name):
    # Check if file exists to prevent crash
    if not os.path.exists(filename):
        print(f"Error: {filename} not found. Please run the C++ code first.")
        return

    # Read the data
    data = pd.read_csv(filename)
    iters = data['Iteration']
    h_vals = data['h_value']
    rates = data['Convergence_Rate']
    
    # Create a wide 1x2 figure (14 inches wide, 6 inches tall)
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    
    if 'Method' in data.columns:
        # --- Handling for Q1(c) Hybrid Method ---
        bisect_mask = data['Method'] == 'Bisection'
        newton_mask = data['Method'] == 'Newton'
        
        # Plot h vs Iteration
        ax1.plot(iters, h_vals, 'k-', alpha=0.3) # connecting line
        ax1.scatter(iters[bisect_mask], h_vals[bisect_mask], color='blue', s=60, label='Bisection Phase', zorder=5)
        ax1.scatter(iters[newton_mask], h_vals[newton_mask], color='red', marker='s', s=60, label='Newton Phase', zorder=5)
        
        # Plot Rates (Filter out 0 values so log scale doesn't break)
        valid_bisect = bisect_mask & (rates > 0)
        valid_newton = newton_mask & (rates > 0)
        
        ax2.plot(iters[valid_bisect], rates[valid_bisect], 'b-o', linewidth=2, markersize=7, label='$e_{k+1}/e_k$ (Bisection)')
        ax2.plot(iters[valid_newton], rates[valid_newton], 'r-s', linewidth=2, markersize=7, label='$e_{k+1}/e_k^2$ (Newton)')
        
    else:
        # --- Handling for Q1(a) Bisection and Q1(b) Newton ---
        color = 'blue' if 'a' in filename else 'red'
        marker = 'o' if 'a' in filename else 's'
        rate_label = '$e_{k+1}/e_k$' if 'a' in filename else '$e_{k+1}/e_k^2$'
        
        # Plot h vs Iteration
        ax1.plot(iters, h_vals, color=color, marker=marker, linestyle='-', linewidth=2, markersize=7)
        
        # Plot Rates (Filter out 0 values)
        valid_rates = rates > 0
        ax2.plot(iters[valid_rates], rates[valid_rates], color=color, marker=marker, linestyle='-', linewidth=2, markersize=7, label=rate_label)
        
    # --- Format Left Plot (h vs Iteration) ---
    ax1.set_title(f"{title_prefix}: Calculated h vs Iteration", fontsize=15, pad=10)
    ax1.set_xlabel("Iteration Number", fontsize=13)
    ax1.set_ylabel("Calculated h (W/m²K)", fontsize=13)
    ax1.grid(True, linestyle='--', alpha=0.7)
    if 'Method' in data.columns:
        ax1.legend(fontsize=12)
        
    # --- Format Right Plot (Convergence Rate) ---
    ax2.set_title(f"{title_prefix}: Rate of Convergence", fontsize=15, pad=10)
    ax2.set_xlabel("Iteration Number (k)", fontsize=13)
    ax2.set_ylabel("Convergence Rate", fontsize=13)
    ax2.set_yscale('log') # Log scale is essential for viewing extreme Newton variations
    ax2.grid(True, linestyle='--', alpha=0.7)
    ax2.legend(fontsize=12)
    
    # Adjust layout to prevent overlapping and save
    plt.tight_layout()
    plt.savefig(save_name, dpi=300, bbox_inches='tight')
    print(f"Saved clear, high-resolution plot to '{save_name}'")
    
    # Show the plot on screen, then close it from memory after the user closes the window
    plt.show()
    plt.close()

def main():
    print("Generating plots...")
    plot_individual('q1a_data.csv', "Q1(a) Bisection Method", 'Plot_Q1a_Bisection.png')
    plot_individual('q1b_data.csv', "Q1(b) Newton's Method", 'Plot_Q1b_Newton.png')
    plot_individual('q1c_data.csv', "Q1(c) Hybrid Method", 'Plot_Q1c_Hybrid.png')
    print("All plots generated successfully! Check your folder for the 3 new PNG images.")

if __name__ == "__main__":
    main()