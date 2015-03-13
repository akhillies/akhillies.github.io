
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

//libraries included so we can be lazy and/or do some things
#include <string>       // for translating strings into numbers
#include <sstream>      // for converting strings to floats
#include <functional>   // for vector addition/subtraction
#include <numeric>      // for vector dot product
//#include "FreeImage.h"


#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;

bool toonify;
float numSpheres = 1;
float numCubes = 0;
bool cube = false;
vector<vector<vector<float> > > plPos(0);
vector<vector<vector<float> > > dlPos(0);
vector<float> camera(3, 0);
bool hasAmbient;
vector<float> ambientRGB(3, 0);
bool hasDiffuse;
vector<float> diffuseRGB(3, 0);
bool hasSpecular;
vector<float> specRGB(3, 0);
float specIntensity;


//****************************************************
// Simple init function
//****************************************************
void initScene(){

  // Nothing to do here for this simple example.

}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);

}

//****************************************************
// converts the string inputs into floats so we can
// actually use them as numbers when making color vector
//
// adapted from http://stackoverflow.com/questions/1012571/stdstring-to-float-or-double
//****************************************************
float convertStringToFloat(string num) {
    float out;
    stringstream stream(num);
    stream >> out;
    if (stream.fail()) {
        runtime_error error(num);
        throw error;
    }
    return out;
}



//********************************************************
// finds the index of a given element in the given array
// - return -1 if the array does not have the element
//********************************************************
int indexOf(vector<string> array, string element, int instance=1)
{
    int reached = 0;
    for(int i = 0; i < array.size(); i++) {
        if(array[i].compare(element) == 0) {
            reached++;
            if(reached == instance) {
                return i;
            }
        }
    }
    return -1;
}

//***********************************************************
// adds two vectors together (each component)
// multiply one of the vectors by -1 using the multiplyByConst menthod for subtraction
//***********************************************************
vector<float> addVectors(vector<float> v1, vector<float> v2) {
    vector<float> out(fmin(v1.size(), v2.size()));
    for(int i = 0; i < fmin(v1.size(), v2.size()); i++) {
        out[i] = v1[i] + v2[i];
    }
    return out;
}

//***********************************************************
// multiplies all the components of a vector by a given float
//***********************************************************
vector<float> multiplyByConst(vector<float> vin, float num) {
    vector<float> vout(vin.size());
    for(int i = 0; i < vin.size(); i++) {
        vout[i] = vin[i] * num;
    }
    return vout;
}

//***********************************************************
// gives the dot product of two vectors
//***********************************************************
float dotProduct(vector<float> v1, vector<float> v2) {
    float total = 0;
    for(int i = 0; i < fmin(v1.size(), v2.size()); i++) {
        total += v1[i] * v2[i];
    }
    return total;
}

//***********************************************************
// gives the result of the distance formula between two vectors
//***********************************************************
float distanceFormula(vector<float> v1, vector<float> v2) {
    float total = 0;
    for(int i = 0; i < fmin(v1.size(), v2.size()); i++) {
        total += sqr(v1[i] - v2[i]);
    }
    return sqrt(total);
}

//************************************************************
// normalizes the vector
//************************************************************
vector<float> normalize(vector<float> v) {
    float total = 0;
    for( float f : v) {
        total += sqr(f);
    }
    total = 1.0 / sqrt(total);
    return multiplyByConst(v, total);
}

//************************************************************
// negates the vector
//************************************************************
vector<float> negateVector(vector<float> v) {
    return multiplyByConst(v, -1);
}



//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);   // The 0.5 is to target pixel
  // centers 
  // Note: Need to check for gap
  // bug on inst machines.
}


//*****************************************************
// following three functions are used for making the image cartoony
// all toon-ifying code adapted from http://prideout.net/blog/?p=22
//*****************************************************
float smoothConst(float edge1, float edge2, float shadingConst) {
    if (shadingConst <= edge1) {
        return 0;
    }
    if (shadingConst >= edge2) {
        return 1;
    }
    float interpolation = fmin(fmax((shadingConst - edge1) / (edge2 - edge1), 0), 1);
    return interpolation * interpolation * (3 - 2 * interpolation);
}

float mixDiffuseConst(float after, float before, float dfConst) {
    return after * (1 - dfConst) + before * dfConst;
}


