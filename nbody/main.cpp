#include"oglFuncs.h"
#include "oclFuncs.h"
#include"oclParams.h"
#include"oglParam.h"

using namespace std;




int main(int argc, char* argv[]){

    srand(time(0));
    glutInit(&argc, argv);
    initOcl();
    initOgl();

}
