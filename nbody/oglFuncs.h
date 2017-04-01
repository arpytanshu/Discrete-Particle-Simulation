#ifndef OGL_FUNCS_H
#define OGL_FUNCS_H

#include<GL/glut.h>
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>
#include"oglParam.h"
#include"oclFuncs.h"





void printText3d(int x, int y, int z, char* text){
    char* ch = text;
    while(*ch){
       glRasterPos3i(x,y,z);
       glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *ch++);
       x+=9;y+=0;
    }
}

void idle(){
    updatePos();
    glutPostRedisplay();
}

static void reshape(GLint width, GLint height)
{

    curr_width = width;
    curr_height = height;
    double w2h = (height>0) ? (double)width/height : 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-dim*w2h, +dim*w2h, -dim, +dim, -dim, +dim);
    //gluOrtho2D(-width*zoom, width*zoom, -height*zoom, height*zoom);
    //gluOrtho2D(0, windowWidth, 0, windowHeight);  //try this to see the difference
    glOrtho(-width*zoom, width*zoom, -height*zoom, height*zoom, -width*zoom, width*zoom);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glRotated(ph,1,0,0);
	glRotated(th,0,1,0);

    //glOrtho(-curr_width*zoom, curr_width*zoom, -curr_height*zoom, curr_height*zoom, -curr_width*zoom, curr_width*zoom);


	glPointSize(0.5);
    glColor3f(1.0, 1.0, 0.4);

    glBegin(GL_POINTS);
        for(int i=0; i<size_buffer; i++){
            glVertex3fv(&h_pos[i*4]);
        }
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0,150.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0,150.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(150.0,0.0, 0.0);
    glEnd();


    printText3d(150.0, 0.0,0.0, "X");
    printText3d(0.0, 150.0, 0, "Y");
    printText3d(0.0, 0.0, 150.0, "Z");

//    printText2d(-150.0, -150.0, "");
//    printText2d(-150.0, -165.0, "");
//    printText2d(-150.0, -180.0, "");
//    printText2d(-150.0, -195.0, "");


	glFlush();
	glutSwapBuffers();
}

void windowSpecial(int key, int x, int y){
    if(key == GLUT_KEY_RIGHT) th+=5;
    else if(key == GLUT_KEY_LEFT) th-=5;
    else if (key == GLUT_KEY_UP) ph+=5;
    else if (key == GLUT_KEY_DOWN) ph-=5;

    th %= 360;
    ph %= 360;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){

    glutPostRedisplay();
}


void initOgl(){
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(windowTopLeftX,windowTopLeftY);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(windowName);
    glClearColor(0.0, 0.0, 0.0, 0.0);   //sets display window color <<BLACK>>


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(windowSpecial);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
}






#endif // HELPER_H