//*****************************************************
// function for getting the diffuse shading value
//*****************************************************
vector<float> diffuse(vector<float> normal, vector<float> light, vector<float> lightRGB) {
    
    float diffuseConst = fmax(dotProduct(normal, light), 0);  // the first part of the diffuse formula

    if(toonify) {
        float df_lvl1 = .1;
        float df_lvl2 = .33;
        float df_lvl3 = .66;
        float df_lvl4 = 1;
        float deltaDf = .01;
        if (diffuseConst > df_lvl1 - deltaDf and diffuseConst < df_lvl1 + deltaDf) {
            diffuseConst = mixDiffuseConst(df_lvl1, df_lvl2, smoothConst(df_lvl1 - deltaDf, df_lvl1 + deltaDf, diffuseConst));
        }
        else if (diffuseConst > df_lvl2 - deltaDf and diffuseConst < df_lvl2 + deltaDf) {
            diffuseConst = mixDiffuseConst(df_lvl2, df_lvl3, smoothConst(df_lvl2 - deltaDf, df_lvl2 + deltaDf, diffuseConst));
        }
        else if (diffuseConst > df_lvl3 - deltaDf and diffuseConst < df_lvl3 + deltaDf) {
            diffuseConst = mixDiffuseConst(df_lvl3, df_lvl4, smoothConst(df_lvl3 - deltaDf, df_lvl3 + deltaDf, diffuseConst));
        }
        else if (diffuseConst < df_lvl1) {
            diffuseConst = 0;
        }
        else if (diffuseConst < df_lvl2) {
            diffuseConst = df_lvl2;
        }
        else if (diffuseConst < df_lvl3) {
            diffuseConst = df_lvl3;
        }
        else {
            diffuseConst = df_lvl4;
        }
    }
    
    vector<float> rgbValue(3);
    // adding the final values that come from the diffuse shading
    rgbValue[0] = lightRGB[0] * diffuseRGB[0] * diffuseConst;
    rgbValue[1] = lightRGB[1] * diffuseRGB[1] * diffuseConst;
    rgbValue[2] = lightRGB[2] * diffuseRGB[2] * diffuseConst;
    
    return rgbValue;
}

//*****************************************************
// function for getting the specular shading value
//*****************************************************
vector<float> specular(vector<float> normal, vector<float> light, vector<float> lightRGB) {
    vector<float> r = addVectors(negateVector(light), multiplyByConst(normal, 2 * dotProduct(normal, light)));
    float specConst = pow(fmax(dotProduct(r, camera), 0), specIntensity); // the first part of the specular formula
    
    if(toonify) {
        float deltaSp = .01;
        if (specConst > 0.5 - deltaSp and specConst < 0.5 + deltaSp) {
            specConst = smoothConst(0.5 - deltaSp, 0.5 + deltaSp, specConst);
        }
        else {
            specConst = round(specConst);
        }
    }
    
    vector<float> rgbValue(3);
    // adding values of the specular shading
    rgbValue[0] = lightRGB[0] * specRGB[0] * specConst;
    rgbValue[1] = lightRGB[1] * specRGB[1] * specConst;
    rgbValue[2] = lightRGB[2] * specRGB[2] * specConst;
    
    return rgbValue;
}


//****************************************************
// Draw a filled circle.
//****************************************************

