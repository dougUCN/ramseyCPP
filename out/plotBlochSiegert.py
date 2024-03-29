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

    plt.figure()
    plt.plot(
        df["phi"].to_numpy(),
        bloch_siegert_params["W0_VAL"] - df["polyFitMin"].to_numpy(),
        label="Polynomial fit",
        color="#005F73",
    )
    # plt.plot(
    #     df["phi"].to_numpy(),
    #     bloch_siegert_params["W0_VAL"] - df["gridSearchMin"].to_numpy(),
    #     label="Grid search",
    # )
    plt.grid(True)
    plt.xlabel(r"$\phi$ [rad]")
    plt.ylabel(r"$\Delta$ Bloch-Sigert [rad/s]")
    plt.ticklabel_format(axis="y", useMathText=True)
    ax = plt.gca()
    ax.xaxis.set_major_locator(plt.MultipleLocator(np.pi / 2))
    ax.xaxis.set_minor_locator(plt.MultipleLocator(np.pi / 4))
    ax.xaxis.set_major_formatter(plt.FuncFormatter(multiple_formatter()))
    # plt.legend()

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


# https://stackoverflow.com/questions/40642061/how-to-set-axis-ticks-in-multiples-of-pi-python-matplotlib
def multiple_formatter(denominator=2, number=np.pi, latex="\pi"):
    def gcd(a, b):
        while b:
            a, b = b, a % b
        return a

    def _multiple_formatter(x, pos):
        den = denominator
        num = np.int(np.rint(den * x / number))
        com = gcd(num, den)
        (num, den) = (int(num / com), int(den / com))
        if den == 1:
            if num == 0:
                return r"$0$"
            if num == 1:
                return r"$%s$" % latex
            elif num == -1:
                return r"$-%s$" % latex
            else:
                return r"$%s%s$" % (num, latex)
        else:
            if num == 1:
                return r"$\frac{%s}{%s}$" % (latex, den)
            elif num == -1:
                return r"$\frac{-%s}{%s}$" % (latex, den)
            else:
                return r"$\frac{%s%s}{%s}$" % (num, latex, den)

    return _multiple_formatter


class Multiple:
    def __init__(self, denominator=2, number=np.pi, latex="\pi"):
        self.denominator = denominator
        self.number = number
        self.latex = latex

    def locator(self):
        return plt.MultipleLocator(self.number / self.denominator)

    def formatter(self):
        return plt.FuncFormatter(
            multiple_formatter(self.denominator, self.number, self.latex)
        )


if __name__ == "__main__":
    main()
