// Draws a graph of Bloch Siegert shift in optimized linear or circular ramsey fringes
// as a function of the initial RF phase angle phi
//
// To determine the BS shift for a linear fringe, searches for the lowest
// local minimun in the vicinity of w_0, the expected resonant frequency.
//
// Fits a quadratic polynomial to the bottom of the fringe. The central peak of
// ramsey fringe has width 1/T, where T = precession period
//
// Outputs: blochSiegert.txt, with columns phi (rad), wRange (ramsey fringe freqs)
//          gridMin(minimums on ramsey fringe from grid search),
//          polyMin (minimums on ramsey fringe from fitting curve to polynomial),
//          Header has params {W0_VAL, PRECESS_TIME, PULSE_TIME, INT_ID}
//
//          Separately outputs every ramsey fringe made, numbered
//          rf1, rf2.... etc

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "neutron.hpp"
#include "polyfit.hpp"

using namespace std;

const double PHASE_INIT = 0;       // Initial phase angle to scan
const double PHASE_FINAL = 2 * PI; // Final Phase angle to scane
// const double PHASE_STEP = 0.1;     // [rad]
const double PHASE_STEP = 0.5; // [rad]

// Ramsey Fringe parameters
const double PRECESS_TIME = 180;  // Seconds
const double W_STEP = 5e-7;       //[rad s^-1]    Step width of search around w0
const int W_STEP_NUM = 100;       // Number of steps to search around w0 (both < and >= of W0_VAL)
const double W0_VAL = 183.247172; //[rad s^-1]    Static field strength
const double PULSE_TIME = 4.286;  //[seconds]  Time in which pi/2 pulse applied

// Integration parameters
const double INT_ID = USE_LINEAR_RF; // Type of RF pulse (USE_CIRCULAR_RF or USE_LINEAR_RF)
const double RK_STEP = 0.001;        // [seconds] For Runge Kutta integrator
                                     // PULSE_TIME cannot have more sig figs than RK_STEP!

// Output precision to stdout and file
const int PRECISION = 12;

int main()
{
    vector<double> fringe, phaseRange, wRange, polyCoeff;
    vector<double> wRangeAdj; // Used for polynomial fitting
    vector<double> gridSearchMin, polyFitMin;
    vector<double>::iterator min;
    string filename, branchname;
    ofstream outfile;
    neutron ucn;
    double phiVal2, wl;

    // Vvectors of parameters to scan, calculate optimal ramsey pulse time
    wl = PI / PULSE_TIME;
    double temp = PHASE_INIT;
    phaseRange.push_back(temp);
    for (int i = 0; temp <= PHASE_FINAL; i++)
    {
        temp += PHASE_STEP;
        phaseRange.push_back(temp);
    }
    for (int i = 0; i < W_STEP_NUM * 2; i++)
    {
        wRange.push_back(W0_VAL - (double)W_STEP_NUM * W_STEP + (double)i * W_STEP);
        wRangeAdj.push_back(-(double)W_STEP_NUM * W_STEP + (double)i * W_STEP);
    }

    // For progress tracking
    int phaseProgress = phaseRange.size();
    int wProgress = wRange.size();

    // For file output
    if (INT_ID == USE_LINEAR_RF)
    {
        filename = "linBlochSiegert.txt";
    }
    else
    {
        filename = "circBlochSiegert.txt";
        wl = (2 * PI) / PULSE_TIME;
    }

    // Make a ramsey fringe for each value in phaseRange
    int counterPhi = 1;
    int counterW = 0;
    double progress;
    for (auto phi : phaseRange)
    {
        fringe.clear();

        // update progress
        cout << "Fringe " << counterPhi << " / " << phaseRange.size() << endl;
        cout << "0%..." << flush;
        progress = 0.1;
        counterW = 0;

        for (auto wVal : wRange)
        {
            ucn.setState({1, 0, 0, 0});
            phiVal2 = wVal * PULSE_TIME + phi + wVal * PRECESS_TIME;
            ucn.integrate(PULSE_TIME, RK_STEP, {wVal, W0_VAL, wl, phi, INT_ID});
            ucn.larmorPrecess(PRECESS_TIME, W0_VAL);
            ucn.integrate(PULSE_TIME, RK_STEP, {wVal, W0_VAL, wl, phiVal2, INT_ID});
            fringe.push_back(getZProb(ucn.getState()));

            // Output progress
            // Print progress
            if ((double)counterW / (double)wRange.size() >= progress)
            {
                cout << progress * 100 << "%..." << flush;
                progress += 0.1;
            }
            counterW++;
        }
        // Find minimum value in fringe via grid search, store resonant freq
        min = min_element(fringe.begin(), fringe.end());
        gridSearchMin.push_back(wRange[distance(fringe.begin(), min)]);

        // Find minimum value of freq via quadratic polynomial fit
        // Min of a quadratic function is x = -b/2a
        polyCoeff = polyfit(wRangeAdj, fringe, 2);
        polyFitMin.push_back(-polyCoeff[1] / (2 * polyCoeff[2]) + W0_VAL);

        // Save output to file
        branchname = "rf" + to_string(counterPhi) + ".txt";
        outfile.open(branchname);
        outfile.precision(PRECISION);
        outfile << "#phi=" << phi << "\n"
                << "#w,zProb\n";
        for (int i = 0; i < wRange.size(); i++)
        {
            outfile << wRange[i] << "," << fringe[i] << "\n";
        }
        outfile.close();

        cout << "100%" << endl;

        // // debugging
        // for (auto coeff : polyCoeff) cout << coeff << "  ";
        // cout << "\ngridSearchMin: ( " << setprecision(12) << polyFitMin.back()+W0_VAL << " , " << polyval(polyCoeff,{-polyCoeff[1]/(2*polyCoeff[2])})[0] << " )" << endl;
        // cout << "gridSearchMin: ( " << setprecision(12) << gridSearchMin.back() << " , " << *min << " )" << endl;

        counterPhi++;
    }

    cout << "\nSaving output to " << filename << "...";

    outfile.open(filename);
    outfile << "#W0_VAL=" << W0_VAL << ",PRECESS_TIME=" << PRECESS_TIME
            << ",PULSE_TIME=" << PULSE_TIME << ",INT_ID=" << INT_ID << "\n";
    outfile.precision(PRECISION);
    outfile << "#phi,gridMin,polyMin\n";

    for (int i = 0; i < phaseRange.size(); i++)
    {
        outfile << phaseRange[i] << "," << gridSearchMin[i] << ','
                << polyFitMin[i] << "\n";
    }
    outfile.close();

    cout << "Done!\n";

    return 0;
}