void circle(float centerX, float centerY, float radius) {
  // Draw inner circle
  glBegin(GL_POINTS);

  // We could eliminate wasted work by only looping over the pixels
  // inside the sphere's radius.  But the example is more clear this
  // way.  In general drawing an object by loopig over the whole
  // screen is wasteful.

  int i,j;  // Pixel indices

    int minI = max(0,(int)floor(centerX-radius));
    int maxI = min(viewport.w-1,(int)ceil(centerX+radius));

    int minJ = max(0,(int)floor(centerY-radius));
    int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));
    
  for (i=minI;i<maxI;i++) {
    for (j=minJ;j<maxJ;j++) {
      // Location of the center of pixel relative to center of sphere
      float x = (i+0.5-centerX);
      float y = (j+0.5-centerY);

      float dist = sqrt(sqr(x) + sqr(y));

      if (dist<=radius) {
        // This is the front-facing Z coordinate
        float z = sqrt(radius*radius-dist*dist);

          vector<float> rgbValue(3, 0); // what will be used as the final values of the pixel
          
          vector<float> pixelPoint (3);  //giving the pixel a vector to use
          pixelPoint[0] = x / radius;
          pixelPoint[1] = y / radius;
          pixelPoint[2] = z / radius;
          
          vector<float> l(0);   // the light vector, calculated based on what the lighting is
          vector<float> n(0);
          n = normalize(pixelPoint);    // normal vector to the sphere at the pixel point
          
          // for ambient lighting
          if(hasAmbient) {
              rgbValue[0] += ambientRGB[0];
              rgbValue[1] += ambientRGB[1];
              rgbValue[2] += ambientRGB[2];
          }
          // for point lighting
          for(vector<vector<float> > pl : plPos) {
              
              l = normalize(addVectors(pl[0], negateVector(pixelPoint)));   // normalized light vector at this point on sphere
              
              // diffuse shading
              if(hasDiffuse) {
                  // adding the values that come from the diffuse shading
                  rgbValue = addVectors(rgbValue, diffuse(n, l, pl[1]));
              }
              
              // specular shading
              if(hasSpecular) {
                  // adding values to the RGB vector
                  rgbValue = addVectors(rgbValue, specular(n, l, pl[1]));
              }
          }
          
          for(vector<vector<float> > dl : dlPos) {
              
              l = normalize(negateVector(dl[0]));   // normalized light vector at this point on sphere
              
              // diffuse shading
              if(hasDiffuse) {
                  // adding the values that come from the diffuse shading
                  rgbValue = addVectors(rgbValue, diffuse(n, l, dl[1]));
              }
              
              // specular shading
              if(hasSpecular) {
                  // adding values to the RGB vector
                  rgbValue = addVectors(rgbValue, specular(n, l, dl[1]));
              }
          }
          
//          if(!bmp) {
//              RGBQUAD color;
//              color.rgbRed = rgbValue[0];
//              color.rgbGreen = rgbValue[1];
//              color.rgbBlue = rgbValue[2];
//              FreeImage_SetPixelColor(bmp, i, j, &color);
//          }
//          else {
              setPixel(i,j, rgbValue[0], rgbValue[1], rgbValue[2]);
//          }
        // This is amusing, but it assumes negative color values are treated reasonably.
        // setPixel(i,j, x/radius, y/radius, z/radius );
      }
    }
  }


  glEnd();
}

