# XOA_QuantumDynamicsEngine
This is the repository to gather a self-motivated open source Quantum Dynamics Simulator software written in C++, for research and educational purposes.

## Installation of external dependancies
--------------------

Execute the INSTALL_DEPENANCIES.sh bash script using:

        bash INSTALL_DEPENANCIES.sh
        
The script basically executes the following three steps:

    1) g++ is installed using apt-get assuming the user has Debian Linux. This requires admin priviledge so pwd will be asked.
    
    2) Gnuplot is installed using apt-get assuming the user has Debian Linux.
    
    3) Eigen 3.3.7 is downloaded and extracted. The header files are added to usr/local/include such that g++ does not need the -I flag in the compilation.
