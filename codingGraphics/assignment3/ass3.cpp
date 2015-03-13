//
//  ass3.cpp
//
//
//  Created by Akhil Batra on 3/27/14.
//
//

#include "ass3.h"
#include "src/triangle.h"
#include "src/patch.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
//#include <OpenGL/gl.h>


//****************************************************
// Some Classes / Structs
//****************************************************
struct Viewport
{
    int w, h; // width and height
};

struct PatchInfo
{
    vector<Vec3> points;
    map<Vec3, Vec3> normals;
    vector<Triangle> tris;
    Patch surface;
};

struct Object
{
    string name;
    vector<PatchInfo> patches;
    Vec3 translate = Vec3();
    Vec3 rotate = Vec3();
    float scale = 1;
    float subDivParam = 1;
    float numDivs = 1;
    bool wire = false;
    bool hidden = false;
    bool frame = false;
    bool adaptive = false;
    bool bezier = true;
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess = 0;
    float emmision[4];
    //    bool smooth = true;
    //    int shadeModel = GL_SMOOTH;
};

//****************************************************
// Global Variables
//****************************************************
Viewport viewport;
vector<Object> objects;
int selectedObj = 0;
bool smooth = true;


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void resize(int w, int h)
{
    viewport.w = w;
    viewport.h = h;
    
    glViewport (0,0,viewport.w,viewport.h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport.w, 0, viewport.h, 0, (viewport.w + viewport.h) / 2.0f);
}

void drawQuads(Object obj)
{
    glBegin(GL_QUADS);
    for(PatchInfo &p : obj.patches)
    {
        for(int i = 0; i < obj.numDivs - 1; i++)
        {
            for(int j = 0; j < obj.numDivs - 1; j++)
            {
                Vec3 bottomLeft = p.points[i * obj.numDivs + j];
                Vec3 bottomRight = p.points[i * obj.numDivs + j + 1];
                Vec3 topLeft = p.points[i * obj.numDivs + j + obj.numDivs];
                Vec3 topRight = p.points[i * obj.numDivs + j + obj.numDivs + 1];
                
                Vec3 normalBL = p.normals[bottomLeft];
                Vec3 normalBR = p.normals[bottomRight];
                Vec3 normalTL = p.normals[topLeft];
                Vec3 normalTR = p.normals[topRight];
                
                
                glNormal3f(normalBL.getX(), normalBL.getY(), normalBL.getZ());
                glVertex3f(bottomLeft.getX(), bottomLeft.getY(), bottomLeft.getZ());
                
                glNormal3f(normalBR.getX(), normalBR.getY(), normalBR.getZ());
                glVertex3f(bottomRight.getX(), bottomRight.getY(), bottomRight.getZ());
                
                glNormal3f(normalTR.getX(), normalTR.getY(), normalTR.getZ());
                glVertex3f(topRight.getX(), topRight.getY(), topRight.getZ());
                
                glNormal3f(normalTL.getX(), normalTL.getY(), normalTL.getZ());
                glVertex3f(topLeft.getX(), topLeft.getY(), topLeft.getZ());
            }
        }
    }
    glEnd();
}

void drawTriags(Object obj)
{
    glBegin(GL_TRIANGLES);
    for(PatchInfo &p : obj.patches)
    {
        for(Triangle &t : p.tris)
        {
            glNormal3f(t[0].normal.getX(), t[0].normal.getY(), t[0].normal.getZ());
            glVertex3f(t[0].point.getX(), t[0].point.getY(), t[0].point.getZ());
            
            glNormal3f(t[1].normal.getX(), t[1].normal.getY(), t[1].normal.getZ());
            glVertex3f(t[1].point.getX(), t[1].point.getY(), t[1].point.getZ());
            
            glNormal3f(t[2].normal.getX(), t[2].normal.getY(), t[2].normal.getZ());
            glVertex3f(t[2].point.getX(), t[2].point.getY(), t[2].point.getZ());
        }
    }
    glEnd();
}

