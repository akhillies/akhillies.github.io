//
//  ass2.cpp
//
//
//  Created by Akhil Batra on 2/27/14.
//
//
#include "ass2.h"
#include "src/pixel.h"
#include "src/camera.h"
#include "src/shape.h"
#include "src/sphere.h"
#include "src/triangle.h"

#include "freeImage/FreeImage.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>

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
Viewport viewport;
vector<vector<Pixel> > pixels(0);
vector<Shape*> objs(0);
vector<vector<Vec3> > pointLight(0);
vector<vector<Vec3> > directLight(0);


//****************************************************
// outputs the image in the PPM format
//****************************************************
void writePPMimage(string name) {
    string s = (string("outputImages/") + name + string(".ppm"));
    FILE *pfile = fopen(s.c_str(), "w");
    
    if (!pfile) {
        cout << "FILE IS NULL" << endl;
        return;
    }
    
    fprintf(pfile,"P3\n # %s\n %d %d\n 65535\n",s.c_str(), viewport.w,viewport.h);
    
    for(int x = 0; x < viewport.w; x++) {
        for(int y = 0; y < viewport.h; y++) {
            fprintf(pfile,"%d %d %d  ",
                    max(min((int)(pixels[x][y].getRed() * 65535), 65535), 0),
                    max(min((int)(pixels[x][y].getBlue() * 65535), 65535), 0),
                    max(min((int)(pixels[x][y].getGreen() * 65535), 65535), 0));
        }
        fprintf(pfile, "\n");
    }
}

//****************************************************
// outputs the image in the PNG format
//****************************************************
void writePNGimage(string name) {
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(viewport.w, viewport.h, 24);
    if(bitmap) {
        for (int x = 0; x < viewport.w; x++) {
            for (int y = 0; y < viewport.h; y++) {
                RGBQUAD color;
                color.rgbRed = fmax(fmin(pixels[x][y].getRed() * 255.0, 255), 0);
                color.rgbGreen = fmax(fmin(pixels[x][y].getGreen() * 255.0, 255), 0);
                color.rgbBlue = fmax(fmin(pixels[x][y].getBlue() * 255.0, 255), 0);
                FreeImage_SetPixelColor ( bitmap, x, y, &color );
            }
        }
        FreeImage_Save(FIF_PNG, bitmap, (string("outputImages/") + name + string(".png")).c_str(), 0);
        FreeImage_DeInitialise();
    }
}

