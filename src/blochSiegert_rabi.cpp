// Draws a graph of Bloch Siegert shift in optimized linear or circular rabi fringes
// as a function of pi pulse time
//
// To determine the BS shift for a linear fringe, searches for the lowest
// local minimun in the vicinity of w_0, the expected resonant frequency.
//
// Fits a quadratic polynomial to the bottom of the fringe.
//
// Outputs: blochSiegert_rabi.txt, with columns pulseWidth (s), wRange (ramsey fringe freqs)
//          gridMin(minimums on rabi fringe from grid search),
//          polyMin (minimums on rabi fringe from fitting curve to polynomial),
//          Header has params {W0_VAL, INT_ID}
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

const double TIME_INIT = 1;  // Initial phase angle to scan
const double TIME_FINAL = 6; // Final Phase angle to scane
const double TIME_STEP = 0.05;   // [rad]

// Ramsey Fringe parameters
const double W_STEP = 5e-7;       //[rad s^-1]    Step width of search around w0
const int W_STEP_NUM = 100;       // Number of steps to search around w0 (both < and >= of W0_VAL)
const double W0_VAL = 183.247172; //[rad s^-1]    Static field strength
const double PHI_INIT = 0;        //[rad] Initial phase angle of RF

// Integration parameters
const double INT_ID = USE_LINEAR_RF; // Type of RF pulse (USE_CIRCULAR_RF or USE_LINEAR_RF)
const double RK_STEP = 0.001;        // [seconds] For Runge Kutta integrator
                                     // PULSE_TIME cannot have more sig figs than RK_STEP!

// Output precision to stdout and file
const int PRECISION = 12;

int main()
{
    vector<double> fringe, tRange, wRange, polyCoeff;
    vector<double> wRangeAdj; // Used for polynomial fitting
    vector<double> gridSearchMin, polyFitMin;
    vector<double>::iterator min;
    string filename, branchname;
    ofstream outfile;
    neutron ucn;
    double phiVal2, wl;

    // Vvectors of parameters to scan, calculate optimal ramsey pulse time
    double temp = TIME_INIT;
    tRange.push_back(temp);
    for (int i = 0; temp <= TIME_FINAL; i++)
    {
        temp += TIME_STEP;
        tRange.push_back(temp);
    }
    for (int i = 0; i < W_STEP_NUM * 2; i++)
    {
        wRange.push_back(W0_VAL - (double)W_STEP_NUM * W_STEP + (double)i * W_STEP);
        wRangeAdj.push_back(-(double)W_STEP_NUM * W_STEP + (double)i * W_STEP);
    }

    // For progress tracking
    int phaseProgress = tRange.size();
    int wProgress = wRange.size();

    // For file output
    if (INT_ID == USE_LINEAR_RF)
    {
        filename = "linBlochSiegertRabi.txt";
    }
    else
    {
        filename = "circBlochSiegertRabi.txt";
    }

    // Make a ramsey fringe for each value in tRange
    int counterTime = 1;
    int counterW = 0;
    double progress;
    for (auto t : tRange)
    {
        wl = ((2 - INT_ID) * PI) / t;
        fringe.clear();

        // update progress
        cout << "Fringe " << counterTime << " / " << tRange.size() << endl;
        cout << "0%..." << flush;
        progress = 0.1;
        counterW = 0;

        for (auto wVal : wRange)
        {
            ucn.setState({1, 0, 0, 0});
            phiVal2 = wVal * t + PHI_INIT + wVal;
            ucn.integrate(t, RK_STEP, {wVal, W0_VAL, wl, PHI_INIT, INT_ID});
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
        branchname = "rf" + to_string(counterTime) + ".txt";
        outfile.open(branchname);
        outfile.precision(PRECISION);
        outfile << "#pulseWidth=" << t << "\n"
                << "#w,zProb\n";
        for (int i = 0; i < wRange.size(); i++)
        {
            outfile << wRange[i] << "," << fringe[i] << "\n";
        }
        outfile.close();

        cout << "100%" << endl;
        counterTime++;
    }

    cout << "\nSaving output to " << filename << "...";

    outfile.open(filename);
    outfile << "#W0_VAL=" << W0_VAL << ",INT_ID=" << INT_ID << "\n";
    outfile.precision(PRECISION);
    outfile << "#pulseWidth,gridMin,polyMin\n";

    for (int i = 0; i < tRange.size(); i++)
    {
        outfile << tRange[i] << "," << gridSearchMin[i] << ','
                << polyFitMin[i] << "\n";
    }
    outfile.close();

    cout << "Done!\n";

    return 0;
}
