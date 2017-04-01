//#include"oglFuncs.h"
//#include"oglParam.h"

#include "oclFuncs.h"
#include "oclParams.h"
#include "oglFuncs.h"
#include "oglParam.h"
using namespace std;

using namespace std;

int main(int argc, char* argv[]){

    srand(time(0));

    initOcl();
//    printArray(h_position, global_size*4, "positions - 1");
//
//    updatePos();
//
//    printArray(h_position, global_size*4, "positions - 1");

    glutInit(&argc, argv);
    initOgl();

}
