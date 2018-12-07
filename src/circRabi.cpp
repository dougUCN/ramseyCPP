// Sample program that does a circular
// rabi pi pulse, compares to analytical solution

#include <iostream>
#include <cmath>
#include <vector>
#include "neutron.h"
#include "TTree.h"
#include "TFile.h"

using namespace std;

// Some initial parameters
const double W_VAL = 20;   //[rad s^-1]
const double W0_VAL = 20;  //[rad s^-1]    Static field strength
const double WC_VAL = 1.57; //[rad s^-1]   Rotating field strength
const double PHI_VAL = 0; //[rad]          RF pulse inital phase

// Initial "orientation" of neutron spin
// Complex and real parts of the ket, psi= |a,b>
const double A_REAL = 1;
const double A_COMP = 0;
const double B_REAL = 0;
const double B_COMP = 0;

// Step taken by integrator and total period of integration
// Please note
const double MAX_TIME = 2;      //[seconds]
const double TIME_STEP = 0.001;    // [seconds]

double analytical(double w, double w0, double wc, double t);

int main()
{
    vector<double> t,x,y,z;
    neutron ucn({A_REAL, A_COMP, B_REAL, B_COMP});
    vector<double> params = {W_VAL, W0_VAL, WC_VAL, PHI_VAL, CIRC_ID};
    ucn.integrate(MAX_TIME, TIME_STEP, params, t,x,y,z);

    cout << "Difference between analytical and numerical solution: ";
    cout << setprecision(9) << analytical(W_VAL, W0_VAL, WC_VAL,MAX_TIME) - getZProb(ucn.getState()) << endl;

    // Save output to root tree
    cout << "Saving spin tracking to circRabi.root..."
    TFile *file = TFile::Open("circRabi.root","RECREATE");
    TTree *tree = new TTree("circRabi","t, xProb, yProb, zProb");
    tree->Branch("time",&t);
    tree->Branch("xProb",&x);
    tree->Branch("yProb",&y);
    tree->Branch("zProb",&z);
    tree->Fill();
    file->Write();
    cout << "Done!"

    return 0;
}

double analytical(double w, double w0, double wc, double t)
// Analytical solution to a ramsey pulse with a neutron starting spin up
// May thesis eq 3.51
{
    double omega = sqrt( (w-w0)*(w-w0) + wc*wc )/2;
    return 1-( wc*wc/( (w-w0)*(w-w0)+ wc*wc) * sin(omega*t) * sin(omega*t));
}
