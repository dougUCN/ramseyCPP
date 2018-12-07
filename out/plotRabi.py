#!/usr/bin/env python

def main():
    from root_numpy import root2array
    import sys
    import argparse

    parser = argparse.ArgumentParser(description='Plots output from rabi.cpp')
    parser.add_argument("-f", "--file", type=str, help = "Filename", required=True)
    args = parser.parse_args()

    filename = args.file
    print("Loading...", filename)

    try:
        time = root2array(filename,branches="time")[0]
        x = root2array(filename,branches="xProb")[0]
        y = root2array(filename,branches="yProb")[0]
        z = root2array(filename,branches="zProb")[0]
        params = root2array(filename,branches="params")[0]
    except:
        sys.exit()

    if(params[4] == 1):
        print("Circular RF Rabi pulse")
    else:
        print("Linear RF Rabi pulse")
    print("{W_VAL, W0_VAL, WL_VAL, PHI_VAL}")
    print(params[0],"  ",params[1],"  ",params[2],"  ",params[3])

    plotStuff(x,y,z,time)
    return

def plotStuff(xProb, yProb, zProb, time):
    import numpy as np
    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D

    plt.figure(1)
    plt.plot(time, zProb)
    plt.title('Odds of measuring spin up along z')
    plt.xlabel('time [s]')
    plt.ylabel('P(z)')
    plt.axis([0,2*np.pi,0,1])

    fig2 = plt.figure(2)
    ax = fig2.add_subplot(111, projection='3d')
    plt.title('Odds of measuring spin up along x, y, z')
    ax.scatter(xProb, yProb, zProb)
    ax.set_xlim3d(0,1)
    ax.set_ylim3d(0,1)
    ax.set_zlim3d(0,1)
    ax.set_xlabel('P(x)')
    ax.set_ylabel('P(y)')
    ax.set_zlabel('P(z)')
    ax.view_init(30,220) # So that the viewing angle looks ok

    plt.figure(3)
    plt.plot(time, xProb)
    plt.title('Odds of measuring spin up along x')
    plt.xlabel('time [s]')
    plt.ylabel('P(x)')
    plt.axis([0,2*np.pi,0,1])

    plt.figure(4)
    plt.plot(time, yProb)
    plt.title('Odds of measuring spin up along y')
    plt.xlabel('time [s]')
    plt.ylabel('P(y)')
    plt.axis([0,2*np.pi,0,1])

    plt.show()

    return

if ( __name__ == '__main__' ):
    main()
