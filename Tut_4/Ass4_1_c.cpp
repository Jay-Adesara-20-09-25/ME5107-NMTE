// ME23B002 - Jay Adesara
// Problem 1(c): Hybrid Method (10 Bisection iterations -> Newton's Method)

#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

double erf_trapezoidal(double beta) {
    if (beta == 0.0) return 0.0;
    int N = 51;
    double a = 0.0, b = beta;
    double dx = (b - a) / (N - 1);
    
    double sum = exp(-a * a) + exp(-b * b);
    for (int i = 2; i <= N - 1; ++i) {
        double x_i = a + (i - 1) * dx;
        sum += 2.0 * exp(-x_i * x_i);
    }
    double pi = acos(-1.0);
    return (2.0 / sqrt(pi)) * (dx / 2.0) * sum;
}

double erfc_custom(double beta) {
    return 1.0 - erf_trapezoidal(beta);
}

void calculate_f_and_df(double h, double &f_val, double &df_val) {
    double Ti = 25.0, Tinf = 300.0, T_target = 53.5;
    double t = 400.0, k = 400.0, alpha = 1e-4;
    double LHS = (T_target - Ti) / (Tinf - Ti);
    double c = sqrt(alpha * t) / k; 
    double beta = c * h;
    
    double shared_term = exp(beta * beta) * erfc_custom(beta);
    f_val = 1.0 - shared_term - LHS;
    
    double pi = acos(-1.0);
    df_val = c * ((2.0 / sqrt(pi)) - 2.0 * beta * shared_term);
}

double f(double h) {
    double f_val, df_val;
    calculate_f_and_df(h, f_val, df_val);
    return f_val;
}

int main() {
    double h_low = 1.0, h_high = 10000.0, epsilon = 1e-5;
    
    vector<double> h_history;
    vector<string> method_used;
    
    double f_low = f(h_low);

    // --- PHASE 1: BISECTION (Iterations 1 to 10) ---
    for (int i = 1; i <= 10; ++i) {
        double h_mid = h_low + (h_high - h_low) / 2.0;
        h_history.push_back(h_mid);
        method_used.push_back("Bisection");
        
        double f_mid = f(h_mid);
        if (f_low * f_mid < 0) { 
            h_high = h_mid; 
        } else { 
            h_low = h_mid; f_low = f_mid; 
        }
    }

    // --- PHASE 2: NEWTON'S METHOD (Iterations 11 onwards) ---
    // Start with the 10th calculated bisection value
    double h_current = h_history.back(); 
    
    while (true) {
        double f_val, df_val;
        calculate_f_and_df(h_current, f_val, df_val);
        
        double h_next = h_current - (f_val / df_val);
        h_history.push_back(h_next);
        method_used.push_back("Newton"); // Records iteration 11, 12...
        
        if (abs(h_next - h_current) < epsilon) break;
        h_current = h_next;
    }

    double h_exact = h_history.back(); 
    int total_iters = h_history.size();
    
    ofstream out("q1c_data.csv");
    out << "Iteration,Method,h_value,Error,Convergence_Rate\n";
    
    for (size_t k = 0; k < h_history.size() - 1; ++k) {
        double e_k = abs(h_history[k] - h_exact);
        double e_k1 = abs(h_history[k+1] - h_exact);
        double rate = 0.0;
        
        if (e_k > 0) {
            // Check method to apply appropriate rate equation
            if (method_used[k] == "Bisection") rate = 0.5;
            else rate = e_k1 / (e_k * e_k);
        }
        
        out << (k+1) << "," << method_used[k] << "," << h_history[k] 
            << "," << e_k << "," << rate << "\n";
    }
    out.close();
    
    cout << "--- Q1(c): Hybrid Method ---\n";
    cout << "Converged in: " << total_iters << " total iterations (10 Bisection + " 
         << (total_iters - 10) << " Newton)\n";
    cout << "Calculated h: " << fixed << setprecision(5) << h_exact << " W/m^2.K\n\n";
    return 0;
}