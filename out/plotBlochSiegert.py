#!/usr/bin/env python


def main():
    import pandas as pd
    import sys
    import argparse
    import matplotlib.pyplot as plt
    import numpy as np

    parser = argparse.ArgumentParser(description="Plots output from blochSiegert.cpp")
    parser.add_argument("-f", "--file", type=str, help="Filename", required=True)
    parser.add_argument("-rf", "--ramseyFringe", type=int, help="rf___ branch to draw")
    args = parser.parse_args()

    filename = args.file
    print("Loading...", filename)

    try:
        phi = np.array(root2array(filename, branches="phi")[0])
        gridMin = np.array(root2array(filename, branches="gridMin")[0])
        polyMin = np.array(root2array(filename, branches="polyMin")[0])
        params = np.array(root2array(filename, branches="params")[0])
    except:
        sys.exit()

    if args.ramseyFringe != None:
        try:
            branchname = "rf" + str(args.ramseyFringe)
            fringe = np.array(root2array(filename, branches=branchname)[0])
            wRange = np.array(root2array(filename, branches="wRange")[0])

            fig2 = plt.figure(branchname)
            ax2 = fig2.add_subplot(111)
            ax2.set(title=branchname)
            ax2.set(xlabel="w [rad/s]")
            ax2.set(ylabel="P(z)")
            ax2.plot(wRange, fringe)
            ax2.grid(True)

        except:
            print("Could not read branch ", branchname)

    if params[3] == 1:
        print("Circular RF Ramsey fringe")
    else:
        print("Linear RF Ramsey fringe")
    print("{W0_VAL, PRECESS_TIME, PULSE_TIME}")
    print(params[0], "  ", params[1], "  ", params[2])

    fig1 = plt.figure("blochSiegertShift")
    ax1 = fig1.add_subplot(111)

    ax1.plot(phi, params[0] - polyMin, label="Polynomial fit")
    ax1.plot(phi, params[0] - gridMin, label="Gridsearch")
    ax1.grid(True)
    ax1.set(title="Bloch Siergert shift for optimized Ramsey Fringes")
    ax1.set(xlabel="Initial phase angle [rad]")
    ax1.set(ylabel="Shift [rad/s]")
    ax1.legend()
    plt.show()
    return


if __name__ == "__main__":
    main()
