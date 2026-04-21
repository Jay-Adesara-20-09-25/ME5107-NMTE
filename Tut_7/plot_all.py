# ME23B002 - Jay Adesara
# Assignment 7 - Plotting script for all programming problems
# Generates figures matching the expected outputs in the solution PDF

import matplotlib
matplotlib.use('Agg')  # Non-interactive backend (no display required)
import matplotlib.pyplot as plt
import csv
import os
import math

# =====================================================================
# PLOT 1a: y_numerical vs y_exact vs time  (Programming 1)
# =====================================================================
def plot_prog1_y_vs_time():
    t_vals     = []
    y1_vals    = []
    yexact_vals = []

    with open("prog1_output.csv") as f:
        reader = csv.DictReader(f)
        for row in reader:
            t_vals.append(float(row["t"]))
            y1_vals.append(float(row["y1"]))
            yexact_vals.append(float(row["y_exact"]))

    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(t_vals, y1_vals,     label="y_num (Leapfrog)", color="blue",  linewidth=1.5)
    ax.plot(t_vals, yexact_vals, label="y_exact",           color="red",   linewidth=1.5,
            linestyle="--")
    ax.set_xlabel("time (s)", fontsize=12)
    ax.set_ylabel("y", fontsize=12)
    ax.set_title("Y values with respect to time\n(Simple Harmonic Motion - Leapfrog)", fontsize=12)
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.4)
    ax.set_xlim(0, 9)
    plt.tight_layout()
    plt.savefig("fig2_y_vs_time.png", dpi=150)
    plt.close()
    print("Saved: fig2_y_vs_time.png")


# =====================================================================
# PLOT 1b: Amplitude error at peak times  (Programming 1)
# =====================================================================
def plot_prog1_amplitude_error():
    t_vals   = []
    err_vals = []

    with open("prog1_amp_errors.csv") as f:
        reader = csv.DictReader(f)
        for row in reader:
            t_vals.append(float(row["time"]))
            err_vals.append(float(row["amplitude_error"]))

    fig, ax = plt.subplots(figsize=(8, 4))
    ax.plot(t_vals, err_vals, marker="o", color="darkorange",
            linewidth=1.5, markersize=6, label="Amplitude Error")
    ax.axhline(0, color="black", linewidth=0.8, linestyle="--")
    ax.set_xlabel("time step", fontsize=12)
    ax.set_ylabel("Amplitude error", fontsize=12)
    ax.set_title("Amplitude error vs time step\n(Leapfrog method peaks)", fontsize=12)
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.4)
    ax.set_xlim(0, 9)
    plt.tight_layout()
    plt.savefig("fig1_amplitude_error.png", dpi=150)
    plt.close()
    print("Saved: fig1_amplitude_error.png")


# =====================================================================
# PLOT 2: Species concentration vs log(t) - Trapezoidal method
# (Programming 2)
# =====================================================================
def plot_prog2_species():
    # Read the time-series CSV; filter Trapezoidal with smallest h = 1e-5
    t_vals  = []
    y1_vals = []
    y2_vals = []
    y3_vals = []

    target_method = "Trapezoidal"
    target_h      = "1e-05"

    with open("prog2_timeseries.csv") as f:
        reader = csv.DictReader(f)
        for row in reader:
            if row["method"] == target_method and abs(float(row["h"]) - 1e-5) < 1e-10:
                t = float(row["t"])
                if t > 0:  # skip t=0 for log scale
                    t_vals.append(t)
                    y1_vals.append(float(row["y1"]))
                    y2_vals.append(float(row["y2"]))
                    y3_vals.append(float(row["y3"]))

    if not t_vals:
        print("Warning: No Trapezoidal data found in prog2_timeseries.csv")
        return

    fig, ax = plt.subplots(figsize=(9, 5))
    ax.semilogx(t_vals, y1_vals, label="TM y1 (A)", color="blue",  linewidth=1.5)
    ax.semilogx(t_vals, y2_vals, label="TM y2 (B)", color="green", linewidth=1.5)
    ax.semilogx(t_vals, y3_vals, label="TM y3 (C)", color="red",   linewidth=1.5)
    ax.set_xlabel("log(t)", fontsize=12)
    ax.set_ylabel("Species concentration", fontsize=12)
    ax.set_title("Trapezoidal method (h = 1e-5)\nSpecies Concentration vs log(t)", fontsize=12)
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.4, which="both")
    ax.set_ylim(-0.1, 1.1)
    plt.tight_layout()
    plt.savefig("fig3_species_trapezoidal.png", dpi=150)
    plt.close()
    print("Saved: fig3_species_trapezoidal.png")


# =====================================================================
# PLOT 3: Shooting method converged solution y(x) and dy/dx
# (Programming 3)
# =====================================================================
def plot_prog3_solution():
    x_vals  = []
    y_vals  = []
    dy_vals = []

    with open("prog3_solution.csv") as f:
        reader = csv.DictReader(f)
        for row in reader:
            x_vals.append(float(row["x"]))
            y_vals.append(float(row["y"]))
            dy_vals.append(float(row["dydx"]))

    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(x_vals, y_vals,  label="y(x)",  color="blue",   linewidth=2)
    ax.plot(x_vals, dy_vals, label="dy/dx", color="darkorange", linewidth=2,
            linestyle="--")
    ax.axhline(0, color="black", linewidth=0.6, linestyle="-")
    ax.set_xlabel("x", fontsize=12)
    ax.set_ylabel("y(x) and dy/dx", fontsize=12)
    ax.set_title("Shooting Method - Converged Solution\n(y'' = y², y(0)=0, y'(1)=-0.8)", fontsize=12)
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.4)
    ax.set_xlim(0, 1)
    plt.tight_layout()
    plt.savefig("fig4_shooting_solution.png", dpi=150)
    plt.close()
    print("Saved: fig4_shooting_solution.png")


# =====================================================================
# MAIN
# =====================================================================
if __name__ == "__main__":
    errors = []

    for name, func in [
        ("Prog1 - Y vs Time",       plot_prog1_y_vs_time),
        ("Prog1 - Amplitude Error",  plot_prog1_amplitude_error),
        ("Prog2 - Species Conc.",    plot_prog2_species),
        ("Prog3 - Shooting Method",  plot_prog3_solution),
    ]:
        try:
            func()
        except Exception as e:
            errors.append(f"  {name}: {e}")

    if errors:
        print("\nWarnings (may be missing CSV files):")
        for e in errors:
            print(e)
    else:
        print("\nAll plots generated successfully!")
