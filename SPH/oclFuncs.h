#ifndef OCL_FUNCS_H
#define OCL_FUNCS_H

#include<CL/cl.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<random>
#include<time.h>
#include<math.h>
#include"oclParams.h"

/***
CRREATES A RANDOM FLOATING POINT NUMBER
USING 2 RANDOM TIME SEEDED VALUES  FOR THE REAL AND FRACTIONAL PARTS
***/

float randomFloat(int MAX_RAND){
    float integer, fraction, value;
    integer = rand()% MAX_RAND;
    fraction = rand() % MAX_RAND;
    value = integer + fraction/MAX_RAND;
    return value;
}
float initPosCirclex(int radius, int MAX_RAND){
    float x = radius * sin(rand()%MAX_RAND);
    std::cout<<x<<std::endl;
    return x;
}
float initPosCircley(int radius, int MAX_RAND){
    float y = radius * cos(rand()%MAX_RAND);
    std::cout<<y<<std::endl;
    return y;
}
/***        char* loadKernelSource(char * filename, char* source_string)

LOADS THE KERNEL SOURCE CODE FROM A FILE WHOSE NAME IS GIVEN AS ARGUMENT < < char* filename> >
RETURNS < < char* > > WHICH IS THE POINTER TO THE KERNEL SOURCE IN A CAHRACTER STRING
***/
char* loadSourceCode(char* filename){
    char* source;
    long s_size, r_size;
    FILE* fp;
    fp = fopen(filename, "r");
    if(!fp){
        std::cerr<<"Failed to open Kernel Source file!";
    }
    fseek(fp, 0L, SEEK_END);
    s_size = ftell(fp);
    rewind(fp);

    source = (char*)malloc(s_size + 1);
    r_size = fread(source, 1, s_size, fp);

    source[r_size] = '\0';
    fclose(fp);
    return source;
}
/***
PASS THIS FUNCTION < < queryDevices() > > THE LIST OF DEVICES < < cl_device_id * > >
YOU MAY ALSO NEED TO CHANGE THE MACRO < < num_devices > >
***/
//  QUERY DEVICE INFO < < JUST FOR FUN & **LATER OPTIMIZATIONS** > >
void queryDevices(cl_device_id* devices, cl_uint num_devices){

    cl_uint max_clk_freq;
    cl_uint num_comp_unit;
    cl_ulong sizeof_global_mem;
    cl_ulong sizeof_global_mem_cache;
    cl_ulong sizeof_local_mem;
    size_t max_work_group_size;
    size_t max_work_item_size;
    char device_name[50];
    char device_version[30];
    char driver_version[30];

    for(int i=0; i<num_devices; i++){
    clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(device_name), &device_name, NULL );
    clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, sizeof(device_version), &device_version, NULL );
    clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, sizeof(driver_version), &driver_version, NULL );
    clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(num_comp_unit), &num_comp_unit, NULL );
    clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(max_clk_freq), &max_clk_freq, NULL );
    clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(sizeof_global_mem), &sizeof_global_mem, NULL );
    clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(sizeof_global_mem_cache), &sizeof_global_mem_cache, NULL );
    clGetDeviceInfo(devices[i], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(sizeof_local_mem), &sizeof_local_mem, NULL );
    clGetDeviceInfo(devices[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(max_work_group_size), &max_work_group_size, NULL );
    //clGetDeviceInfo(devices[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(max_work_item_size), &max_work_item_size, NULL );

    //PRINT OUT THE RESULTS
    std::cout<<"Device Name: \t\t"<<device_name<<std::endl;
    std::cout<<"Device Version: \t"<<device_name<<std::endl;
    std::cout<<"Driver Version: \t"<<driver_version<<std::endl;
    std::cout<<"Maximum Clock Frequency: "<<max_clk_freq<<std::endl;
    std::cout<<"Num of Compute Units: \t"<<num_comp_unit<<std::endl;
    std::cout<<"Global Mem Size: \t"<<sizeof_global_mem/(1024*1024)<<" MB"<<std::endl;
    std::cout<<"Global Mem Cache Size: \t"<<sizeof_global_mem_cache/1024<<" KB"<<std::endl;
    std::cout<<"Local Mem Size: \t"<<sizeof_local_mem/1024<<" KB"<<std::endl;
    std::cout<<"Max WorkGroup Size: \t"<<max_work_group_size<<std::endl;
    //std::cout<<"Max WorkItem Size: \t"<<max_work_item_size<<std::endl;

    std::cout<<std::endl<<std::endl;
    }
}
void queryKernel(cl_kernel kernel){
    cl_int err;
    char func_name[30];
    cl_uint num_args;
    cl_uint reference_count;

    err = clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME, sizeof(func_name), &func_name, NULL);
        if(err!=0){std::cout<<"clGetKernelInfo<FUNCTION_NAME> Error! Error Code: "<<err<<std::endl;}

    err = clGetKernelInfo(kernel, CL_KERNEL_NUM_ARGS, sizeof(num_args), &num_args, NULL);
        if(err!=0){std::cout<<"clGetKernelInfo<NUM_ARGS> Error! Error Code: "<<err<<std::endl;}

    err = clGetKernelInfo(kernel, CL_KERNEL_REFERENCE_COUNT, sizeof(reference_count), &reference_count, NULL);
        if(err!=0){std::cout<<"clGetKernelInfo<REFERENCE_COUNT> Error! Error Code: "<<err<<std::endl;}


    std::cout<<"Function Name: \t\t\t"<<func_name<<std::endl;
    std::cout<<"Number of Arguments: \t\t"<<num_args<<std::endl;
    std::cout<<"Number of Reference Counts: \t"<<reference_count<<std::endl;
}
void printArray(float* A, int size_buffer, char* text = "", char* filepath = "C:/Users/arpyt/Desktop/SPH/output.txt"){
    std::fstream fout(filepath, std::ios_base::app);
    if(!fout.is_open()){
        std::cerr<<"unable to open output file"<<std::endl;
    }
    fout<<text<<"\n";
    for(int i=0; i<size_buffer; i++){
        fout<<A[i]<<"\t\t";
        if((i+1) % 4 == 0){fout<<"\n";}
    }
    fout.close();
}
/***
THIS FUNCTION PRINTS THE TOTAL COMPLETION TIME OF AN EVENT
***/

