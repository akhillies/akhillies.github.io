//
//  shape.h
//  
//
//  Created by Akhil Batra on 3/13/14.
//
//

#ifndef ____shape__
#define ____shape__

#include "ray.h"
#include "vec3.h"
#include "matrix.h"
#include <string>
#include <iostream>

class Shape {
private:
    Vec3 diffuseRGB, specRGB, ambientRGB, reflect, rotate, translate, scale;
    float specInt;
    Matrix trans;
    
public:
    // for calculating intersection of this shape and the given ray
    virtual float intersection(Ray r) = 0;
    
    // for testing purposes - converts the shape into a string containing its important information
    virtual string stringify() = 0;
    
    // calculates the rgb addition of the diffuse material with the given light
    virtual Vec3 calcDiffuse(Vec3 normal, Vec3 lightVec, Vec3 lightColor) = 0;
    
    // calculates the rgb addition of the specular material with the given light
    virtual Vec3 calcSpec(Vec3 normal, Vec3 eye, Vec3 lightVec, Vec3 lightColor) = 0;
    
    // returns the normal of this shape at the given point
    virtual Vec3 getNormal(Vec3 v) = 0;
    
    // returns the given world space point in the object space
    virtual Vec3 transformVec4(Vec3 v, float w) = 0;
    
    // returns the given object space point in the world space
    virtual Vec3 untransformVec4(Vec3 v, float w) = 0;
    
    // gets the ambient RGB for this material
    Vec3 getAmbient();
    
    // gets the diffuse RGB for material and phong shading
    Vec3 getDiffuseRGB();
    
    // gets the specular RGB for material and phong shading
    Vec3 getSpecularRGB();
    
    // gets the specular intensity used in phong shading
    float getSpecIntensity();
    
    // gets the reflectance of this shape
    Vec3 getReflectance();
    
    // gets rotation matrix
    Matrix getRotater();
    
    // gets the scale matrix
    Matrix getScaler();
    
    // gets the translation matrix
    Matrix getTranslater();
    
    // getter for the transformation matrix
    Matrix getTransformer();
    
    // sets the transformation matrix to be applied
    void setTransformer();
    
    // sets the ambient RGB for this material
    void setAmbient(float r, float g, float b);
    
    // sets the diffuse RGB for material and phong shading
    void setDiffuseRGB(float r, float g, float b);
    
    // sets the specular RGB for material and phong shading
    void setSpecularRGB(float r, float g, float b);
    
    // sets the specular intensity used in phong shading
    void setSpecIntensity(float ni);
    
    // sets the reflectance of this shape
    void setReflectance(float r, float g, float b);
    
    // sets the rotation elements
    void setRotater(float x, float y, float z);
    
    // sets the scaling elements
    void setScaler(float x, float y, float z);
    
    // sets the translation elements
    void setTranslater(float x, float y, float z);
};

#endif /* defined(____shape__) */
