// CS184 Simple OpenGL Example
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265

using namespace std;

//****************************************************
// Some Classes
//****************************************************
class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport    viewport;
float mouseX;
float mouseY;
//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                // loading the identity matrix for the screen

  //----------- setting the projection -------------------------
  // glOrtho sets left, right, bottom, top, zNear, zFar of the chord system


  // glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
  // glOrtho(-w/400.0, w/400.0, -h/400.0, h/400.0, 1, -1); // resize type = center

  glOrtho(-1, 1, -1, 1, 1, -1);    // resize type = stretch

  //------------------------------------------------------------
}


//****************************************************
// sets the window up
//****************************************************
void initScene(){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

  myReshape(viewport.w,viewport.h);
}

void setMousePos(int x, int y)
{
    mouseX = (float)x / viewport.w;
    //place current mouse pos in mouse_
    mouseY = (float)(viewport.h - y) / viewport.h;
}

void draw5star(float centerX, float centerY, float height)
{
    float bottomHeight = cos(36.0f / 180.0f * PI) * height;
    float bottomWidth = sin(36.0f / 180.0f * PI) * height;
    float topHeight = cos(72.0f / 180.0f * PI) * height;
    float topWidth = sin(72.0f / 180.0f * PI) * height;
    
    float bottomSlope = (height - topHeight) / topWidth;
    float topSlope = (topHeight + bottomHeight) / (topWidth - bottomWidth);
    float p1x = (bottomSlope * centerX - topSlope * (centerX - topWidth) + height - topHeight) / (bottomSlope - topSlope);
    float p2x = (-bottomSlope * (centerX - topWidth) + topHeight + bottomHeight) / -bottomSlope;
    float p4x = (bottomSlope * (centerX + topWidth) + topHeight + bottomHeight) / bottomSlope;
    float p5x = (-topSlope * (centerX + topWidth) + bottomSlope * centerX + topHeight - height) / (bottomSlope - topSlope);
    
    glBegin(GL_POLYGON);
    glVertex3f(centerX, centerY - height, 0.0f);
    glVertex3f(p1x, bottomSlope * (p1x - centerX) + centerY - height, 0.0f);
    glVertex3f(centerX - topWidth, centerY - topHeight, 0.0f);
    glVertex3f(p2x, centerY + bottomHeight, 0.0f);
    glVertex3f(centerX - bottomWidth, centerY + bottomHeight, 0.0f);
    glVertex3f(centerX, topSlope * bottomWidth + centerY + bottomHeight, 0.0f);
    glVertex3f(centerX + bottomWidth, centerY + bottomHeight, 0.0f);
    glVertex3f(p4x, centerY + bottomHeight, 0.0f);
    glVertex3f(centerX + topWidth, centerY - topHeight, 0.0f);
    
    glVertex3f(p5x, -bottomSlope * (p5x - centerX) + centerY - height, 0.0f);
    
    glEnd();
}

void draw6star(float centerX, float centerY, float height)
{
    float sideLength = height / cos(PI / 3.0f);
    
    glBegin(GL_POLYGON);
    glVertex3f(centerX, centerY + height, 0.0f);
    glVertex3f(centerX + 0.5f * sideLength, centerY - 0.5f * height, 0.0f);
    glVertex3f(centerX - 0.5f * sideLength, centerY - 0.5f * height, 0.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(centerX, centerY - height, 0.0f);
    glVertex3f(centerX + 0.5f * sideLength, centerY + 0.5f * height, 0.0f);
    glVertex3f(centerX - 0.5f * sideLength, centerY + 0.5f * height, 0.0f);
    glEnd();
}

//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {
    //----------------------- ----------------------- -----------------------
    // This is a quick hack to add a little bit of animation.
    srand (time(NULL));
    static int length = rand() % 15 + 5;
    static vector<float> xSteps (length, 0.0001f);
    static vector<float> ySteps (length, 0.0001f);
    static vector<float> xes (length, 0.0f);
    static vector<float> ys (length, 0.0f);
    static vector<float> heights (length, 0.0f);
    static vector<bool> whatStar (length, false);
    static bool setStars = true;

    if(setStars)
    {
        for(int i = 0; i < length; i++)
        {
            xes[i] = rand() / (RAND_MAX * 2.0f) - 1.0f;
            ys[i] = rand() / (RAND_MAX * 2.0f) - 1.0f;
            whatStar[i] = rand() > RAND_MAX / 2.0f ? true : false;
            xSteps[i] += rand() / (RAND_MAX * 22.0f);
            ySteps[i] += rand() / (RAND_MAX * 22.0f);
            if(whatStar[i])
            {
                heights[i] = rand() / (RAND_MAX * 15.0f);
            }
            else
            {
                heights[i] = rand() / (RAND_MAX * 7.0f);
            }
        }
        setStars = false;
    }
    
  //----------------------- ----------------------- -----------------------


  glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)

  glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
  glLoadIdentity();                            // make sure transformation is "zero'd"

  //----------------------- code to draw objects --------------------------
  // Square Code
  //glColor3f(red component, green component, blue component);
    glutPassiveMotionFunc( setMousePos );

    for(float j = 0.0f; j <= 2.0f; j+=0.01f){
        for(float i = 0.0f; i <= 2.0f; i+=0.01f){
            float red = i*j;
            float green = mouseX >= i/2 ? mouseX - i/2 : i/2 - mouseX;
            float blue = mouseY >= j/2 ? mouseY - j/2 : j/2 - mouseY;
            glColor3f(red, green + 0.05f * i, blue + 0.05f * j); // setting the color

            glBegin(GL_POLYGON); // draw rectangle
            //glVertex3f(x val, y val, z val (won't change the point because of the projection type));
            glVertex3f( -1.0f - 0.005f + i, -1.0f - 0.005f + j, 0.0f); // bottom left corner of rectangle
            glVertex3f( -1.0f - 0.005f + i, -1.0f + 0.005f + j, 0.0f); // top left corner of rectangle
            glVertex3f( -1.0f + 0.005f + i, -1.0f + 0.005f + j, 0.0f); // top right corner of rectangle
            glVertex3f( -1.0f + 0.005f + i, -1.0f - 0.005f + j, 0.0f); // bottom right corner of rectangle
            glEnd();
        }
    }
    // star code
    for(int i = 0; i < length; i++)
    {
        glColor3f(i, xes[i], ys[i]); // setting the color
        if(whatStar[i])
        {
            draw5star(xes[i], ys[i], heights[i]);
        }
        else
        {
            draw6star(xes[i], ys[i], heights[i]);
        }
        
        xes[i] += xSteps[i];
        ys[i] += ySteps[i];
        
        if(xes[i] <= -1.0f || xes[i] >= 1.0f)
        {
            xSteps[i] *= -1;
        }
        if(ys[i] <= -1.0f || ys[i] >= 1.0f)
        {
            ySteps[i] *= -1;
        }
    }
    
    //-----------------------------------------------------------------------

    glFlush();
    glutSwapBuffers();                           // swap buffers (we earlier set double buffer)
}

//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 1000;
  viewport.h = 1000;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("CS184!");

  initScene();                                 // quick function to set up scene

  glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
  glutReshapeFunc(myReshape);                  // function to run when the window gets resized
  glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}








