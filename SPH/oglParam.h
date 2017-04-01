#ifndef OGL_PARAM_H
#define OGL_PARAM_H


#define BUF_LEN 1024


/***WINDOW PARAM***/
int windowWidth = 1590;
int windowHeight = 900;
char* windowName = "Window";
int windowTopLeftX = 10;
int windowTopLeftY = 10;


int th = 10;             //azimuth of view angle
int ph = 10;             //elevation of view angle




/***SOME COLORS ***/
GLfloat RED[3] = {1.0, 0.0, 0.0};
GLfloat BLUE[3] = {0.0, 1.0, 0.0};
GLfloat GREEN[3] = {0.0, 0.0, 1.0};
GLfloat WHITE[3] = {1.0, 1.0, 1.0};
GLfloat BLACK[3] = {0.0, 0.0, 0.0};
GLfloat PINK[3] = {1.0, 0.0, 1.0};
GLfloat YELLOW[3] = {0.0, 1.0, 1.0};


#endif // PARAM_H