void draw(Object obj)
{
    if(obj.adaptive)
    {
        drawTriags(obj);
    }
    else
    {
        drawQuads(obj);
    }
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearStencil(0);
    glLoadIdentity();
    
    
    glScalef(objects[0].scale, objects[0].scale, objects[0].scale);
    glTranslatef(objects[0].translate.getX(), objects[0].translate.getY(), 0);
    glRotatef(objects[0].rotate.getY(), 0, 1, 0);
    glRotatef(objects[0].rotate.getX(), 1, 0, 0);
    
    int index = 0;
    for(Object &obj : objects)
    {
        glPushMatrix();
        glStencilFunc(GL_ALWAYS, index, -1);
        index++;
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, obj.ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, obj.diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obj.specular);
        glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, obj.shininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, obj.emmision);
        
        //        glShadeModel(obj.shadeModel);
        glScalef(obj.scale, obj.scale, obj.scale);
        glTranslatef(obj.translate.getX(), obj.translate.getY(), 0);
        glRotatef(obj.rotate.getY(), 0, 1, 0);
        glRotatef(obj.rotate.getX(), 1, 0, 0);
        
        if(obj.frame)
        {
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor3f(.25, .25, .25);
            draw(obj);
            
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0, 1.0);
            glEnable(GL_LIGHTING);
            draw(obj);
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
        else if(obj.hidden)
        {
            glDisable(GL_LIGHTING);
            
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor3f(1, 1, 1);
            draw(obj);
            
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0, 1.0);
            glColor3f(0, 0, 0);
            draw(obj);
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
        else
        {
            if(obj.wire)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
                glDisable(GL_LIGHTING);
                glColor3f(1, 1, 1);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
                glEnable(GL_LIGHTING);
            }
            draw(obj);
        }
        glPopMatrix();
    }
    
    glFlush();
    glutSwapBuffers();
}


void subdivide(PatchInfo *pInfo, Triangle t, float error)
{
    Edges edg = t.checkFlatness(error);
    if(!edg.oneTwo && !edg.twoThree && !edg.threeOne)
    {
        subdivide(pInfo, Triangle(t.getPatch(), t[0], edg.mid12, edg.mid31), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[1], edg.mid12, edg.mid23), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[2], edg.mid23, edg.mid31), error);
        subdivide(pInfo, Triangle(t.getPatch(), edg.mid12, edg.mid23, edg.mid31), error);
    }
    else if(!edg.twoThree && !edg.threeOne)
    {
        subdivide(pInfo, Triangle(t.getPatch(), t[0], t[1], edg.mid31), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[1], edg.mid23, edg.mid31), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[2], edg.mid23, edg.mid31), error);
    }
    else if(!edg.oneTwo && !edg.threeOne)
    {
        subdivide(pInfo, Triangle(t.getPatch(), t[1], t[2], edg.mid12), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[0], edg.mid12, edg.mid31), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[2], edg.mid12, edg.mid31), error);
    }
    else if(!edg.oneTwo && !edg.twoThree)
    {
        subdivide(pInfo, Triangle(t.getPatch(), t[0], t[2], edg.mid12), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[1], edg.mid12, edg.mid23), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[2], edg.mid12, edg.mid23), error);
    }
    else if(!edg.oneTwo)
    {
        subdivide(pInfo, Triangle(t.getPatch(), t[0], t[2], edg.mid12), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[1], t[2], edg.mid12), error);
    }
    else if(!edg.twoThree)
    {
        subdivide(pInfo, Triangle(t.getPatch(), t[0], t[1], edg.mid23), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[0], t[2], edg.mid23), error);
    }
    else if(!edg.threeOne)
    {
        subdivide(pInfo, Triangle(t.getPatch(), t[0], t[1], edg.mid31), error);
        subdivide(pInfo, Triangle(t.getPatch(), t[1], t[2], edg.mid31), error);
    }
    else
    {
        pInfo->tris.push_back(t);
    }
}


