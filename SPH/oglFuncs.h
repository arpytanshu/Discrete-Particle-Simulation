#ifndef OGL_FUNCS_H
#define OGL_FUNCS_H

#include<GL/glut.h>
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>
#include"oglParam.h"
#include"oclFuncs.h"


void printText(int x, int y, int z, char* text){
    char* ch = text;
    while(*ch){
//       glRasterPos2i(x,y);
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
    float zoom = 0.15;
    double w2h = (height>0) ? (double)width/height : 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-dim*w2h, +dim*w2h, -dim, +dim, -dim, +dim);
    //gluOrtho2D(-width*zoom, width*zoom, -height*zoom, height*zoom);
    //gluOrtho2D(0, windowWidth, 0, windowHeight);  //try this to see the difference
    glOrtho(-width*zoom*0.5, width*zoom*1.5, -height*zoom*0.5, height*zoom*1.5, -width, width);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glRotated(ph,1,0,0);
	glRotated(th,0,1,0);

	printText(110, 0, 0, "X");
	printText(0, 110, 0, "Y");
	printText(0, 0, 110, "Z");
    printText(-3, -3, -3, "O");



	glPointSize(1.2);
    glColor3f(0.2, 1.0, 1.0);

    glBegin(GL_LINES);
        for(int i=0; i<size_buffer; i++){
                cl_float x = h_position[i*4 + 0];
                cl_float y = h_position[i*4 + 1];
                cl_float z = h_position[i*4 + 2];

                glVertex3f(x-1,y,z);
                glVertex3f(x+1,y,z);
                glVertex3f(x,y,z-1);
                glVertex3f(x,y,z+1);
                glVertex3f(x,y-1,z);
                glVertex3f(x,y+1,z);
        }
    glEnd();

        glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINES); //display the box
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(200.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 100.0);

        glVertex3f(200.0, 0.0, 100.0);
        glVertex3f(0.0, 0.0, 100.0);
        glVertex3f(200.0, 0.0, 100.0);
        glVertex3f(200.0, 0.0, 0.0);
        glVertex3f(200.0, 0.0, 100.0);
        glVertex3f(200.0, 100.0, 100.0);

        glVertex3f(200.0, 100.0, 0.0);
        glVertex3f(200.0, 100.0, 100.0);
        glVertex3f(200.0, 100.0, 0.0);
        glVertex3f(200.0, 0.0, 0.0);
        glVertex3f(200.0, 100.0, 0.0);
        glVertex3f(0.0, 100.0, 0.0);
    glEnd();

    glBegin(GL_LINES);

        glVertex3f(90, 0, 0);
        glVertex3f(90, 0, 100);
        glVertex3f(90, 15, 0);
        glVertex3f(90, 15, 100);

        glVertex3f(110, 0, 0);
        glVertex3f(110, 0, 100);
        glVertex3f(110, 15, 0);
        glVertex3f(110, 15, 100);

        glVertex3f(90, 0, 0);
        glVertex3f(90, 15, 0);
        glVertex3f(90, 0, 100);
        glVertex3f(90, 15, 100);

        glVertex3f(110, 0, 0);
        glVertex3f(110, 15, 0);
        glVertex3f(110, 0, 100);
        glVertex3f(110, 15, 100);

        glVertex3f(90, 15, 0);
        glVertex3f(110, 15, 0);
        glVertex3f(90, 15, 100);
        glVertex3f(110, 15, 100);


//SOLID OBJECT
//          glVertex3f(90, 0, 0);
//        glVertex3f(90, 0, 100);
//        glVertex3f(90, 15, 0);

//        glVertex3f(90, 0, 100);
//        glVertex3f(90, 15, 0);
//        glVertex3f(90, 15, 100);
//
//        glVertex3f(110, 0, 0);
//        glVertex3f(110, 0, 100);
//        glVertex3f(110, 15, 0);
//
//        glVertex3f(110, 0, 100);
//        glVertex3f(110, 15, 0);
//        glVertex3f(110, 15, 100);
//
//        glVertex3f(90, 15, 0);
//        glVertex3f(90, 15, 100);
//        glVertex3f(110, 15, 0);
//
//        glVertex3f(90, 15, 100);
//        glVertex3f(110, 15, 0);
//        glVertex3f(110, 15, 100);


    glEnd();




//    printText(100, 100, text);

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

void initOgl(){
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(windowTopLeftX,windowTopLeftY);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(windowName);
    glClearColor(0.0, 0.0, 0.0, 0.0);   //sets display window color <<BLACK>>


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(windowSpecial);
    glutIdleFunc(idle);
    glutMainLoop();
}

#endif // HELPER_H