void profileEvent(cl_event* event, cl_uint num_events){
    cl_int err;
    cl_ulong time_start, time_end;
    double total_time = 0.0;

    for(int i=0; i<num_events; i++){
        err = clGetEventProfilingInfo(event[i], CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
            if(err!=0){std::cout<<"clGetEventProfilingInfo<COMMAND_START> Error! Error Code: "<<err<<std::endl;}

        err = clGetEventProfilingInfo(event[i], CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
            if(err!=0){std::cout<<"clGetEventProfilingInfo<COMMAND_END> Error! Error Code: "<<err<<std::endl;}

        total_time += (time_end - time_start);
    }
    std::cout<<"Total time: "<<(total_time/1000.0)<<" microsecond."<<std::endl;
}


void queryPlatform(cl_platform_id* platform, cl_uint num_platform){
    char platform_profile[50];
    char platform_version[30];
    char platform_name[30];
    char platform_vendor[30];
    char platform_extensions[4000];

    for(int i=0; i<num_platform; i++){

    clGetPlatformInfo(platform[i], CL_PLATFORM_PROFILE, sizeof(platform_profile), &platform_profile, NULL);
    clGetPlatformInfo(platform[i], CL_PLATFORM_VERSION, sizeof(platform_version), &platform_version, NULL);
    clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, sizeof(platform_name), &platform_name, NULL);
    clGetPlatformInfo(platform[i], CL_PLATFORM_VENDOR, sizeof(platform_vendor), &platform_vendor, NULL);
    clGetPlatformInfo(platform[i], CL_PLATFORM_EXTENSIONS, sizeof(platform_extensions), &platform_extensions, NULL);


    //PRINT OUT THE RESULTS
    std::cout<<"Platform Profile: \t\t"<<platform_profile<<std::endl;
    std::cout<<"Platform Version: \t"<<platform_version<<std::endl;
    std::cout<<"Platform Name: \t"<<platform_name<<std::endl;
    std::cout<<"Platform Vendor: "<<platform_vendor<<std::endl;
    std::cout<<"Platform Extensions: \t"<<platform_extensions<<std::endl;

    std::cout<<std::endl<<std::endl;
    }
}
void initOcl(){
    int i;
    //  CREATE PLATFORM
    err = clGetPlatformIDs(0, NULL, &ret_num_platforms); //get no of available platforms
        if(err!=0){std::cout<<"clGetPlatformIDs Error! Error Code: "<<err<<std::endl;}

    platforms = new cl_platform_id[ret_num_platforms];

    err = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
        if(err!=0){std::cout<<"clGetPlatformIDs Error! Error Code: "<<err<<std::endl;}


    //  CREATE DEVICES FOR THE SELECTED PLATFORM <select platform from 'params.h'>
    //get number of devices of each type in the selected platform
    //  <<<THE CPUs ARE FILLED IN FIRST>>>

    err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_ALL, NULL, NULL, &ret_num_devices_all);
        if(err!=0){std::cout<<"clGetDeviceIDs Error! Error Code: "<<err<<std::endl;}
    err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_CPU, NULL, NULL, &ret_num_devices_cpu);
        if(err!=0){std::cout<<"clGetDeviceIDs Error! Error Code: "<<err<<std::endl;}
    err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_GPU, NULL, NULL, &ret_num_devices_gpu);
        if(err!=0){std::cout<<"clGetDeviceIDs Error! Error Code: "<<err<<std::endl;}

    devices = new cl_device_id[ret_num_devices_all];

    if(ret_num_devices_cpu>0){
        err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_CPU, ret_num_devices_cpu, &devices[0], NULL);
            if(err!=0){std::cout<<"clGetDeviceIDs Error! Error Code: "<<err<<std::endl;}
    }
    if(ret_num_devices_gpu>0){
        err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_GPU, ret_num_devices_gpu, &devices[0+ret_num_devices_cpu], NULL);
            if(err!=0){std::cout<<"clGetDeviceIDs Error! Error Code: "<<err<<std::endl;}
    }

    //queryDevices(devices, ret_num_devices_all);