void square(float centerX, float centerY, float sideLength) {
    // Draw square...
    glBegin(GL_POINTS);
    
    int i,j;  // Pixel indices
    
    int minI = centerX - sideLength / 2;
    int maxI = centerX + sideLength * 1.75;
    
    int minJ = centerY;
    int maxJ = centerY + sideLength * 1.75;
    
    for (i=minI;i<maxI;i++) {
        for (j=minJ;j<maxJ;j++) {
            // Location of the center of pixel relative to center of sphere
            float x = (i+0.5-centerX);
            float y = (j+0.5-centerY);
            
            
            vector<float> rgbValue(3, 0); // what will be used as the final values of the pixel
            
            vector<float> viewerVec(3, 0);
            viewerVec[2] = -1;
            
            vector<float> pixelPoint (3);  //giving the pixel a vector to use
            pixelPoint[0] = x;
            pixelPoint[1] = y;
            if(x < 0 or y < 0 or x > sideLength * 1.25 or y > sideLength * 1.25){
                continue;
            }
            else if(x <= sideLength and y <= sideLength) {
                pixelPoint[2] = sideLength / 2;
            }
            else if(x >= sideLength / 2 and x <= sideLength * 1.25 and y <= sideLength * 1.25 and y - sideLength  - 1 <= (x - sideLength) * tan(PI / 4)) {
                if(y < (x - sideLength) * tan(PI / 4)) {
                    continue;
                }
                pixelPoint[2] = sideLength * (x - sideLength) / 1.25 + sideLength / 2;
                viewerVec[0] = 1;
            }
            else if(y >= sideLength and y <= sideLength * 1.25 and x <= sideLength * 1.25) {
                if(x < (y - sideLength) / tan(PI / 4) or x - sideLength - 1 > (y - sideLength) * tan(PI / 4)) {
                    continue;
                }
                pixelPoint[2] = sideLength * (y - sideLength) / 1.25 + sideLength / 2;
                viewerVec[1] = 1;
            }
            viewerVec = normalize(viewerVec);
            vector<float> l(0);   // the light vector, calculated based on what the lighting is
            vector<float> n(0);
            n = normalize(addVectors(negateVector(viewerVec), normalize(pixelPoint)));    // normal vector to the sphere at the pixel point
            
            // for ambient lighting
            if(hasAmbient) {
                rgbValue[0] += ambientRGB[0];
                rgbValue[1] += ambientRGB[1];
                rgbValue[2] += ambientRGB[2];
            }
            // for point lighting
            for(vector<vector<float> > pl : plPos) {
                
                l = normalize(addVectors(pl[0], negateVector(pixelPoint)));   // normalized light vector at this point on sphere
                
                // diffuse shading
                if(hasDiffuse) {
                    // adding the values that come from the diffuse shading
                    rgbValue = addVectors(rgbValue, diffuse(n, l, pl[1]));
                }
                
                // specular shading
                if(hasSpecular) {
                    // adding values to the RGB vector
                    rgbValue = addVectors(rgbValue, specular(n, l, pl[1]));
                }
            }
            
            for(vector<vector<float> > dl : dlPos) {
                
                l = normalize(negateVector(dl[0]));   // normalized light vector at this point on sphere
                
                // diffuse shading
                if(hasDiffuse) {
                    // adding the values that come from the diffuse shading
                    rgbValue = addVectors(rgbValue, diffuse(n, l, dl[1]));
                }
                
                // specular shading
                if(hasSpecular) {
                    // adding values to the RGB vector
                    rgbValue = addVectors(rgbValue, specular(n, l, dl[1]));
                }
            }
            
            
            setPixel(i,j, rgbValue[0], rgbValue[1], rgbValue[2]);
            // This is amusing, but it assumes negative color values are treated reasonably.
            // setPixel(i,j, x/radius, y/radius, z/radius );
        }
    }
    
    glEnd();
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT);				// clear the color buffer

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
  glLoadIdentity();				        // make sure transformation is "zero'd"


  // Start drawing
    if(numSpheres == 1) {
        circle(viewport.w / 2.0 , viewport.h / 2.0 , min(viewport.w, viewport.h) / 3.0);
    }
    else {
        float smallEdge = min(viewport.w, viewport.h);
        float num = 3;
        float x = fmin(fmax(viewport.w / ((rand() + 1) % 3 + 1), smallEdge / num), viewport.w - smallEdge / num);
        float y = fmin(fmax(viewport.h / ((rand() + 1) % 3 + 1), smallEdge / num), viewport.h - smallEdge / num);
        for(int i = 0; i < numSpheres; i++) {
            circle(x , y , smallEdge / num);
            num += ((rand() + 1) % 4) * ((float)rand()) / ((float)(RAND_MAX));
            x = fmin(fmax(viewport.w / ((rand() + 1) % 5 + 1), smallEdge / num), viewport.w - smallEdge / num);
            y = fmin(fmax(viewport.h / ((rand() + 1) % 5 + 1), smallEdge / num), viewport.h - smallEdge / num);
        }
    }
    
    if(numCubes == 1) {
        square(viewport.w / 10.0 , viewport.h / 10.0 , min(viewport.w, viewport.h) / 2.0);
    }
    else {
        float smallEdge = min(viewport.w, viewport.h);
        float num = 3;
        float x = fmin(fmax(viewport.w / ((rand() + 1) % 3 + 1), smallEdge / num), viewport.w - smallEdge * 1.25 / num);
        float y = fmin(fmax(viewport.h / ((rand() + 1) % 3 + 1), smallEdge / num), viewport.h - smallEdge * 1.25 / num);
        for(int i = 0; i < numCubes; i++) {
            square(x / 1.25, y / 1.25, smallEdge / num);
            num += ((rand() + 1) % 4) * ((float)rand()) / ((float)(RAND_MAX));
            x = fmin(fmax(viewport.w / ((rand() + 1) % 5 + 1), smallEdge / num), viewport.w - smallEdge * 1.25 / num);
            y = fmin(fmax(viewport.h / ((rand() + 1) % 5 + 1), smallEdge / num), viewport.h - smallEdge * 1.25 / num);
        }
    }

  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}



//******************************************************
// function that runs whenever a key is pressed on the keyboard
// - used only for exiting when space is pressed
//******************************************************
void keyPressed(unsigned char key, int x, int y) {
    if(key == ' ') {
        exit(0);
    }
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
  viewport.w = 400;
  viewport.h = 400;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);
    
    srand (time(NULL));
