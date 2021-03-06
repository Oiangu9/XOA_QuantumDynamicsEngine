# XOA_QuantumDynamicsEngine
This is the repository to gather a self-motivated open source Quantum Dynamics Simulator software written in C++, for research and educational purposes.

## Installation of External Dependancies for Linux Machines
--------------------

In order to use the Linux software or the ResearchLab it is necessary to install: g++, Eigen3 and gnuplot.

The installed version of Eigen3 should be at least 3.3.7.
The installed verion of g++ should be at least 7.5 to support the complex number libraies used.
The installed version of gnuplot should be at least 5.0 to support special for loops used, and should have "terminal gif" installed.

You can fast forward this by executing the INSTALL_DEPENANCIES.sh bash script using:

        bash INSTALL_DEPENANCIES.sh
        
The script basically executes the following three steps:

  **1)** g++ is installed using apt-get assuming the user has Debian Linux. This requires admin priviledge so pwd will be asked.
    
  **2)** Gnuplot is installed using apt-get assuming the user has Debian Linux.
    
  **3)** Eigen 3.3.7 is downloaded and extracted. The header files are added to usr/local/include such that g++ does not need the -I flag in the compilation. Here a directory called eigen3 is created where a subdirectory Eigen will contain the library headers.

## General To Do-s:
-------------------

- The Linux Graphical User Interface is still work in progress.

- Nothing has been implemented for Windows yet.
