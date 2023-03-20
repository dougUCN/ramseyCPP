// Sample program that does a circular
// rabi pi pulse, compares to analytical solution
// Also does a linear rabi pi pulse
//
// Outputs: circRabi.txt, linRabi.txt
// Each text file contains columns time, xProb, yProb, zProb,
// and params {W_VAL, W0_VAL, WC_VAL, PHI_VAL}

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include "neutron.hpp"

using namespace std;

// Some initial parameters
const double W_VAL = 20;      //[rad s^-1]
const double W0_VAL = 20;     //[rad s^-1]    B0 field strength
const double WC_VAL = PI / 2; //[rad s^-1]   Circ RF strength
const double WL_VAL = PI;     //[rad s^-1] Lin RF strength
const double PHI_VAL = 0;     //[rad]          RF pulse inital phase

// Initial "orientation" of neutron spin
// Complex and real parts of the ket, psi= |a,b>
const double A_REAL = 1;
const double A_COMP = 0;
const double B_REAL = 0;
const double B_COMP = 0;

// Step taken by integrator and total period of integration
// Please note MAX_TIME cannot have more sig figs than TIME_STEP
const double MAX_TIME = 2;      //[seconds]
const double TIME_STEP = 0.001; // [seconds]

// Output precision to stdout and file
const int PRECISION = 12;

double analytical(double w, double w0, double wc, double t);

int main()
{
    vector<vector<double>> t(2), x(2), y(2), z(2);
    neutron circ({A_REAL, A_COMP, B_REAL, B_COMP});
    neutron lin({A_REAL, A_COMP, B_REAL, B_COMP});
    vector<double> params = {W_VAL, W0_VAL, WC_VAL, PHI_VAL, USE_CIRCULAR_RF};
    vector<double> params2 = {W_VAL, W0_VAL, WL_VAL, PHI_VAL, USE_LINEAR_RF};
    ofstream outfile;
    vector<string> outnames = {"circRabi.txt", "linRabi.txt"};

    // Apply Rabi Pulse
    circ.integrate(MAX_TIME, TIME_STEP, params, t[0], x[0], y[0], z[0]);
    lin.integrate(MAX_TIME, TIME_STEP, params2, t[1], x[1], y[1], z[1]);

    // Output
    cout << "### Odds of measuring spin up along z ###\n";
    cout << "circ: " << getZProb(circ.getState()) << endl;
    cout << "lin: " << getZProb(lin.getState()) << endl;
    cout << "Difference between analytical and numerical sol (circular RF): ";
    cout << setprecision(PRECISION)
         << analytical(W_VAL, W0_VAL, WC_VAL, MAX_TIME) - getZProb(circ.getState()) << endl;
    cout << "Difference between numerical circ and numerical linear RF: ";
    cout << setprecision(PRECISION)
         << getZProb(lin.getState()) - getZProb(circ.getState()) << "\n\n";

    // Save output to text
    for (int i = 0; i < outnames.size(); ++i)
    {
        cout << "Saving spin tracking to " << outnames[i] << "...";
        outfile.open(outnames[i]);
        outfile << "#W_VAL=" << W_VAL << ",W0_VAL=" << W0_VAL
                << ",WC_VAL=" << WC_VAL << ",WL_VAL=" << WL_VAL
                << ",PHI_VAL=" << PHI_VAL << "\n";
        outfile.precision(PRECISION);
        outfile << "#time,xProb,yProb,zProb\n";
        for (int j = 0; j < t[i].size(); j++)
        {
            outfile << t[i][j] << "," << x[i][j] << ","
                    << y[i][j] << "," << z[i][j] << "\n";
        }
        outfile.close();
        cout << "Done\n";
    }

    return 0;
}

double analytical(double w, double w0, double wc, double t)
// Analytical solution to a ramsey pulse with a neutron starting spin up
// May thesis eq 3.51
{
    double omega = sqrt((w - w0) * (w - w0) + wc * wc) / 2;
    return 1 - (wc * wc / ((w - w0) * (w - w0) + wc * wc) * sin(omega * t) * sin(omega * t));
}