void generateUniform(Object *obj, Patch p)
{
    PatchInfo info;
    info.surface = Patch(p);
    for (int iu = 0; iu < obj->numDivs; iu++)
    {
        float u = iu * obj->subDivParam;
        for (int iv = 0; iv < obj->numDivs; iv++)
        {
            float v = iv * obj->subDivParam;
            pointNormalPair pnp = p.bezPatchInterpolation(u, v);
            info.normals[pnp.point] = pnp.normal;
            info.points.push_back(pnp.point);
        }
    }
    obj->patches.push_back(info);
}


void generateAdaptive(Object *obj, Patch p)
{
    PatchInfo info;
    info.surface = Patch(p);
    
    pointNormalPair pnpBL = p.bezPatchInterpolation(0, 0);
    pointNormalPair pnpBR = p.bezPatchInterpolation(1, 0);
    pointNormalPair pnpTL = p.bezPatchInterpolation(0, 1);
    pointNormalPair pnpTR = p.bezPatchInterpolation(1, 1);
    
    Vertex bottomLeft;
    bottomLeft.point = Vec3(pnpBL.point);
    bottomLeft.normal = Vec3(pnpBL.normal);
    bottomLeft.u = 0;
    bottomLeft.v = 0;
    
    Vertex bottomRight;
    bottomRight.point = Vec3(pnpBR.point);
    bottomRight.normal = Vec3(pnpBR.normal);
    bottomRight.u = 1;
    bottomRight.v = 0;
    
    Vertex topLeft;
    topLeft.point = Vec3(pnpTL.point);
    topLeft.normal = Vec3(pnpTL.normal);
    topLeft.u = 0;
    topLeft.v = 1;
    
    Vertex topRight;
    topRight.point = Vec3(pnpTR.point);
    topRight.normal = Vec3(pnpTR.normal);
    topRight.u = 1;
    topRight.v = 1;
    
    subdivide(&info, Triangle(&p, bottomLeft, topLeft, bottomRight), obj->subDivParam);
    subdivide(&info, Triangle(&p, topLeft, bottomRight, topRight), obj->subDivParam);
    obj->patches.push_back(info);
}


void generateObject(Object *obj)
{
    string filename = string("inputFiles/") + obj->name;
    ifstream file(filename.c_str());
    string line;
    
    float largestCoordinate = 1;
    PatchInfo pInfo;
    pInfo.surface = Patch();
    vector<Vertex> verts;
    int index = 0;
    if(file.is_open())
    {
        while(!file.eof())
        {
            getline(file, line);
            istringstream reader(line);
            string command;
            float x, y, z, w;
            reader >> command;
            
            if(command.compare("v") == 0)
            {
                reader >> x;
                reader >> y;
                reader >> z;
                Vertex v;
                v.point = Vec3(x, y, z);
                v.normal = Vec3(0, 0, 1);
                verts.push_back(v);
                largestCoordinate = max(largestCoordinate, max(max(abs(x), abs(y)), abs(z)));
            }
            else if(command.compare("vn") == 0)
            {
                reader >> x;
                reader >> y;
                reader >> z;
                verts[index].normal = Vec3(x, y, z).normalize();
                index++;
            }
            else if(command.compare("f") == 0)
            {
                reader >> x;
                reader >> y;
                reader >> z;
                reader >> w;
                if(w > 0)
                {
                    pInfo.tris.push_back(Triangle(&pInfo.surface, verts[x - 1], verts[z - 1], verts[w - 1]));
                }
                pInfo.tris.push_back(Triangle(&pInfo.surface, verts[x - 1], verts[y - 1], verts[z - 1]));
            }
            else if(command.compare("ka") == 0)
            {
                reader >> obj->ambient[0];
                reader >> obj->ambient[1];
                reader >> obj->ambient[2];
                obj->ambient[3] = 1;
            }
            else if(command.compare("kd") == 0)
            {
                reader >> obj->diffuse[0];
                reader >> obj->diffuse[1];
                reader >> obj->diffuse[2];
                obj->diffuse[3] = 1;
            }
            else if(command.compare("ks") == 0)
            {
                reader >> obj->specular[0];
                reader >> obj->specular[1];
                reader >> obj->specular[2];
                reader >> obj->shininess;
                obj->specular[3] = 1;
            }
        }
        file.close();
    }
    
    obj->scale = 1 / largestCoordinate;
    obj->patches.push_back(pInfo);
}

