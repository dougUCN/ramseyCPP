#!/usr/bin/env python
import pandas as pd
import re
import argparse
import matplotlib.pyplot as plt
import numpy as np


def main():
    parser = argparse.ArgumentParser(description="Plots output from blochSiegert.cpp")
    parser.add_argument("-f", "--file", type=str, help="Filename", required=True)
    parser.add_argument(
        "-rf", "--ramseyFringe", type=str, nargs="+", help="rf__.txt file(s) to draw"
    )
    args = parser.parse_args()

    print(f"Loading {args.file}")
    df = pd.read_csv(
        args.file, comment="#", names=["phi", "gridSearchMin", "polyFitMin"]
    )
    print(df)
    bloch_siegert_params = parse_params(args.file)
    print(bloch_siegert_params)
    # print("{W0_VAL, PRECESS_TIME, PULSE_TIME}")
    # print(params[0], "  ", params[1], "  ", params[2])

    plt.figure()
    plt.plot(
        df["phi"].to_numpy(),
        bloch_siegert_params["W0_VAL"] - df["polyFitMin"].to_numpy(),
        label="Polynomial fit",
    )
    plt.plot(
        df["phi"].to_numpy(),
        bloch_siegert_params["W0_VAL"] - df["gridSearchMin"].to_numpy(),
        label="Grid search",
    )
    plt.grid(True)
    plt.xlabel(r"$\phi$ [rad]")
    plt.ylabel(r"$\Delta$Bloch-Siegert [rad/s]")
    plt.legend()

    if args.ramseyFringe:
        fringe = {}
        for fringe_file in args.ramseyFringe:
            print(f"Loading {fringe_file}")
            phi = parse_params(fringe_file)["phi"]
            fringe[phi] = pd.read_csv(
                fringe_file, comment="#", header=0, names=["w", "zProb"]
            )

            plt.figure()
            plt.title(rf"$\phi$={phi} [rad]")
            plt.xlabel(r"$\omega$ [rad/s]")
            plt.ylabel("P(z)")
            plt.plot(fringe[phi]["w"].to_numpy(), fringe[phi]["zProb"].to_numpy())
            plt.axvline(
                df.query("phi == @phi")["polyFitMin"].tolist()[0],
                label="polyFitMin",
                color="C1",
            )
            plt.axvline(
                df.query("phi == @phi")["gridSearchMin"].tolist()[0],
                label="gridSearchMin",
                color="C2",
            )
            plt.grid(True)
            plt.legend()

    plt.show()
    return


def parse_params(filename):
    params = {}
    with open(filename, "r") as infile:
        param_line = re.sub(r"\s+", "", infile.readline()[1:]).split(",")
        # regex deletes all whitespace. First character is assumed to be '#'.
        # Splitting parameters along the comma should gives the pair PARAM=VALUE

        for pair in param_line:
            split = pair.split("=")
            if len(split) != 2:
                raise (f"Could not parse string '{pair}'")
            params[split[0]] = float(split[1])

    return params


if __name__ == "__main__":
    main()
