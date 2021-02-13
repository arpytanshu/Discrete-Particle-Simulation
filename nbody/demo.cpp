
#include<CL/cl.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<random>
#include<time.h>
#include<math.h>
#include "oclFuncs.h"


/*
$ g++ main.cpp -lOpenCL -std=c++11  -lglut -lGLU -lGL -o main.o && ./main.o
*/

int main(){
    int i;
    //  CREATE PLATFORM
    err = clGetPlatformIDs(0, NULL, &ret_num_platforms); //get no of available platforms
        if(err!=0){std::cout<<"clGetPlatformIDs Error! Error Code: "<<err<<std::endl;}

    platforms = new cl_platform_id[ret_num_platforms];

    err = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
        if(err!=0){std::cout<<"clGetPlatformIDs Error! Error Code: "<<err<<std::endl;}
        else{std::cout<<"Num Platforms returned:"<<ret_num_platforms<<std::endl;}

    //  CREATE DEVICES FOR THE SELECTED PLATFORM <select platform from 'params.h'>
    //get number of devices of each type in the selected platform
    //  <<<THE CPUs ARE FILLED IN FIRST>>>
    
    err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_ALL, NULL, NULL, &ret_num_devices_all);
        if(err!=0){std::cout<<"clGetDeviceIDs Error! Error Code: "<<err<<std::endl;}
        else{std::cout<<"Num devices (all) returned:"<<ret_num_devices_all<<std::endl;}
    err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_CPU, NULL, NULL, &ret_num_devices_cpu);
        if(err!=0){std::cout<<"clGetDeviceIDs Error! Error Code: "<<err<<std::endl;}
        else{std::cout<<"Num devices (cpu) returned:"<<ret_num_devices_cpu<<std::endl;}
    err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_GPU, NULL, NULL, &ret_num_devices_gpu);
        if(err!=0){std::cout<<"clGetDeviceIDs Error! Error Code: "<<err<<std::endl;}
        else{std::cout<<"Num devices (gpu) returned:"<<ret_num_devices_gpu<<std::endl;}


    devices = new cl_device_id[ret_num_devices_all];

    if(ret_num_devices_cpu>0){
        std::cout<<"ret_num_devices_cpu"<<std::endl;
        err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_CPU, ret_num_devices_cpu, &devices[0], NULL);
            if(err!=0){std::cout<<"clGetDeviceIDs Error! Error Code: "<<err<<std::endl;}
    }
    if(ret_num_devices_gpu>0){
        std::cout<<"ret_num_devices_gpu"<<std::endl;
        err = clGetDeviceIDs(PLATFORM, CL_DEVICE_TYPE_GPU, ret_num_devices_gpu, &devices[0+ret_num_devices_cpu], NULL);
            if(err!=0){std::cout<<"clGetDeviceIDs Error! Error Code: "<<err<<std::endl;}
    }


    return 0;
}
