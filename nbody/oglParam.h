#ifndef OGL_PARAM_H
#define OGL_PARAM_H


#define BUF_LEN 1024


/***WINDOW PARAM***/
int windowWidth = 1500;
int windowHeight = 900;
char* windowName = "Window";
int windowTopLeftX = 0;
int windowTopLeftY = 0;

char text[100] = "i am a useless piece of shtring";

int th = 0;             //azimuth of view angle
int ph = 0;             //elevation of view angle
float zoom = 0.2;

float curr_width;
float curr_height;

/***SOME COLORS ***/
GLfloat RED[3] = {1.0, 0.0, 0.0};
GLfloat BLUE[3] = {0.0, 1.0, 0.0};
GLfloat GREEN[3] = {0.0, 0.0, 1.0};
GLfloat WHITE[3] = {1.0, 1.0, 1.0};
GLfloat BLACK[3] = {0.0, 0.0, 0.0};
GLfloat PINK[3] = {1.0, 0.0, 1.0};
GLfloat YELLOW[3] = {0.0, 1.0, 1.0};


#endif // PARAM_H
