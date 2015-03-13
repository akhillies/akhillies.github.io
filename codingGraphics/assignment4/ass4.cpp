//
//  ass4.cpp
//  
//
//  Created by Akhil Batra on 5/5/14.
//
//

#include "Eigen/Dense"
#include "src/joint.h"
#include "src/structs.h"
#include "src/system.h"
#include <string>
#include <fstream>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>

using namespace::std;
using namespace::Eigen;

float PI = 3.14159265359;

Viewport viewport;
System sys;
vector<Vector3f> goals;
vector<Vector3f> positions;
int goalNum = 0;
Vector3f worldRotation(0, 0, 0);
Vector3f worldTranslation(0, 0, 0);
float worldScaling = 1;
int iterations = 0;
int setsOfOneK = 0;
float error = .1;
bool freeze = false;
bool dontDrawJoints = false;
bool dontDrawPath = true;
bool dontDrawTrail = true;
bool dontDrawGoal = false;
bool dontDrawGoalPath = true;
bool input = false;
bool mouseDwn = false;
bool dontDrawBase = false;
Vector3f inputGoal(0, 0, 0);
Vector3f mousePos(0, 0, 0);

void resize(int w, int h)
{
    viewport.w = w;
    viewport.h = h;
    
    glViewport (0,0,viewport.w,viewport.h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, viewport.w * 1.0 / viewport.h, .1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void changeInput()
{
    string in;
    getline(cin, in);
    cout << in.compare("done") << endl;
    if(in.compare("done") == 0)
    {
        input = false;
        freeze = false;
    }
    else
    {
        istringstream reader(in);
        float coordinate;
        reader >> coordinate;
        inputGoal(0) = coordinate;
        reader >> coordinate;
        inputGoal(1) = coordinate;
        reader >> coordinate;
        inputGoal(2) = coordinate;
    }
}

void animate(Vector3f goal)
{
    bool finished = sys.applyIK(goal);
    if(finished)
    {
        cout << "it took " << iterations + setsOfOneK * 100 << " iterations to reach the goal of " << goal(0) << ", " << goal(1) << ", " << goal(2) << " with error of " << (sys.getEndEffector() - goal).norm() << endl;
        goalNum = (goalNum + 1) % goals.size();
        iterations = 0;
        setsOfOneK = 0;
        sys.setError(error);
        if(input)
        {
            cout << "Custom goal has been reached, type 'done' to go back to file path or enter a new input:" << endl;
            changeInput();
        }
    }
    else
    {
        iterations++;
        if(iterations >= 100)
        {
//            goalNum = (goalNum + 1) % goals.size();
            sys.setError(sys.getError() * 2);
            iterations = 0;
            setsOfOneK++;
        }
    }
}

void ik_it_UP()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslatef(worldTranslation(0), worldTranslation(1), -15);
    glRotatef(worldRotation(0), 0, 1, 0);
    glRotatef(worldRotation(1), 1, 0, 0);
    glScalef(worldScaling, worldScaling, worldScaling);
    
    if(!dontDrawBase)
    {
        glPushMatrix();
        //    glTranslatef(0, 0, -sys[0].getLength() / 2);
        GLUquadric* qud = gluNewQuadric();
        gluQuadricNormals(qud, GL_FLAT);
        gluQuadricDrawStyle(qud, GLU_FILL);
        //    gluSphere(qud, sys[0].getLength() / 2, 3, 2);
        gluDisk(qud, fmin(sys[sys.numJoints() - 1].getLength() / 4.0, .75), 1, 3, 5);
        glPopMatrix();
    }
    
    glPushMatrix();
//    cout << "lkjh" << endl;
//    Vector3f goal = goals[3];
    Vector3f goal;
    if(input)
    {
        goal = inputGoal;
    }
    else if(mouseDwn)
    {
        goal = mousePos;
    }
    else
    {
//        goal = Vector3f(2, 1, 1);
//        goal = goals[0];
        goal = goals[goalNum];
    }
    
    if(!freeze)
    {
        positions.push_back(sys.getEndEffector());
        animate(goal);
    }
//    cout << "asf: " << endl;
    if(!dontDrawJoints)
    {
        sys.drawJoints();
    }
//    cout << "asdf" << endl;
    
    float transparent[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    if(!dontDrawGoal)
    {
        glPushMatrix();
        GLUquadric* quad = gluNewQuadric();
        gluQuadricNormals(quad, GL_SMOOTH);
        gluQuadricDrawStyle(quad, GLU_LINE);
        
        glTranslatef(goal(0), goal(1), goal(2));
        
        transparent[1] = 0;
        transparent[2] = 0;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, transparent);
        glPushMatrix();
        glRotatef(iterations, 0, 0, 1);
        glScalef(1, 1, 1);
        gluSphere(quad, sys.getError(), 3, 3);
        glPopMatrix();
        
        transparent[1] = 1;
        transparent[2] = 0;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, transparent);
        glPushMatrix();
        glRotatef(iterations * 4, 1, 0, 0);
        glScalef(.75, .75, .75);
        gluSphere(quad, sys.getError(), 5, 5);
        glPopMatrix();
        
        transparent[1] = 0;
        transparent[2] = 0.25;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, transparent);
        glPushMatrix();
        glRotatef(iterations * 2, 0, 1, 0);
        glScalef(1, 1, 1);
        gluSphere(quad, sys.getError(), 4, 4);
        glPopMatrix();
        
        glPopMatrix();
    }
    
    if(!dontDrawPath || !dontDrawTrail)
    {
        glLineWidth(2);
        glBegin(GL_LINES);
        transparent[0] = 0;
        transparent[1] = 1;
        transparent[2] = .75;
        for(int i = 0; i < positions.size() - 1; i++)
        {
            if(!dontDrawTrail)
            {
                transparent[3] = (i - positions.size() / 5.0) / positions.size();
            }
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, transparent);
            
            glVertex3f(positions[i](0), positions[i](1), positions[i](2));
            glVertex3f(positions[i + 1](0), positions[i + 1](1), positions[i + 1](2));
        }
        glTranslatef(positions.back()(0), positions.back()(1), positions.back()(2));
        GLUquadric* quad = gluNewQuadric();
        gluQuadricNormals(quad, GL_SMOOTH);
        gluQuadricDrawStyle(quad, GLU_FILL);
        gluSphere(quad, .025, 32, 32);
        
        glEnd();
    }
    
    if(!dontDrawGoalPath)
    {
        glLineWidth(4);
        glBegin(GL_LINES);
        transparent[0] = 1;
        transparent[1] = 1;
        transparent[2] = 0;
        transparent[3] = 1;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, transparent);
        for(int i = 0; i < goals.size() - 1; i++)
        {
            glVertex3f(goals[i](0), goals[i](1), goals[i](2));
            glVertex3f(goals[i + 1](0), goals[i + 1](1), goals[i + 1](2));
        }
        glVertex3f(goals[goals.size() - 1](0), goals[goals.size() - 1](1), goals[goals.size() - 1](2));
        glVertex3f(goals[0](0), goals[0](1), goals[0](2));
        glEnd();
    }
    
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
//    glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
    switch(key)
    {
        case ' ':
            exit(0);
        case '=':
            worldScaling += .05;
            break;
        case '+':
            worldScaling += .05;
            break;
        case '-':
            worldScaling -= .05;
            break;
        case '_':
            worldScaling -= .05;
            break;
        case 'a':
            float l;
            cout << "Please enter the length of the new Joint:" << endl;
            cin >> l;
            sys.addJoint(Joint(l, Vector3f(0, 0, 0)));
            break;
        case 'b':
            dontDrawBase = !dontDrawBase;
            break;
        case 'e':
            positions.clear();
            break;
        case 'f':
            freeze = !freeze;
            break;
        case 'i':
            input = !input;
            if(input)
            {
                cout << "Please enter the goal of the System in the form of x y z (no commas or brackets, only spaces):" << endl;
                changeInput();
            }
            else
            {
                cout << "Going back to file path" << endl;
                inputGoal = Vector3f(0, 0, 0);
            }
            break;
        case 'g':
            dontDrawGoal = !dontDrawGoal;
            break;
        case 'j':
            dontDrawJoints = !dontDrawJoints;
            break;
        case 'l':
            dontDrawGoalPath = !dontDrawGoalPath;
            break;
        case 'p':
            dontDrawPath = !dontDrawPath;
            dontDrawTrail = true;
            break;
        case 'r':
            worldTranslation = Vector3f(0, 0, 0);
            worldRotation = Vector3f(0, 0, 0);
            worldScaling = 1;
            break;
        case 't':
            dontDrawTrail = !dontDrawTrail;
            dontDrawPath = true;
            break;
    }
    sys.drawJoints();
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
                worldTranslation(0) += .1;
            }
            else
            {
                worldRotation(0) += 5;
			}
            break;
		case GLUT_KEY_RIGHT:
            if(shift)
            {
                worldTranslation(0) -= .1;
            }
            else
            {
                worldRotation(0) -= 5;
			}
			break;
		case GLUT_KEY_DOWN:
            if(shift)
            {
                worldTranslation(1) -= .1;
            }
            else
            {
                worldRotation(1) -= 5;
			}
			break;
		case GLUT_KEY_UP:
            if(shift)
            {
                worldTranslation(1) += .1;
            }
            else
            {
                worldRotation(1) += 5;
			}
			break;
	}
    sys.drawJoints();
}

