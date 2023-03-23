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
        "-rabi", "--rabiFringe", type=str, nargs="+", help="rabi__.txt file(s) to draw"
    )
    args = parser.parse_args()

    print(f"Loading {args.file}")
    df = pd.read_csv(
        args.file, comment="#", names=["pulseWidth", "gridSearchMin", "polyFitMin"]
    )
    print(df)
    bloch_siegert_params = parse_params(args.file)
    print(bloch_siegert_params)

    # Bloch prediction
    bloch_x = np.linspace(df["pulseWidth"].min(), df["pulseWidth"].max(), 300)
    bloch_y = (2 * np.pi / bloch_x) ** 2 / (16 * bloch_siegert_params["W0_VAL"])

    plt.figure()
    plt.plot(
        df["pulseWidth"].to_numpy(),
        bloch_siegert_params["W0_VAL"] - df["polyFitMin"].to_numpy(),
        # label="Polynomial fit",
        color="#005F73",
        marker=".",
        linestyle="None",
        markersize=3,
    )
    plt.plot(bloch_x, bloch_y, color="#CA6702")
    plt.plot(bloch_x, (-1) * bloch_y, color="#CA6702")
    # plt.plot(
    #     df["pulseWidth"].to_numpy(),
    #     bloch_siegert_params["W0_VAL"] - df["gridSearchMin"].to_numpy(),
    #     label="Grid search",
    # )
    plt.grid(True)
    plt.xlabel(r"$\pi$ pulse width [s]")
    plt.ylabel(r"$\Delta$ Bloch-Siegert [rad/s]")
    plt.ticklabel_format(axis="y", useMathText=True)
    plt.legend()

    if args.rabiFringe:
        fringe = {}
        for fringe_file in args.rabiFringe:
            print(f"Loading {fringe_file}")
            pulseWdith = parse_params(fringe_file)["pulseWidth"]
            fringe[pulseWdith] = pd.read_csv(
                fringe_file, comment="#", header=0, names=["w", "zProb"]
            )

            plt.figure()
            plt.title(rf"$t$={pulseWdith} [s]")
            plt.xlabel(r"$\omega$ [rad/s]")
            plt.ylabel("P(z)")
            plt.plot(
                fringe[pulseWdith]["w"].to_numpy(),
                fringe[pulseWdith]["zProb"].to_numpy(),
            )
            plt.axvline(
                df.query("pulseWidth == @pulseWdith")["polyFitMin"].tolist()[0],
                label="polyFitMin",
                color="C1",
            )
            plt.axvline(
                df.query("pulseWdith == @pulseWdith")["gridSearchMin"].tolist()[0],
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
