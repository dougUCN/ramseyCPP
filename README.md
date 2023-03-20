# ramsey

Makes circular/linear ramsey fringes numerically using RK4 integration.  
Solves the equations of motion of a neutron spin in a magnetic field (Schrodinger equation).  
Calculates bloch siegert shift

Change parameters in the body of the code in /src/, or write your own routines  
using the functions in neutron.hpp and neutron.cpp

## Code

### Sample Code

Executables will be found in /out/  
rabi -- Applies a rabi pulse with a circular and linear RF to a neutron  
ramsey -- Creates a ramsey fringe with circular or linear RF

### Plotting

plotRabi -- Plots a single rabi pulse
plotRamsey -- Plots a ramsey fringe

## Prerequisites

Requirements: cmake, BOOST (only tested 1.58.0)

Python plotting dependencies:

### Boost

The [Boost C++ libraries](https://www.boost.org/) are used for some fitting.  
Boost is included in most Linux OSs; should you need to download and compile it manually,  
you may have to adjust the search path of cmake by calling cmake with `-DBOOST_ROOT=/path/to/boost`.

## Compiling

$ cmake .  
$ make

and then run the executable of your choice in /out/

If you write your own routines and want stuff to compile, edit CMakeLists.txt  
and add add_executable and target_link_libraries flags

## Experiment parameters

Parameters for test run of ramsey fringes at Los Alamos Nat. Lab nEDM search

B_0 = 1 uT  
B_l = 4 nT  
Precession = 180s

gyromagnetic ratio of the neutron (y)  
1.832 471 72 x 10^8 rad s^-1 T^-1

w0 = - 183.247  
wl = - 0.732989

Approximate pulse time for ramsey resonance: 4.286 seconds

## More information

Equations in comments are in reference to this [thesis](https://github.com/dougUCN/thesis)
