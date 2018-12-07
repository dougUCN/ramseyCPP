ramsey
==================
Makes circular/linear ramsey fringes numerically using RK4 integration.
Solves the equations of motion of a neutron spin in a magnetic field (Schrodinger equation)
Calculates bloch siegert shift

Change parameters in the body of the code

Code
-------------
### Sample Code
Executables found in /out/
circRabi -- Applies a rabi pulse with a circular RF to a neutron

### Adding to the library
Basically the important stuff is in neutron.cpp and neutron.h

### Plotting
Code outputs root trees, but since I despise root, I use python for all my plotting
plotRabi -- Plots the output from either circRabi or linRabi

Prerequisites
--------------
Requirements: cmake, [ROOT](https://github.com/root-project/root) (only tested on v6.15)
For pretty plots: Python, [root_numpy](http://scikit-hep.org/root_numpy/)

Compiling
-----------
$ cmake .
$ make

and then run the executable of your choice in /out/


Experiment parameters
------------------------
Parameters for test run of ramsey fringes at Los Alamos Nat. Lab nEDM search

B_0 = 1 uT
B_l = 4 nT
Precession = 180s

gyromagnetic ratio of the neutron (y)
1.832 471 72 x 10^8 rad s^-1 T^-1

w0 = - 183.247
wl = - 0.732989

Approximate pulse time for ramsey resonance: 4.286 seconds

More information
--------------------
Many of my comments refer to Daniel May's 1999 Thesis on the ILL nEDM
https://inis.iaea.org/search/searchsinglerecord.aspx?recordsFor=SingleRecord&RN=30045807

If you are having difficulty finding this thesis, email me for a copy