void mouseClicked(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseDwn = true;
        }
        else if(state == GLUT_UP)
        {
            mouseDwn = false;
        }
    }
}

void setMousePos(int x, int y)
{
    mousePos(0) = (x - viewport.w / 2.0) / viewport.w * 12;
    mousePos(1) = -(y - viewport.h / 2.0) / viewport.h * 12;
    mousePos(2) = 0;
}

void initScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glutDisplayFunc(ik_it_UP);
    glutIdleFunc(ik_it_UP);
//    glutTimerFunc(100, ik_it_UP, NULL);
    glutReshapeFunc(resize);
    glutSpecialFunc(specialKeyPressed);
    glutKeyboardFunc(keyPressed);
    glutMouseFunc(mouseClicked);
    glutMotionFunc(setMousePos);
    
    
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
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
    
    
    GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse1[] = { 1.0, 0.5, 0.5, 1.0 };
    GLfloat light_specular1[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_position1[] = { 0.0, 1.0, 0.0, 0.0 };
    GLfloat lightDirection1[] = { 0.0f, 0.0f, 3.0f };
    
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient1);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular1);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightDirection1);
    glEnable(GL_LIGHT2);
}

int main(int argc, char *argv[])
{
    Joint j, o, i, n, t, s, y, e, m, f, k, c, a, h, l, b;
    
    
    j = Joint(1, Vector3f(0, 0, 0));
    o = Joint(1, Vector3f(0, 0, PI / 2));
    i = Joint(1, Vector3f(3, 0, 0));
    n = Joint(1, Vector3f(.5, .2, 1));
    
    t = Joint(1, Vector3f(PI / 2, 0, 0));
    s = Joint(2, Vector3f(0, PI / 2, 0));
    y = Joint(3, Vector3f(0, -PI / 2, 0));
    e = Joint(4, Vector3f(0, -PI / 2, 0));
    m = Joint(4.5, Vector3f(0, -PI / 2, PI / 7));
    f = Joint(3.5, Vector3f(-PI / 2, PI / 2, -PI));
    k = Joint(2.5, Vector3f(0, 0, PI / 5));
    c = Joint(1.5, Vector3f(PI / 8, -PI / 3, PI / 6));
    
    a = Joint(4, Vector3f(0, 0, 0));
    h = Joint(2, Vector3f(0, 0, 0));
    l = Joint(3, Vector3f(0, 0, 0));
    b = Joint(.5, Vector3f(0, 0, 0));
    
    Joint jList[] = {j, o, i, n, t, s, y, e, m, f, k, c, a, h, l, b};
    
    bool premade = false;
    int numJoints = 0;
    
    for(int i = 2; i < argc; i++)
    {
        if(string(argv[i]).compare("-j") == 0)
        {
            premade = true;
            stringstream r(argv[i + 1]);
            r >> numJoints;
        }
        else if(string(argv[i]).compare("-e") == 0)
        {
            stringstream r(argv[i + 1]);
            r >> error;
        }
    }
    
    sys.setError(error);
    
    string filename = string("inputFiles/") + argv[1];
    ifstream file(filename.c_str());
    string line;
    
    if(file.is_open())
    {
        while(!file.eof())
        {
            getline(file, line);
            istringstream reader(line);
            string command;
            reader >> command;
            
            if(command.compare("v") == 0)
            {
                Vector3f v;
                float coordinate;
                
                reader >> coordinate;
                v(0) = coordinate;
                reader >> coordinate;
                v(1) = coordinate;
                reader >> coordinate;
                v(2) = coordinate;
                
                goals.push_back(v);
            }
            else if(command.compare("j") == 0)
            {
                float l;
                reader >> l;
                
                Vector3f v(0, 0, 0);
                float coordinate;
                reader >> coordinate;
                v(0) = coordinate;
                reader >> coordinate;
                v(1) = coordinate;
                reader >> coordinate;
                v(2) = coordinate;
                
                sys.addJoint(Joint(l, v));
            }
        }
        file.close();
    }
    
    if(premade)
    {
        for(int j = 0; j < numJoints; j++)
        {
            sys.addJoint(jList[j]);
        }
    }

//    Matrix3f qw;
//    qw << 0, 0, 1,   0, 1, 0,    -1, 0, 0;
//    EigenSolver<Matrix3f> es(qw);
//    cout << es.eigenvectors() << endl;
//    exit(0);
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    viewport.w = 1000;
    viewport.h = 1000;
    
    glutInitWindowSize(viewport.w, viewport.h);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    
    initScene();
    
    glutMainLoop();
    
    return 0;
}