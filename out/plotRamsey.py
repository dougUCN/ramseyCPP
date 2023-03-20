#!/usr/bin/env python
import pandas as pd
import argparse
import matplotlib.pyplot as plt
import re


def main():
    parser = argparse.ArgumentParser(description="Plots ramsey fringe from root tree")
    parser.add_argument("-f", "--file", type=str, help="Filename", required=True)
    args = parser.parse_args()

    print(f"Loading {args.file}")

    df = pd.read_csv(args.file, comment="#", names=["w", "zProb"])
    print(parse_params(args.file))

    plt.plot(df["w"].to_numpy(), df["zProb"].to_numpy())
    plt.grid(True)
    plt.xlabel(r"$\omega$ [rad/s]")
    plt.ylabel("P(z)")
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