//  CREATE CONTEXT USING DEVICES
    context = clCreateContext(NULL, ret_num_devices_all, devices, NULL, NULL, &err);
        if(err!=0){std::cout<<"clCreateContext Error!  Error Code: "<<err<<std::endl;}


//  CREATE COMMAND QUEUES FOR EACH DEVICE
    queue = new cl_command_queue[ret_num_devices_all];
    for(i=0; i<ret_num_devices_all; i++){
        queue[i] = clCreateCommandQueue(context, devices[i], CL_QUEUE_PROFILING_ENABLE, &err);
            if(err!=0){std::cout<<"clCreateCommandQueue Error!  Error Code: "<<err<<" FOR DEVICE: "<<i<<std::endl;}
    }

/***
CREATE PROGRAM      ==>     BUILD PROGRAM       ==>     BUILD KERNELS       ==>      SET KERNEL PARAMETERS
***/

//  GET KERNEL SOURCE IN A < < CHAR* > >
    source = loadSourceCode(source_filename);


//  CREATE PROGRAM WITH SOURCE CODE
    program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, &err);
        if(err!=0){std::cout<<"clProgramWithSource Error!  Error Code: "<<err<<std::endl;}


//  BUILD THE PROGRAM
// if the build is not successful, DISPLAY THE KERNEL SOURCE BUILD LOG

    err = clBuildProgram(program, 0, NULL,NULL, NULL, NULL);
    if (err != 0) {
        std::cout<<"Error Building the Program! Error Code: "<<err<<std::endl;
        size_t log_size;
        for(i=0; i<ret_num_devices_all; i++){
            // Determine the size of the log
            clGetProgramBuildInfo(program, devices[i], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
            char *log = (char *) malloc(log_size);

            // Get the log
            clGetProgramBuildInfo(program, devices[i], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
            std::cout<<log<<std::endl;
        }
    }


//  CREATE THE KERNEL
    sph_kernel = clCreateKernel(program, "SPH_kernel", &err);
        if(err!=0){std::cout<<"clCreateKernel Error! Error Code: "<<err<<std::endl;}


//  CREATE BUFFERS IN HOST MEMORY
   h_position = new cl_float[4*size_buffer];
   h_velocity = new cl_float[4*size_buffer];


   for(i=0; i<size_buffer; i++){
        h_position[i*4 + 0] = randomFloat(25);
        h_position[i*4 + 1] = randomFloat(50);
        h_position[i*4 + 2] = randomFloat(100);
        h_position[i*4 + 3] = 0.0f;

//        h_velocity[i*4 + 0] = 0.0f;
//        h_velocity[i*4 + 1] = 0.0f;
//        h_velocity[i*4 + 2] = 0.0f;
//        h_velocity[i*4 + 3] = 0.0f;

        h_velocity[i*4 + 0] = randomFloat(50);
        h_velocity[i*4 + 1] = 0.0f;
        h_velocity[i*4 + 2] = 0.0f;
        h_velocity[i*4 + 3] = 0.0f;
   }


//  CREATE MEMORY OBJECTS IN DEVICE MEMORY

    //write nothing to none of these buffers, just create these
    dev_position = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float4)*size_buffer, NULL, &err);
        if(err!=0){std::cout<<"clCreateBuffer Error! Error Code: "<<err<<std::endl;}
    dev_velocity_full = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float4)*size_buffer, NULL, &err);
        if(err!=0){std::cout<<"clCreateBuffer Error! Error Code: "<<err<<std::endl;}
    dev_velocity_half = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float4)*size_buffer, NULL, &err);
        if(err!=0){std::cout<<"clCreateBuffer Error! Error Code: "<<err<<std::endl;}
    dev_acceleration = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float4)*size_buffer, NULL, &err);
        if(err!=0){std::cout<<"clCreateBuffer Error! Error Code: "<<err<<std::endl;}
    dev_density = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float)*size_buffer, NULL, &err);
        if(err!=0){std::cout<<"clCreateBuffer Error! Error Code: "<<err<<std::endl;}


