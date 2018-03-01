# Discrete-Particle-Simulation
Discrete Paarticle simulation using OpenCL and OpenGL &lt;freeglut>



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


 N – Body Simulation

 Smooth Particles Hydrodynamics
