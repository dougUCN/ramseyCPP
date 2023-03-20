#!/usr/bin/env python
import pandas as pd
import re
import argparse
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


def main():
    parser = argparse.ArgumentParser(description="Plots output from rabi.cpp")
    parser.add_argument("-f", "--file", type=str, help="Filename", required=True)
    args = parser.parse_args()

    filename = args.file
    print("Loading...", filename)

    df = pd.read_csv(
        args.file, comment="#", header=0, names=["time", "xProb", "yProb", "zProb"]
    )

    print(parse_params(args.file))

    plt.figure(1)
    plt.plot(df["time"].to_numpy(), df["zProb"].to_numpy())
    plt.xlabel("time [s]")
    plt.ylabel("P(z)")
    plt.ylim([0, 1])

    fig2 = plt.figure(2)
    ax = fig2.add_subplot(111, projection="3d")
    ax.plot(df["xProb"].to_numpy(), df["yProb"].to_numpy(), df["zProb"].to_numpy())
    ax.set_xlim3d(0, 1)
    ax.set_ylim3d(0, 1)
    ax.set_zlim3d(0, 1)
    ax.set_xlabel("P(x)")
    ax.set_ylabel("P(y)")
    ax.set_zlabel("P(z)")
    ax.view_init(30, 220)  # So that the viewing angle looks ok

    plt.figure(3)
    plt.plot(df["time"].to_numpy(), df["zProb"].to_numpy())
    plt.xlabel("time [s]")
    plt.ylabel("P(x)")
    plt.ylim([0, 1])

    plt.figure(4)
    plt.plot(df["time"].to_numpy(), df["yProb"].to_numpy())
    plt.xlabel("time [s]")
    plt.ylabel("P(y)")
    plt.ylim([0, 1])

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
