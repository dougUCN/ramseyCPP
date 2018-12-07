#!/usr/bin/env python

def main():
    from root_numpy import root2array
    import sys
    import argparse
    import matplotlib.pyplot as plt

    parser = argparse.ArgumentParser(description='Plots ramsey fringe from root tree')
    parser.add_argument("-f", "--file", type=str, help = "Filename", required=True)
    args = parser.parse_args()

    filename = args.file
    print("Loading...", filename)

    try:
        w = root2array(filename,branches="freq")[0]
        z = root2array(filename,branches="zProb")[0]
    except:
        sys.exit()

    try:
        params = root2array(filename,branches="params")[0]
        if(params[4] == 1):
            print("Circular RF Ramsey fringe")
        else:
            print("Linear RF Ramsey fringe")
        print("{W0_VAL, WL_VAL, PHI_VAL}")
        print(params[1],"  ",params[2],"  ",params[3])
    except:
        print("Unable to read branch 'params'")

    plt.plot(w, z)
    plt.grid(True)
    plt.xlabel('w [rad/s]')
    plt.ylabel('P(z)')
    plt.show()
    return


if ( __name__ == '__main__' ):
    main()
