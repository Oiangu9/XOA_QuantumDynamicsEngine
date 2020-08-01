#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from random import choices

# PLOT SPECKLE POTENTIAL---------------------------------------
# Probability Density Function

VR=2.4
Vmin=-VR-2.0
Vmax=4*VR
nV=3000
dV=(Vmax-Vmin)/nV

def speckleProbablityDensity(V, VR): # the probability to have a potential x at a given point of space
    if(V/VR +1 >=0):
        return (1/(np.e*VR)*np.exp(-V/VR))
    else:
        return 0

posibleV = [Vmin+i*dV for i in range(nV+1)]
probabDensity = [speckleProbablityDensity(i, VR) for i in posibleV]

'''
plt.plot(posibleV, probabDensity)
plt.xlabel("posibleV")
plt.ylabel("Probability")
plt.show()
'''

# R -> R PLOT

xmax=7.0
xmin=-7.0
nx=30
dx=(xmax-xmin)/nx

posx = [xmin+i*dx for i in range(nx+1)]
potential=choices(posibleV, probabDensity, k=nx+1)

plt.plot(posx, potential)
plt.xlabel("x")
plt.ylabel("V(x)")
plt.show()

#calculate the mean value of the potential
print(np.mean(potential))
print(np.mean(np.abs(potential)))
print((np.mean(np.square(potential)))**0.5)

# PLOT INCOMMENSURATE POTENTIAL------------------------------------------

def bichromicLatticePot(x):
    k2=4*(np.sqrt(5)-1)
    k1=8
    s1=1.0
    s2=s1/5
    return s1*(np.cos(x*k1))**2 + s2*(np.cos(x*k2))**2

xmin=-32
xmax=32
nx=6000
dx=(xmax-xmin)/nx
posx=[xmin+i*dx for i in range(nx+1)]
potential=[bichromicLatticePot(i) for i in posx]


plt.plot(posx, potential)
plt.xlabel("x")
plt.ylabel("V(x)")
plt.show()


# PLOT DOUBLE SLIT POTENTIAL-------------------------------------------------
# R2 -> R PLOT

q0min=-7.0
q0max=7.0
q1min=-7.0
q1max=7.0
nq0=400
nq1=400
dq0=(q0max-q0min)/nq0
dq1=(q1max-q1min)/nq1

def potkGaussian(q0,q1):
    
    sigma0=0.5 #monticulo central en q1
    sigma1=0.5 # monticulos laterales en q1
    sigma2=sigma1
    sigma3=0.5 # el grueso de la barrera en q0
    mu1=3
    mu2=-3
    centralHdiv=4.0
    E0=1/(sigma1*np.sqrt(2*np.pi))
    V0=45.0 #if 45, the wall gets a height of 7
    
    # return V0/(2*np.pi*sigma3)*np.exp(-0.5*q0**2/sigma3**2)*(np.exp(-0.5*q1**2/sigma0**2)/(sigma0*centralHdiv) + np.exp(-0.5*((q1-mu1)/sigma1)**2)/sigma1 + np.exp(-0.5*((q1-mu2)/sigma2)**2)/sigma2)
    return V0/(np.sqrt(2*np.pi)*sigma3)*np.exp(-0.5*q0**2/sigma3**2)*(E0-1/(np.sqrt(2*np.pi))*(np.exp(-0.5*((q1-mu1)/sigma1)**2)/sigma1 + np.exp(-0.5*((q1-mu2)/sigma2)**2)/sigma2))

grid_q0 = np.array([i*(q0max-q0min)/(nq0-1)+q0min for i in range(nq0)])
grid_q1 = np.array([i*(q1max-q1min)/(nq1-1)+q1min for i in range(nq1)])

potential = np.array([[potkGaussian(j,i) for j in grid_q1] for i in grid_q0])

mesh1, mesh2 = np.meshgrid(grid_q0, grid_q1)

fig=plt.figure()
ax = fig.add_subplot(1,1,1, projection='3d')
ax.set_xlabel('x1')
ax.set_ylabel('x2')
ax.set_xlim(q0min,q0max)
ax.set_ylim(q1min,q1max)
surf = ax.plot_surface(mesh1, mesh2, potential, cmap=plt.cm.coolwarm, linewidth=0, antialiased=True)
ax.set_xlabel('q0')
ax.set_ylabel('q1')
ax.set_zlabel('Potential(q0,q1)')
# x.view_init(azim=24)
plt.show()

