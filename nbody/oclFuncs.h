#ifndef OCL_COMMONFUNCS_H
#define OCL_COMMONFUNCS_H


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
char* loadKernelSource(char* filename){
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

void printArray(float* A, int size_buffer, char*text = "", char* filename = "output.txt"){
    std::fstream fout(filename, std::ios_base::app);
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
void profileEvent(cl_event* event, cl_uint num_events, char* text = ""){
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
    std::cout<<"Total"<<text<<" time: "<<(total_time/1000.0)<<" microsecond."<<std::endl; //uncomment to display it in console
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
    if(ret_num_devices_cpu>0){
        err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_GPU, ret_num_devices_cpu, &devices[0+ret_num_devices_cpu], NULL);
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
    source = loadKernelSource(kernel_filename);


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
    kernel = clCreateKernel(program, kernel_func_name, &err);
        if(err!=0){std::cout<<"clCreateKernel Error! Error Code: "<<err<<std::endl;}




//  CREATE BUFFERS IN HOST MEMORY

   h_pos = new cl_float[size_buffer * sizeof(cl_float4)];
   h_vel = new cl_float[size_buffer * sizeof(cl_float4)];

   //initialize the position and velocity vectors
   for(i=0; i<size_buffer; i++){
        float theta = randomFloat(180);
        float phi = randomFloat(90);
        h_pos[i*4 + 0] = 100 * cos(theta) * cos(phi);
        h_pos[i*4 + 1] = 100 * cos(phi) * sin(theta);
        h_pos[i*4 + 2] = 100 * sin(phi);
        h_pos[i*4 + 3] = 1.0f;
   }
   for(i=0; i<size_buffer; i++){
        float phi = randomFloat(359);
        h_vel[i*4 + 0] = 0.01 * sin(phi);
        h_vel[i*4 + 1] = 0.01 * sin(phi) ;
        h_vel[i*4 + 2] = 0.01 * sin(phi) ;
        h_vel[i*4 + 3] = 1.0;

   }


//  CREATE MEMORY OBJECTS IN DEVICE MEMORY

    //write nothing to none of these buffers, just create these
    dev_pos_old = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float4)*size_buffer, NULL, &err);
        if(err!=0){std::cout<<"clCreateBuffer Error! Error Code: "<<err<<std::endl;}
    dev_vel = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float4)*size_buffer, NULL, &err);
        if(err!=0){std::cout<<"clCreateBuffer Error! Error Code: "<<err<<std::endl;}
    dev_pos_new = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float4)*size_buffer, NULL, &err);
        if(err!=0){std::cout<<"clCreateBuffer Error! Error Code: "<<err<<std::endl;}


//  SET KERNEL PARAMETERS
    err = clSetKernelArg(kernel, 0, sizeof(cl_float), &dt1);
        if(err!=0){std::cout<<"clSetKernelArg Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(kernel, 1, sizeof(cl_float), &eps);
        if(err!=0){std::cout<<"clSetKernelArg Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&dev_pos_old);
        if(err!=0){std::cout<<"clSetKernelArg Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&dev_pos_new);
        if(err!=0){std::cout<<"clSetKernelArg Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void*)&dev_vel);
        if(err!=0){std::cout<<"clSetKernelArg Error! Error Code: "<<err<<std::endl;}
    err = clSetKernelArg(kernel, 5, sizeof(cl_float4)*local_size*2, NULL);        //local mem allocation <last param = NULL>
        if(err!=0){std::cout<<"clSetKernelArg Error! Error Code: "<<err<<std::endl;}



    err = clEnqueueWriteBuffer(QUEUE, dev_vel, CL_TRUE, 0, sizeof(cl_float4)*size_buffer, h_vel, 0, NULL, &event_write_buffer[0]);
        if(err!=0){std::cout<<"clEnqueueWriteBuffer Error! Error Code: "<<err<<std::endl;}
}


void updatePos(){


//  ENQUEUE WRITES TO MEMORY OBJECT
    err = clEnqueueWriteBuffer(QUEUE, dev_pos_old, CL_TRUE, 0, sizeof(cl_float4)*size_buffer, h_pos, 0, NULL, &event_write_buffer[1]);
        if(err!=0){std::cout<<"clEnqueueWriteBuffer Error! Error Code: "<<err<<std::endl;}



//  ENQUEUE TASKS = or =  ENQUEUE KERNEL

    err = clEnqueueNDRangeKernel(QUEUE, kernel, 1, NULL, &global_size, &local_size, 1, &event_write_buffer[1], event_kernel_run);
        if(err!=0){std::cout<<"clEnqueueTask Error! Error Code: "<<err<<std::endl;}


// ENQUEUE READS FROM MEMORY OBJECTS
    err = clEnqueueReadBuffer(QUEUE, dev_pos_new, CL_TRUE, 0, sizeof(cl_float4)*size_buffer, h_pos, 1, event_kernel_run, &event_read_buffer[0]);
        if(err!=0){std::cout<<"clEnqueueReadBuffer Error! Error Code: "<<err<<std::endl;}

    err = clWaitForEvents(1, event_read_buffer);
        if(err!=0){std::cout<<"clWaitForEvents Error! Error Code: "<<err<<std::endl;}

//  RELEASE MEM OBJECTS

    //printArray(h_pos, 4*size_buffer);
    profileEvent(&event_write_buffer[1], 1, "writes");
    profileEvent(event_kernel_run, 1, "compute");
    profileEvent(&event_read_buffer[0], 1, "reads");


}

#endif // OCL_COMMONFUNCS_H