//******************************************************
// functions that runs whenever a key is pressed on the keyboard
//******************************************************
void normalKeyPressed(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ' ':
            exit(0);
            break;
        case 'f':
            if(selectedObj == 0)
            {
                bool newFrame = !objects[selectedObj].frame;
                for(Object &obj : objects)
                {
                    obj.frame = newFrame;
                    obj.hidden = false;
                    obj.wire = false;
                }
            }
            else
            {
                objects[selectedObj].frame = !objects[selectedObj].frame;
            }
            objects[selectedObj].hidden = false;
            objects[selectedObj].wire = false;
            break;
        case 'h':
            if(selectedObj == 0)
            {
                bool newHidden = !objects[selectedObj].hidden;
                for(Object &obj : objects)
                {
                    obj.hidden = newHidden;
                    obj.frame = false;
                    obj.wire = false;
                }
            }
            else
            {
                objects[selectedObj].hidden = !objects[selectedObj].hidden;
            }
            objects[selectedObj].frame = false;
            objects[selectedObj].wire = false;
            break;
        case 's':
            //            if(selectedObj == 0)
            //            {
            //                bool newSmooth = !objects[selectedObj].smooth;
            //                for(Object &obj : objects)
            //                {
            //                    obj.smooth = newSmooth;
            //                    if(obj.smooth)
            //                    {
            //                        obj.shadeModel = GL_SMOOTH;
            //                    }
            //                    else
            //                    {
            //                        obj.shadeModel = GL_FLAT;
            //                    }
            //                }
            //            }
            //            else
            //            {
            //                objects[selectedObj].smooth = !objects[selectedObj].smooth;
            //            }
            smooth = !smooth;
            if(smooth)
            {
                glShadeModel(GL_SMOOTH);
            }
            else
            {
                glShadeModel(GL_FLAT);
            }
            break;
        case 'w':
            if(selectedObj == 0)
            {
                bool newWire = !objects[selectedObj].wire;
                for(Object &obj : objects)
                {
                    obj.wire = newWire;
                    obj.frame = false;
                    obj.hidden = false;
                }
            }
            else
            {
                objects[selectedObj].wire = !objects[selectedObj].wire;
            }
            objects[selectedObj].frame = false;
            objects[selectedObj].hidden = false;
            break;
        case '=':
            objects[selectedObj].scale += .01;
            break;
        case '+':
            objects[selectedObj].scale += .01;
            break;
        case '-':
            objects[selectedObj].scale -= .01;
            break;
    }
    drawScene();
}

void specialKeyPressed(int key, int x, int y)
{
    bool shift = false;
    if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
    {
        shift = true;
    }
    switch (key)
	{
		case GLUT_KEY_LEFT:
            if(shift)
            {
                objects[selectedObj].translate.setX(objects[selectedObj].translate.getX() - .1);
            }
            else
            {
                objects[selectedObj].rotate.setY(objects[selectedObj].rotate.getY() + 5);
			}
            break;
		case GLUT_KEY_RIGHT:
            if(shift)
            {
                objects[selectedObj].translate.setX(objects[selectedObj].translate.getX() + .1);
            }
            else
            {
                objects[selectedObj].rotate.setY(objects[selectedObj].rotate.getY() - 5);
			}
			break;
		case GLUT_KEY_DOWN:
            if(shift)
            {
                objects[selectedObj].translate.setY(objects[selectedObj].translate.getY() - .1);
            }
            else
            {
                objects[selectedObj].rotate.setX(objects[selectedObj].rotate.getX() - 5);
			}
			break;
		case GLUT_KEY_UP:
            if(shift)
            {
                objects[selectedObj].translate.setY(objects[selectedObj].translate.getY() + .1);
            }
            else
            {
                objects[selectedObj].rotate.setX(objects[selectedObj].rotate.getX() + 5);
			}
			break;
	}
    drawScene();
}

