// Sample program that does a circular
// rabi pi pulse, compares to analytical solution
// Also does a linear rabi pi pulse
//
// Outputs: circRabi.root, linRabi.root
// Each root file contains branches time, xProb, yProb, zProb,
// and a vector params that holds {W_VAL, W0_VAL, WC_VAL, PHI_VAL, CIRC_ID}

#include <iostream>
#include <cmath>
#include <vector>
#include "neutron.hpp"
#include "TTree.h"
#include "TFile.h"

using namespace std;

// Some initial parameters
const double W_VAL = 20;   //[rad s^-1]
const double W0_VAL = 20;  //[rad s^-1]    B0 field strength
const double WC_VAL = PI/2; //[rad s^-1]   Circ RF strength
const double WL_VAL = PI; //[rad s^-1] Lin RF strength
const double PHI_VAL = 0; //[rad]          RF pulse inital phase

// Initial "orientation" of neutron spin
// Complex and real parts of the ket, psi= |a,b>
const double A_REAL = 1;
const double A_COMP = 0;
const double B_REAL = 0;
const double B_COMP = 0;

// Step taken by integrator and total period of integration
// Please note MAX_TIME cannot have more sig figs than TIME_STEP
const double MAX_TIME = 2;      //[seconds]
const double TIME_STEP = 0.001;    // [seconds]

double analytical(double w, double w0, double wc, double t);

int main()
{
    vector<double> t,x,y,z, t2, x2, y2, z2;
    neutron circ({A_REAL, A_COMP, B_REAL, B_COMP});
    neutron lin({A_REAL, A_COMP, B_REAL, B_COMP});
    vector<double> params = {W_VAL, W0_VAL, WC_VAL, PHI_VAL, CIRC_ID};
    vector<double> params2 = {W_VAL, W0_VAL, WL_VAL, PHI_VAL, LIN_ID};

    // Apply Rabi Pulse
    circ.integrate(MAX_TIME, TIME_STEP, params, t,x,y,z);
    lin.integrate(MAX_TIME, TIME_STEP, params2, t2,x2,y2,z2);

    // Output
    cout << "LIN_ID: " << params2[4] << endl;
    cout << "### Odds of measuring spin up along z ###\n";
    cout << "circ: " << getZProb(circ.getState()) << endl;
    cout << "lin: " << getZProb(lin.getState()) << endl;
    cout << "Difference between analytical and numerical sol (circular RF): ";
    cout << setprecision(9) << analytical(W_VAL, W0_VAL, WC_VAL,MAX_TIME) - getZProb(circ.getState()) << endl;
    cout << "Difference between numerical circ and numerical linear RF: ";
    cout << setprecision(9) << getZProb(lin.getState()) - getZProb(circ.getState()) << "\n\n";

    // Save output to root trees
    cout << "Saving spin tracking to circRabi.root...";
    TFile *file = TFile::Open("circRabi.root","RECREATE");
    TTree *tree = new TTree("circRabi","t, xProb, yProb, zProb");
    tree->Branch("time",&t);
    tree->Branch("xProb",&x);
    tree->Branch("yProb",&y);
    tree->Branch("zProb",&z);
    tree->Branch("params",&params);
    tree->Fill();
    file->Write();

    cout << "Done!\nSaving spin tracking to linRabi.root...";
    TFile *file2 = TFile::Open("linRabi.root","RECREATE");
    TTree *tree2 = new TTree("linRabi","t, xProb, yProb, zProb, params");
    tree2->Branch("time",&t2);
    tree2->Branch("xProb",&x2);
    tree2->Branch("yProb",&y2);
    tree2->Branch("zProb",&z2);
    tree2->Branch("params",&params2);
    tree2->Fill();
    file2->Write();
    cout << "Done!\n";

    return 0;
}

double analytical(double w, double w0, double wc, double t)
// Analytical solution to a ramsey pulse with a neutron starting spin up
// May thesis eq 3.51
{
    double omega = sqrt( (w-w0)*(w-w0) + wc*wc )/2;
    return 1-( wc*wc/( (w-w0)*(w-w0)+ wc*wc) * sin(omega*t) * sin(omega*t));
}
