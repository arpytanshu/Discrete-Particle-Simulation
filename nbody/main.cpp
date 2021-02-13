#include"oglFuncs.h"
#include "oclFuncs.h"
#include"oclParams.h"
#include"oglParam.h"

using namespace std;




int main(int argc, char* argv[]){

    srand(time(0));
    glutInit(&argc, argv);
    // std::cout<<"HW"<<std::endl;
    initOcl();
    // std::cout<<"init ocl suceess"<<std::endl;
    initOgl();
    // std::cout<<"init ogl suceess"<<std::endl;
}

// g++ main.cpp -lOpenCL -std=c++11  -lglut -lGLU -lGL -o main.o && ./main.o
