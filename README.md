# Discrete-Particle-Simulation
Discrete Particles simulation using OpenCL and OpenGL &lt;freeglut>

Discrete particle methods are relatively computationally intensive, which
limits either the duration of a simulation or the number of particles. Several
Discrete Particle methods, take advantage of parallel processing capabilities to
scale up the number of particles and/or length of the simulation.

The main aim of the project is to demonstrate that computationally intensive
problems that require huge computational power of a lots of CPUs can be
mapped to readily available cheap commodity hardware such as a GPU or an
Accelerator.

The performance scaling that can be achieved over by using just a CPU VS
using a much cheaper vector processor like a GPU could be as much as 100X
or even more, depending upon how efficiently we utilize the system resources.
In our simple demonstration, we achieved performance scaling of up to 10X
on an embedded mobile GPU Intel HD 5500, and up to 25X on a discrete mobile
GPU NVIDIA GeForce GTX 860M.


# N – Body Simulation
An N-body Simulation is a simulation of a dynamic system of particles
usually under the influence of physical forces such as gravity or electrostatic
forces.

![N-Body Simulation Image](https://github.com/arpytanshu/Discrete-Particle-Simulation/blob/master/images/nbody-simulation-image.png)

The Nbody Kernel illustrates the working of the discrete particle simulation model used, as it is pretty simple and intuitive to understand.

![N-Body Kernel DFD](https://github.com/arpytanshu/Discrete-Particle-Simulation/blob/master/images/nbody-kernel.png)


# Smooth Particles Hydrodynamics
Smooth Particle Hydrodynamics (SPH) is a computational method which is
used for simulating the dynamics of continuum medium, such as fluid flow.
The method was developed by Gingold & Monaghan [1977] and Lucy [1977]
initially for astrophysical problems. 

![SPH Simulation Image](https://github.com/arpytanshu/Discrete-Particle-Simulation/blob/master/images/sph-simulation-image.png)

The SPH Kernel is divided into 3 modules.
The first named Acceleration_Calc() is responsible for computing the
density, followed by the acceleration. The second module, named
LeapFrog_Integrator() integrates the result of the first module into
velocities and positions vectors. The third module, named
Boundary_Reflect(), performes a reflection operation, in which, if a
particle, if tries to escape beyond the boundaries of the container or of
an obstacle, is reflected back with a damped velocity.

![SPH Kernel DFD](https://github.com/arpytanshu/Discrete-Particle-Simulation/blob/master/images/sph-kernel.png)

# System DFD

The system has 2 main Modules namely OCL & OGL. How these modules
bring data back & forth between the Host & Device momory is described in the
following System DFD.
The OCL module first initializes the Device Memory by bringing in initialized
buffers from the Host Memory. It then initiates the Kernel to run on the
OpenCL device and compute upon it. The results are then brought back to the
Host Memory by the module which is used for rendering.
![System DFD](https://github.com/arpytanshu/Discrete-Particle-Simulation/blob/master/images/System%20DFD.png)

# Performance Metrics
Here we present the performance comparison of our simulation when running
in real time over our test devices:
1. Intel HD 5500 with 24 Execution Units
2. Nvidia GeForce GTX 860M with 640 Execution Units
3. Intel Core i5 5200U with 4 Execution Units


Performance of the N-Body Kernel on test devices:

![N-Body Kernel Performance](https://github.com/arpytanshu/Discrete-Particle-Simulation/blob/master/images/Nbody%20Test%20Result.png)

Performance of the SPH Kernel on test devices:

![SPH Kernel Performance](https://github.com/arpytanshu/Discrete-Particle-Simulation/blob/master/images/SPH%20Test%20result.png)