//    bool toFile = false;
    camera[2] = 1;  // changing the camera vector to be normal
    int i = 1;  // variable for going through the argv array
    while(argv[i] != NULL) {
        if(string(argv[i]).compare("-ka") == 0) {   // adding the ambient constant to the global variable
            hasAmbient = true;
            ambientRGB[0] += convertStringToFloat(argv[i+1]);
            ambientRGB[1] += convertStringToFloat(argv[i+2]);
            ambientRGB[2] += convertStringToFloat(argv[i+3]);
            i += 3;
        }
        else if(string(argv[i]).compare("-kd") == 0) {  // adding the diffuse shading color to the global variable
            hasDiffuse = true;
            diffuseRGB[0] = convertStringToFloat(argv[i+1]);
            diffuseRGB[1] = convertStringToFloat(argv[i+2]);
            diffuseRGB[2] = convertStringToFloat(argv[i+3]);
            i += 3;
        }
        else if(string(argv[i]).compare("-ks") == 0) {  // adding the specular shading color to the global variable
            hasSpecular = true;
            specRGB[0] = convertStringToFloat(argv[i+1]);
            specRGB[1] = convertStringToFloat(argv[i+2]);
            specRGB[2] = convertStringToFloat(argv[i+3]);
            i += 3;
        }
        else if(string(argv[i]).compare("-sp") == 0) {  // adding the specular light intensity to the global variable
            specIntensity = convertStringToFloat(argv[i+1]);
            i += 1;
        }
        else if(string(argv[i]).compare("-pl") == 0) {  // point light information - for each point light adds the xyz coordinates and the rgb color
            vector<float> xyz(3);
            vector<float> rgb(3);
            xyz[0] = convertStringToFloat(string(argv[i + 1]));
            xyz[1] = convertStringToFloat(string(argv[i + 2]));
            xyz[2] = convertStringToFloat(string(argv[i + 3]));
            rgb[0] = convertStringToFloat(string(argv[i + 4]));
            rgb[1] = convertStringToFloat(string(argv[i + 5]));
            rgb[2] = convertStringToFloat(string(argv[i + 6]));
            vector<vector<float> > vpl(2);
            vpl[0] = xyz;
            vpl[1] = rgb;
            plPos.push_back(vpl);
            i += 6;
        }
        else if(string(argv[i]).compare("-dl") == 0) {  // directional light information - xyz and rgb for each directional light
            vector<float> xyz(3);
            vector<float> rgb(3);
            xyz[0] = convertStringToFloat(string(argv[i + 1]));
            xyz[1] = convertStringToFloat(string(argv[i + 2]));
            xyz[2] = convertStringToFloat(string(argv[i + 3]));
            rgb[0] = convertStringToFloat(string(argv[i + 4]));
            rgb[1] = convertStringToFloat(string(argv[i + 5]));
            rgb[2] = convertStringToFloat(string(argv[i + 6]));
            vector<vector<float> > vpl(2);
            vpl[0] = xyz;
            vpl[1] = rgb;
            dlPos.push_back(vpl);
            i += 6;
        }
        else if(string(argv[i]).compare("-cn") == 0) {  // if this image should be cartooned or not
            toonify = true;
        }
        else if(string(argv[i]).compare("-ns") == 0) {  // number of spheres to include
            numSpheres = convertStringToFloat(string(argv[i + 1]));
            i += 1;
        }
        else if(string(argv[i]).compare("-nc") == 0) {  // number of cubes to include
            numCubes = convertStringToFloat(string(argv[i + 1]));
            i += 1;
        }
//        else if(string(argv[i]).compare("-tf") == 0) {
//            toFile = true;
//        }
        i++;
    }
    glutKeyboardFunc(keyPressed);     //sets the keyboard listener to the passed function
    
//    if(toFile) {
//        FreeImage_Initialise();
//        FIBITMAP* bitmap = FreeImage_Allocate (viewport.w, viewport.h, 24);
//        circle(viewport.w / 2, viewport.h / 2, viewport.w / 3, bitmap);
//        FreeImage_Save (FIF_PNG, bitmap , "test.png" , 0);
//        FreeImage_DeInitialise();
//    }
//    else {
        initScene();							// quick function to set up scene
        
        glutDisplayFunc(myDisplay);				// function to run when its time to draw something
        glutReshapeFunc(myReshape);				// function to run when the window gets resized
        
        
        glutMainLoop();							// infinite loop that will keep drawing and resizing
//    }
  // and whatever else
  return 0;
}