Vec3 trace(Ray r, int dep) {
    if(dep == 0) {
        return Vec3();
    }
    
    Vec3 color = Vec3(0, 0, 0);
    int closestObj = -1;
    float minT = 99999;
    for(int i = 0; i < objs.size(); i++) {
        Ray newR((objs[i])->transformVec4(r.getSource(), 1), (objs[i])->transformVec4(r.getDirection(), 0), 0, 100);
        float t = (objs[i])->intersection(newR);
        if(t != -1 && minT >= t) {
            closestObj = i;
            minT = t;
            r = newR;
        }
    }
    
    if(closestObj != -1) {
        Vec3 intersectPt = r.applySteps(minT);
        Vec3 worldIntersect = (objs[closestObj])->untransformVec4(intersectPt, 1);
        Vec3 normal = (objs[closestObj])->transformVec4((objs[closestObj])->getNormal(worldIntersect), 0);
        
        for(int i = 0; i < pointLight.size(); i++) {
            Ray worldShad(worldIntersect, pointLight[i][0].subVec3(worldIntersect), .001, 100);
            float t = -1;
            for(int j = 0; j < objs.size(); j++) {
                Ray otherObjShad((objs[j])->transformVec4(worldShad.getSource(), 1), (objs[j])->transformVec4(worldShad.getDirection(), 0), worldShad.getMinT(), worldShad.getMaxT());
                t = (objs[j])->intersection(otherObjShad);
                if(t != -1) {
                    break;
                }
            }
            
            if(t == -1) {
                Vec3 lightVec = (objs[closestObj])->transformVec4(pointLight[i][0], 1).subVec3(intersectPt).normalize();
                
                color = color.addVec3((objs[closestObj])->calcDiffuse(normal,
                                                                      lightVec,
                                                                      pointLight[i][1]));
                color = color.addVec3((objs[closestObj])->calcSpec(normal,
                                                                   r.getSource().subVec3(intersectPt).normalize(),
                                                                   lightVec,
                                                                   pointLight[i][1]));
            }
        }
        
        for(int i = 0; i < directLight.size(); i++) {
            Ray worldShad(worldIntersect, directLight[i][0].subVec3(worldIntersect), .001, 100);
            float t = -1;
            for(int j = 0; j < objs.size(); j++) {
                Ray otherObjShad((objs[j])->transformVec4(worldShad.getSource(), 1), (objs[j])->transformVec4(worldShad.getDirection(), 0), worldShad.getMinT(), worldShad.getMaxT());
                t = (objs[j])->intersection(otherObjShad);
                if(t != -1) {
                    break;
                }
            }
            
            if(t == -1) {
                Vec3 lightVec = (objs[closestObj])->transformVec4(directLight[i][0], 0).normalize();
                
                color = color.addVec3((objs[closestObj])->calcDiffuse(normal,
                                                                      lightVec,
                                                                      directLight[i][1]));
                color = color.addVec3((objs[closestObj])->calcSpec(normal,
                                                                   r.getSource().subVec3(intersectPt).normalize(),
                                                                   lightVec,
                                                                   directLight[i][1]));
            }
        }
        
        if((objs[closestObj])->getReflectance().magnitude() > 0) {
            Ray reflected(worldIntersect, (objs[closestObj])->untransformVec4(r.getDirection().subVec3(normal.scale(2 * normal.dotProduct(r.getDirection()))), 0), 0.001, 100);
            color = color.addVec3(trace(reflected, dep - 1).multElements((objs[closestObj])->getReflectance()));
        }
    }
    return color;
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
    viewport.w = 400;
    viewport.h = 400;
    float depth = 5;
    Vec3 camPos(0, 0, 0);
    Vec3 ll(-1, -1, -1);
    Vec3 lr(1, -1, -1);
    Vec3 ul(-1, 1, -1);
    Vec3 ur(1, 1, -1);
    
    string filename = "objFiles/stepThree.obj";
    ifstream file(filename.c_str());
    vector<Vec3> vertices(0);
    vector<Sphere> spheres(0);
    vector<Triangle> triangles(0);
    vector<vector<int> > triSph(0);
    string line;
    
    if(file.is_open()) {
        while(!file.eof()) {
            getline(file, line);
            istringstream reader(line);
            string command;
            reader >> command;
            float p, q, r, x, y, z;
            if(command.compare("w") == 0) {
                reader >> x;
                viewport.w = x;
            }
            else if(command.compare("h") == 0) {
                reader >> y;
                viewport.h = y;
            }
            else if(command.compare("d") == 0) {
                reader >> r;
                depth = r;
            }
            else if(command.compare("eye") == 0) {
                reader >> x;
                reader >> y;
                reader >> z;
                camPos.setX(x);
                camPos.setY(y);
                camPos.setZ(z);
            }
            else if(command.compare("ll") == 0) {
                reader >> x;
                reader >> y;
                reader >> z;
                ll.setX(x);
                ll.setY(y);
                ll.setZ(z);
            }
            else if(command.compare("lr") == 0) {
                reader >> x;
                reader >> y;
                reader >> z;
                lr.setX(x);
                lr.setY(y);
                lr.setZ(z);
            }
            else if(command.compare("ul") == 0) {
                reader >> x;
                reader >> y;
                reader >> z;
                ul.setX(x);
                ul.setY(y);
                ul.setZ(z);
            }
            else if(command.compare("ur") == 0) {
                reader >> x;
                reader >> y;
                reader >> z;
                ur.setX(x);
                ur.setY(y);
                ur.setZ(z);
            }
            else if(command.compare("v") == 0) {
                reader >> x;
                reader >> y;
                reader >> z;
                vertices.push_back(Vec3(x, y, z));
            }
            else if(command.compare("f") == 0) {
                reader >> x;
                reader >> y;
                reader >> z;
                triangles.push_back(Triangle(vertices[x - 1], vertices[y - 1], vertices[z - 1]));
                vector<int> v(2);
                v[0] = 0;
                v[1] = triangles.size() - 1;
                triSph.push_back(v);
            }
            else if(command.compare("s") == 0) {
                reader >> x;
                reader >> y;
                reader >> z;
                reader >> r;
                spheres.push_back(Sphere(x, y, z, r));
                vector<int> v(2);
                v[0] = 1;
                v[1] = spheres.size() - 1;
                triSph.push_back(v);
                spheres[v[1]].setTranslater(x, y, z);
                spheres[v[1]].setScaler(r, r, r);
            }
            else if(command.compare("kd") == 0) {
                reader >> p;
                reader >> x;
                reader >> y;
                reader >> z;
                if(triSph[p - 1][0] == 1) {
                    spheres[triSph[p - 1][1]].setDiffuseRGB(x, y, z);
                }
                else {
                    triangles[triSph[p - 1][1]].setDiffuseRGB(x, y, z);
                }
            }
            else if(command.compare("ks") == 0) {
                reader >> p;
                reader >> x;
                reader >> y;
                reader >> z;
                reader >> r;
                if(triSph[p - 1][0] == 1) {
                    spheres[triSph[p - 1][1]].setSpecularRGB(x, y, z);
                    spheres[triSph[p - 1][1]].setSpecIntensity(r);
                }
                else {
                    triangles[triSph[p - 1][1]].setSpecularRGB(x, y, z);
                    triangles[triSph[p - 1][1]].setSpecIntensity(r);
                }
            }
            else if(command.compare("ka") == 0) {
                reader >> p;
                reader >> x;
                reader >> y;
                reader >> z;
                if(triSph[p - 1][0] == 1) {
                    spheres[triSph[p - 1][1]].setAmbient(x, y, z);
                }
                else {
                    triangles[triSph[p - 1][1]].setAmbient(x, y, z);
                }
            }
            else if(command.compare("kr") == 0) {
                reader >> p;
                reader >> x;
                reader >> y;
                reader >> z;
                if(triSph[p - 1][0] == 1) {
                    spheres[triSph[p - 1][1]].setReflectance(x, y, z);
                }
                else {
                    triangles[triSph[p - 1][1]].setReflectance(x, y, z);
                }
            }
            else if(command.compare("pl") == 0) {
                reader >> x;
                reader >> y;
                reader >> z;
                reader >> p;
                reader >> q;
                reader >> r;
                Vec3 xyz(x, y, z);
                Vec3 rgb(p, q, r);
                vector<Vec3> vpl(2);
                vpl[0] = xyz;
                vpl[1] = rgb;
                pointLight.push_back(vpl);
            }
            else if(command.compare("dl") == 0) {
                reader >> x;
                reader >> y;
                reader >> z;
                reader >> p;
                reader >> q;
                reader >> r;
                Vec3 xyz(-x, -y, -z);
                Vec3 rgb(p, q, r);
                vector<Vec3> vdl(2);
                vdl[0] = xyz;
                vdl[1] = rgb;
                directLight.push_back(vdl);
            }
            else if(command.compare("sca") == 0) {
                reader >> p;
                reader >> x;
                reader >> y;
                reader >> z;
                if(triSph[p - 1][0] == 1) {
                    spheres[triSph[p - 1][1]].setScaler(x, y, z);
                }
                else {
                    triangles[triSph[p - 1][1]].setScaler(x, y, z);
                }
            }
            else if(command.compare("rot") == 0) {
                reader >> p;
                reader >> x;
                reader >> y;
                reader >> z;
                if(triSph[p - 1][0] == 1) {
                    spheres[triSph[p - 1][1]].setRotater(x, y, z);
                }
                else {
                    triangles[triSph[p - 1][1]].setRotater(x, y, z);
                }
            }
        }
        file.close();
    }
    for(int i = 0; i < spheres.size(); i++) {
        spheres[i].setTransformer();
        objs.push_back(&spheres[i]);
    }
    for(int i = 0; i < triangles.size(); i++) {
        triangles[i].setTransformer();
        objs.push_back(&triangles[i]);
    }
    
    for(int i = 0; i < viewport.w; i++) {
        vector<Pixel> row(viewport.h);
        for(int j = 0; j < viewport.h; j++) {
            row[j] = Pixel(0, 0, 0);
        }
        pixels.push_back(row);
    }
    Camera c(camPos, viewport.w, viewport.h, ll, lr, ul, ur);
    
    
    for(int x = 0; x < viewport.w; x++) {
        for(int y = 0; y < viewport.h; y++) {
            Ray r = c.generateViewingRay(x, y, 0.001, 100);
            int closestObj = -1;
            float minT = 99999;
            for(int i = 0; i < objs.size(); i++) {
                Ray newR((objs[i])->transformVec4(r.getSource(), 1), (objs[i])->transformVec4(r.getDirection(), 0), 0, 100);
                float t = (objs[i])->intersection(newR);
                if(t != -1 && minT >= t) {
                    closestObj = i;
                    minT = t;
                }
            }
            if(closestObj != -1) {
                pixels[x][y].setColor((objs[closestObj])->getAmbient());
            }
            pixels[x][y].setColor(pixels[x][y].getColor().addVec3(trace(r, depth)));
        }
    }
    writePNGimage("out");
    return 0;
}