#ifndef NEUTRON_H
#define NEUTRON_H

#include <vector>
using namespace std;

const int LIN_ID = 0;
const int CIRC_ID = 1;
const int NUM_EQ = 4; // Number of equations in derivs

class neutron {
// vector<double> params should be in the form of {w, w0, wl, phi, INT_ID}
// w is the driving RF frequency in rad/s
// w0 is the strength of the applied B0 field in rad/s
// wRF is the strength of either the applied linear and circular RF fields in rad/s
// INT_ID is either LIN_ID (linear RF) or CIRC_ID (circular RF)
public:
    neutron( const vector<double>& ket) {setState(ket);}   // constructor
    void setState( const vector<double>& ket);
    vector<double> getState();
    void larmorPrecess(double precTime, double w0);   // Analytical larmor precession
    void rkStep(const double t, const double dt, const vector<double>& params);
    void integrate(const double time, const double dt, const vector<double>& params);
    void integrate(const double time, const double dt, const vector<double>& params,
        vector<double>& tOut, vector<double>& xOut, vector<double>& yOut, vector<double>& zOut);
private:
    // Systems to solve for linear/circular pi/2 pulses
    vector<double> derivs( const double t, const vector<double>& u, const vector<double>& params);
    vector<double> _u;  // State ket of neutron spin:  u=(Re(a),Im(a),Re(b),Im(b))
};

double getXProb(const vector<double>& u);  // Odds of measuring spin up along x
double getYProb(const vector<double>& u);  // Odds of measuring spin up along y
double getZProb(const vector<double>& u);  // Odds of measuring spin up along z

#endif
