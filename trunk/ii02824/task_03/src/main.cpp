#include <iostream>
#include "models.h"
#include "pid.h"
#include <fstream>
#include <cstdlib>


int main()
{
    const double K1  = 0.06;  ///< коэффициент передачи
    const double T1  = 20.0; ///< постоянная интегрирования
    const double Td1 = 0.0;  ///< постоянная дифференцирования
    const double T01 = 1.0;  ///< шаг
    
    pid1_coeffs coeffs(K1, T1, Td1, T01);
    pid1 PID(coeffs, 1, 2, 3);

    const double a_lin = 0.8; ///< коэффициент линейной модели a
    const double b_lin = 0.1; ///< коэффициент линейной модели b

    NonLinearCoeffs coeffs_nl;
    coeffs_nl.a1 = 0.8;  ///< коэффициент нелинейной модели a
    coeffs_nl.b1 = 0.0;  ///< коэффициент нелинейной модели b
    coeffs_nl.c1 = 0.1;  ///< коэффициент нелинейной модели c
    coeffs_nl.d1 = 0.05; ///< коэффициент нелинейной модели d

    double y; ///< текущее значение температуры
    double w; ///< целевая температура
    int n;    ///< количество шагов моделирования

    std::cout << "Enter initial temperature -> ";
    std::cin >> y;
    std::cout << "Enter target temperature -> ";
    std::cin >> w;
    std::cout << "Enter number of steps -> ";
    std::cin >> n;

    double e; ///< значение отклонения 
    double u; ///< значение управляющего воздействия

    double y_nl = y;
    double y_prev = 0;
    double u_prev = 0;

    std::ofstream csv("results.csv");
    if (!csv.is_open()) {
        std::cerr << "Error: cannot create results.csv\n";
        return 1;
    }
    csv << "step,e,u,y_lin,y_nonlin\n";

    for (int i = 0; i < n; i++)
    {
        e = w - y;              ///< вычисление значение отклонения 
        u = PID.process(e);     ///< нахождение значения управляющего воздействия

        y = linear(y, u, a_lin, b_lin); ///< расчёт новой температуры по линейной модели

        y_nl = non_linear(y_nl, y_prev, u, u_prev, coeffs_nl); ///< расчёт новой температуры по нелинейной модели
        y_prev = y_nl;
        u_prev = u;

        std::cout << "Step " << i + 1
            << " - e = " << e
            << ", u = " << u
            << ", y_lin = " << y
            << ", y_nonlin = " << y_nl
            << ";\n";

        csv << i + 1 << "," << e << "," << u << "," << y << "," << y_nl << "\n";

    }

    csv.close();
    std::cout << "\nResults saved to results.csv\n";

    std::cout << "Launching Python plotter...\n";
    int result = system("python plot_results.py results.csv");
    if (result != 0) {
        std::cerr << "Failed to run Python script.\n"
            << "Make sure Python is installed, matplotlib is available, and plot_results.py is in the same folder.\n";
    }

    return 0;
}