//  S E T   s p h - K E R N E L     A R G U M E N T S


    err = clSetKernelArg(sph_kernel, 0, sizeof(cl_float), &dt);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 1, sizeof(cl_float), &eps);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 2, sizeof(cl_float), &restitution);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 3, sizeof(cl_float), &ConstantDensitySumTerm);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 4, sizeof(cl_float), &ConstantDensityKernelTerm);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 5, sizeof(cl_float), &H2);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 6, sizeof(cl_float), &ReferenceDensity);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 7, sizeof(cl_float), &InteractionRadius);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 8, sizeof(cl_float), &C0);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 9, sizeof(cl_float), &CP);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 10, sizeof(cl_float), &CV);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 11, sizeof(cl_float), &xMin);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 12, sizeof(cl_float), &xMax);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 13, sizeof(cl_float), &yMin);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 14, sizeof(cl_float), &yMax);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 15, sizeof(cl_float), &zMin);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 16, sizeof(cl_float), &zMax);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}

    err = clSetKernelArg(sph_kernel, 17, sizeof(cl_mem), (void*)&dev_position);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 18, sizeof(cl_mem), (void*)&dev_velocity_half);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 19, sizeof(cl_mem), (void*)&dev_velocity_full);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 20, sizeof(cl_mem), (void*)&dev_acceleration);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(sph_kernel, 21, sizeof(cl_mem), (void*)&dev_density);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}

    err = clSetKernelArg(sph_kernel, 22, sizeof(cl_float4)*local_size, NULL);
        if(err!=0){std::cout<<"clSetKernelArg<> Error! Error Code: "<<err<<std::endl;}



    //  UPDATE THE VELOCITY BUFFER
    err = clEnqueueWriteBuffer(QUEUE, dev_velocity_full, CL_TRUE, 0, sizeof(cl_float4)*size_buffer, h_velocity, 0, NULL, &event_write_buffer[0]);
        if(err!=0){std::cout<<"clEnqueueWriteBuffer Error! Error Code: "<<err<<std::endl;}

}


void updatePos(){


//  INITIALIZE BUFFERS IN DEVICE GLOBAL MEMORY
    err = clEnqueueWriteBuffer(QUEUE, dev_position, CL_TRUE, 0, sizeof(cl_float4)*size_buffer, h_position, 0, NULL, &event_write_buffer[1]);
        if(err!=0){std::cout<<"clEnqueueWriteBuffer Error! Error Code: "<<err<<std::endl;}



//  ENQUEUE TASKS = or =  ENQUEUE KERNEL
    err = clEnqueueNDRangeKernel(QUEUE, sph_kernel, 1, NULL, &global_size, &local_size, 2, event_write_buffer, &event_kernel_run[0]);
        if(err!=0){std::cout<<"clEnqueueTask Error! Error Code: "<<err<<std::endl;}


    // wait for kernel to finish
    err = clWaitForEvents(1, &event_kernel_run[0]);
        if(err!=0){std::cout<<"clWaitForEvents Error! Error Code: "<<err<<std::endl;}




// READ updated POSITIONS

    err = clEnqueueReadBuffer(QUEUE, dev_position, CL_TRUE, 0, sizeof(cl_float4)*size_buffer, h_position, 0, NULL, &event_read_buffer[0]);
        if(err!=0){std::cout<<"clEnqueueReadBuffer Error! Error Code: "<<err<<std::endl;}


// WAIT for READ to finish
    err = clWaitForEvents(1, event_read_buffer);
        if(err!=0){std::cout<<"clWaitForEvents Error! Error Code: "<<err<<std::endl;}

        profileEvent(event_kernel_run, 1);
}

#endif
