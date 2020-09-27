#!/bin/bash

:'
This script prepares the things to allow the execution of the MAIN bash scripts in the Linux XOA_QuantumDynamicsEngine and the ResearchLab. In particular, the following three things are carried:

    1) g++ is installed using apt-get assuming the user has Debian Linux. THis requires admin priviledge so pwd will be asked
    
    2) Gnuplot is installed using apt-get assuming the user has Debian Linux
    
    3) Eigen 3.3.7 is downloaded and extracted. The header files are added to usr/local/include such that g++ does not need the -I flag in the compilation
    
'

# Install g++

sudo apt-get install g++

# Install gnuplot

apt-get install gnuplot
apt-get install gnuplot-x11

# Make a temporary installation directory

mkdir tmp
cd tmp

# Download Eigen3

wget https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.tar.gz

# Untar Eigen

tar -zxvf eigen-3.3.7.tar.gz

# Isolate the header files -the proper library we are interested on-

cd eigen-3.3.7

mkdir ../eigen3

cp -r ./Eigen/ ../eigen3/Eigen/

mkdir ../eigen3/unsupported

cp -r ./unsupported/Eigen/ ../eigen3/unsupported/Eigen/

cd ..

# Add the Eigen headers to usr/local/include such that no -I flag is required in g++ compilations

sudo cp -r eigen3/ /usr/local/include/eigen3/

# clean stuff

cd ..
rm -r tmp