void mouseClicked(int button, int state, int x, int y)
{
    if(state != GLUT_DOWN)
    {
        return;
    }
    
    objects[selectedObj].emmision[0] = 0.0;
    objects[selectedObj].emmision[1] = 0.0;
    objects[selectedObj].emmision[2] = 0.0;
    
    glReadPixels(x, viewport.h - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &selectedObj);
    
    objects[selectedObj].emmision[0] = 0.1;
    objects[selectedObj].emmision[1] = 0.1;
    objects[selectedObj].emmision[2] = 0.1;
    
    drawScene();
}


//****************************************************
// Simple init function
//****************************************************
void initScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_SMOOTH);
    glEnable(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(normalKeyPressed);
    glutSpecialFunc(specialKeyPressed);
    glutMouseFunc(mouseClicked);
    
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 0.0, 0.5, 0.5, 1.0 };
    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_position[] = { 1.0, 0.0, 0.0, 0.0 };
    GLfloat lightDirection[] = { 0.0f, 0.0f, -3.0f };
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDirection);
    glEnable(GL_LIGHT1);
}


//****************************************************
int main(int argc, char *argv[])
{
    if(argc < 2 || argc > 6)
    {
        cout << "NOT THE CORRECT NUMBER OF ARGUMENTS!!!!!!" << endl << "Please refer to README file for more informatioin about input" << endl;
        return 0;
    }
    
    bool outputOBJ = false;
    string outputName;
    
    
    Object o;
    o.scale = 1;
    objects.push_back(o);
    float epsilon = 0;
    
    if(argc == 2)
    {
        string filename = string(argv[1]);
        if(filename.compare(filename.length() - 4, 4, ".obj") == 0)
        {
            Object obj;
            obj.name = filename;
            obj.bezier = false;
            obj.adaptive = true;
            objects.push_back(obj);
        }
        else
        {
            ifstream file((string("scenes/") + filename).c_str());
            string line;
            if(file.is_open())
            {
                while(!file.eof())
                {
                    Object obj;
                    getline(file, line);
                    istringstream reader(line);
                    string adapt;
                    reader >> obj.name;
                    if(obj.name.compare(obj.name.length() - 4, 4, ".obj") == 0)
                    {
                        obj.bezier = false;
                        obj.adaptive = true;
                    }
                    else
                    {
                        reader >> obj.subDivParam;
                        reader >> adapt;
                        if(string(adapt).compare("-a") == 0)
                        {
                            obj.adaptive = true;
                        }
                        else
                        {
                            obj.adaptive = false;
                        }
                    }
                    objects.push_back(obj);
                }
                file.close();
            }
        }

    }
    else if(argc == 3)
    {
        Object o;
        o.name = string(argv[1]);
        istringstream reader(argv[2]);
        reader >> o.subDivParam;
        objects.push_back(o);
    }
    else if(argc == 4)
    {
        Object o;
        o.name = string(argv[1]);
        istringstream reader(argv[2]);
        reader >> o.subDivParam;
        if(string(argv[3]).compare("-a") == 0)
        {
            o.adaptive = true;
        }
        objects.push_back(o);
    }
    else if(argc == 5)
    {
        Object o;
        o.name = string(argv[1]);
        istringstream reader(argv[2]);
        reader >> o.subDivParam;
        if(string(argv[3]).compare("-o") == 0)
        {
            outputOBJ = true;
            outputName = string(argv[4]);
        }
        objects.push_back(o);
    }
    else if(argc == 6)
    {
        Object o;
        o.name = string(argv[1]);
        istringstream reader(argv[2]);
        reader >> o.subDivParam;
        if(string(argv[3]).compare("-a") == 0 || string(argv[5]).compare("-a") == 0)
        {
            o.adaptive = true;
        }
        if(string(argv[3]).compare("-o") == 0)
        {
            outputOBJ = true;
            outputName = string(argv[4]);
        }
        else if(string(argv[4]).compare("-o") == 0)
        {
            outputOBJ = true;
            outputName = string(argv[5]);
        }
        objects.push_back(o);
    }
    else
    {
        cout << "I don't know what you inputted but it was not right...refer to README for more info" << endl;
        return 0;
    }
    
    for(Object &obj : objects)
    {
        if(obj.bezier)
        {
            obj.numDivs = (1 + epsilon) / obj.subDivParam + 1;
            float largestCoordinate = 1;
            
            string filename = string("inputFiles/") + obj.name;
            ifstream file(filename.c_str());
            string line;
            
            if(file.is_open())
            {
                if(!file.eof())
                {
                    getline(file, line);
                    istringstream reader(line);
                    int numPatches;
                    reader >> numPatches;
                    
                    float x, y, z;
                    for(int i = 0; i < numPatches; i++)
                    {
                        Patch surf = Patch();
                        for(int j = 0; j < 4; j++)
                        {
                            getline(file, line);
                            istringstream reader(line);
                            Curve c = Curve();
                            for(int k = 0; k < 4; k++)
                            {
                                reader >> x;
                                reader >> y;
                                reader >> z;
                                c[k] = Vec3(x, y, z);
                                largestCoordinate = max(largestCoordinate, max(max(abs(x), abs(y)), abs(z)));
                            }
                            surf[j] = Curve(c);
                        }
                        
                        surf.assignVcurves();
                        
                        if(obj.adaptive)
                        {
                            generateAdaptive(&obj, surf);
                        }
                        else
                        {
                            generateUniform(&obj, surf);
                        }
                        getline(file, line);
                    }
                }
                file.close();
            }
            obj.scale = 1 / largestCoordinate;
            
            obj.ambient[0] = 0.5 * obj.scale;
            obj.ambient[1] = 1 * obj.scale;
            obj.ambient[2] = 0.8 * obj.scale;
            obj.ambient[3] = 1.0;
            
            obj.diffuse[0] = 0.1 * obj.scale;
            obj.diffuse[1] = 1 * obj.scale;
            obj.diffuse[2] = 0.8 * obj.scale;
            obj.diffuse[3] = 1.0;
            
            obj.specular[0] = 0.3 * obj.scale;
            obj.specular[1] = 0.8 * obj.scale;
            obj.specular[2] = 0.7 * obj.scale;
            obj.specular[3] = 1.0;
            obj.shininess = 5.0;
            
            obj.emmision[0] = 0.0;
            obj.emmision[1] = 0.0;
            obj.emmision[2] = 0.0;
            obj.emmision[3] = 0.0;
        }
        else
        {
            generateObject(&obj);
        }
    }
    
    if(outputOBJ)
    {
        for(Object &obj : objects)
        {
            ofstream file;
            file.open(string("outputFiles/") + outputName);
            
            file << "\n#" << obj.name << "\n\n\n\n";
            
            if(obj.adaptive)
            {
                map<Vec3, int> nums;
                int index = 1;
                for(PatchInfo &p : obj.patches)
                {
                    for(Triangle &t : p.tris)
                    {
                        if(nums[t[0].point] == 0)
                        {
                            nums[t[0].point] = index;
                            file << "v " << t[0].point.getX() << " " << t[0].point.getY() << " " << t[0].point.getZ() << "\n";
                            file << "vn " << t[0].normal.getX() << " " << t[0].normal.getY() << " " << t[0].normal.getZ() << "\n";
                            index++;
                        }
                        if(nums[t[1].point] == 0)
                        {
                            nums[t[1].point] = index;
                            file << "v " << t[1].point.getX() << " " << t[1].point.getY() << " " << t[1].point.getZ() << "\n";
                            file << "vn " << t[1].normal.getX() << " " << t[1].normal.getY() << " " << t[1].normal.getZ() << "\n";
                            index++;
                        }
                        if(nums[t[2].point] == 0)
                        {
                            nums[t[2].point] = index;
                            file << "v " << t[2].point.getX() << " " << t[2].point.getY() << " " << t[2].point.getZ() << "\n";
                            file << "vn " << t[2].normal.getX() << " " << t[2].normal.getY() << " " << t[2].normal.getZ() << "\n";
                            index++;
                        }
                        file << "f " << nums[t[0].point] << " " << nums[t[1].point] << " " << nums[t[2].point] << "\n\n";
                    }
                }
            }
            else
            {
                for(PatchInfo &p : obj.patches)
                {
                    for(int i = 0; i < obj.numDivs - 1; i++)
                    {
                        for(int j = 0; j < obj.numDivs - 1; j++)
                        {
                            Vec3 bottomLeft = p.points[i * obj.numDivs + j];
                            Vec3 bottomRight = p.points[i * obj.numDivs + j + 1];
                            Vec3 topLeft = p.points[i * obj.numDivs + j + obj.numDivs];
                            Vec3 topRight = p.points[i * obj.numDivs + j + obj.numDivs + 1];
                            
                            Vec3 normalBL = p.normals[bottomLeft];
                            Vec3 normalBR = p.normals[bottomRight];
                            Vec3 normalTL = p.normals[topLeft];
                            Vec3 normalTR = p.normals[topRight];
                            
                            
                            file << "v " << bottomLeft.getX() << " " << bottomLeft.getY() << " " << bottomLeft.getZ() << "\n";
                            file << "v " << bottomRight.getX() << " " << bottomRight.getY() << " " << bottomRight.getZ() << "\n";
                            file << "v " << topLeft.getX() << " " << topLeft.getY() << " " << topLeft.getZ() << "\n";
                            file << "v " << topRight.getX() << " " << topRight.getY() << " " << topRight.getZ() << "\n";
                            
                            file << "vn " << normalBL.getX() << " " << normalBL.getY() << " " << normalBL.getZ() << "\n";
                            file << "vn " << normalBR.getX() << " " << normalBR.getY() << " " << normalBR.getZ() << "\n";
                            file << "vn " << normalTL.getX() << " " << normalTL.getY() << " " << normalTL.getZ() << "\n";
                            file << "vn " << normalTR.getX() << " " << normalTR.getY() << " " << normalTR.getZ() << "\n";
                            
                            file << "f " << (i * obj.numDivs + j) << " " << (i * obj.numDivs + j + 1) << " " << (i * obj.numDivs + j + obj.numDivs) << " " << (i * obj.numDivs + j + obj.numDivs + 1) << "\n\n";
                        }
                    }
                }

            }
            
            file << "\nka " << obj.ambient[0] << " " << obj.ambient[1] << " " << obj.ambient[2] << "\n";
            file << "kd " << obj.diffuse[0] << " " << obj.diffuse[1] << " " << obj.diffuse[2] << "\n";
            file << "ks " << obj.specular[0] << " " << obj.specular[1] << " " << obj.specular[2] << " " << obj.shininess << "\n\n";
            
            file.close();
        }
    }
    else
    {
        glutInit(&argc, argv);
        
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
        
        viewport.w = 1000;
        viewport.h = 1000;
        
        glutInitWindowSize(viewport.w, viewport.h);
        glutInitWindowPosition(0,0);
        glutCreateWindow(argv[0]);
        initScene();
        glutMainLoop();
    }
    return 0;
}