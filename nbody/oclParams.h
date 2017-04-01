#ifndef OCL_PARAMS_H
#define OCL_PARAMS_H


//CHANGE THE PLATFORM TO USE
#define PLATFORM platforms[0]       // 2 devices - opencl2.0 - cpu + gpu
//#define PLATFORM platforms[1]     // 1 device - opencl2.1 - cpu only
//#define PLATFORM platforms[2]      // 3rd platform :::

//SELECT THE QUEUE
//#define QUEUE queue[0]                      //CPU QUEUE
#define QUEUE queue[0+ret_num_devices_cpu]  //GPU QUEUE


//PATH TO KERNEL SOURCE FILE
char kernel_filename[40] = "D:/bash/project 2017/nbody/kernel.cl";
char kernel_func_name[20] = "nBody_kern";

int MAX_RAND =  360;      //MAXIMUM VALUES FOR RAND
size_t global_size = 1024*28;
size_t local_size = 128;
int size_buffer = global_size;

cl_float dt1 = 0.2f;
cl_float eps = 3e3;





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
cl_kernel       kernel;




//OCL EVENT VARIABLES
cl_event        event_write_buffer[2];
cl_event        event_kernel_run[1];
cl_event        event_read_buffer[2];

cl_mem dev_pos_old, dev_pos_new, dev_vel;
cl_float *h_pos, *h_vel;

#endif // PARAMS_H
