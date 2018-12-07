ramsey
==================
Makes circular/linear ramsey fringes numerically using RK4 integration.  
Solves the equations of motion of a neutron spin in a magnetic field (Schrodinger equation).  
Calculates bloch siegert shift  

Change parameters in the body of the code in /src/, or write your own routines  
using the functions in neutron.h and neutron.cpp

Code
-------------
### Sample Code
Executables found in /out/  
rabi -- Applies a rabi pulse with a circular and linear RF to a neutron
ramsey -- Creates a ramsey fringe with circular or linear RF 

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

If you write your own routines and want stuff to compile, edit CMakeLists.txt  
and add add_executable and target_link_libraries flags

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
