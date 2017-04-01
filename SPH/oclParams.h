#ifndef OCL_PARAMS_H
#define OCL_PARAMS_H


//CHANGE THE PLATFORM TO USE
#define PLATFORM platforms[0]       // 2 devices - opencl2.0 - cpu + gpu
//#define PLATFORM platforms[1]     // 1 device - opencl2.1 - cpu only
//#define PLATFORM platforms[2]      // 3rd platform :::

//SELECT THE QUEUE
//#define QUEUE queue[0]                      //CPU QUEUE
#define QUEUE queue[0+ret_num_devices_cpu]  //GPU QUEUE

#define PI 3.14

//PATH TO KERNEL SOURCE FILE
char source_filename[40] = "D:/bash/project 2017/SPH/newKernel.cl";


int MAX_RAND =  50;      //MAXIMUM VALUES FOR RAND
size_t global_size = 1024*24;
size_t local_size = 128;
int size_buffer = global_size;


//OCL VARIABLES
cl_int          err;      //FOR CAPTURING THE ERROR CODES
cl_uint         ret_num_platforms;
cl_platform_id* platforms;
cl_uint         ret_num_devices_all;
cl_uint         ret_num_devices_cpu;
cl_uint         ret_num_devices_gpu;
cl_device_id*   devices;
cl_context      context;
cl_command_queue* queue;
char*           source;
cl_program      program;

//kernels
cl_kernel sph_kernel;

//  EVENTS
cl_event event_write_buffer[2];
cl_event event_read_buffer[1];
cl_event event_kernel_run[1];

//MEMORY OBJECTS



//equations constants
cl_float dt = 0.3;
cl_float eps;
cl_float restitution = 0.65f;
cl_float H2 = 0;

cl_float ParticleMass = 0.1;
cl_float H8 = H2 * H2 * H2;

//cl_float ConstantDensitySumTerm;
//cl_float ConstantDensityKernelTerm;

cl_float ConstantDensitySumTerm  = 4 * ((ParticleMass / PI) / H2);
cl_float ConstantDensityKernelTerm = 4 * ((ParticleMass / PI) / H8);
cl_float ReferenceDensity = 998;
cl_float InteractionRadius = 50;
cl_float C0 = 0.0;
cl_float CP = 10.0;
cl_float CV = 3.5;
cl_float xMin = 0;
cl_float xMax = 200;
cl_float yMin = 0;
cl_float yMax = 100;
cl_float zMin = 0;
cl_float zMax = 100;



// memory objects that must be initialized at first
cl_float* h_position;          //size = global_size*4
cl_float* h_velocity;


// memory objects that we require in the global memory
cl_mem dev_position; //cl_float4
cl_mem dev_velocity_full;
cl_mem dev_velocity_half;
cl_mem dev_acceleration;
cl_mem dev_density; //cl_float


#endif // PARAMS_H
