//
//  shape.cpp
//  
//
//  Created by Akhil Batra on 3/13/14.
//
//

#include "shape.h"
#define PI 3.14159265  // Should be used from mathlib

Vec3 Shape::getAmbient() {
    return ambientRGB;
}

Vec3 Shape::getDiffuseRGB() {
    return diffuseRGB;
}

Vec3 Shape::getSpecularRGB() {
    return specRGB;
}

float Shape::getSpecIntensity() {
    return specInt;
}

Vec3 Shape::getReflectance() {
    return reflect;
}

Matrix Shape::getScaler() {
    Matrix s(4, 4);
    s.setValue(0, 0, scale.getX() == 0 ? 1 : scale.getX());
    s.setValue(1, 1, scale.getY() == 0 ? 1 : scale.getY());
    s.setValue(2, 2, scale.getZ() == 0 ? 1 : scale.getZ());
    s.setValue(3, 3, 1);
    return s;
}

Matrix Shape::getRotater() {
    Matrix r(4, 4);
    float xRad = rotate.getX() * PI / 180;
    float yRad = rotate.getY() * PI / 180;
    float zRad = rotate.getZ() * PI / 180;
    r.setValue(0, 0, cos(yRad) * cos(zRad));
    r.setValue(0, 1, -cos(xRad) * sin(zRad) + sin(xRad) * sin(yRad) * cos(zRad));
    r.setValue(0, 2, sin(xRad) * sin(zRad) + cos(xRad) * sin(yRad) * cos(zRad));
    r.setValue(1, 0, cos(yRad) * sin(zRad));
    r.setValue(1, 1, cos(xRad) * cos(zRad) + sin(xRad) * sin(yRad) * sin(zRad));
    r.setValue(1, 2, -sin(xRad) * cos(yRad) + cos(xRad) * sin(yRad) * sin(zRad));
    r.setValue(2, 0, -sin(yRad));
    r.setValue(2, 1, sin(xRad) * cos(yRad));
    r.setValue(2, 2, cos(xRad) * cos(yRad));
    r.setValue(3, 3, 1);
    return r;
}

Matrix Shape::getTranslater() {
    Matrix t(4, 4);
    t.setValue(0, 0, 1);
    t.setValue(1, 1, 1);
    t.setValue(2, 2, 1);
    t.setValue(3, 3, 1);
    
    t.setValue(0, 3, translate.getX());
    t.setValue(1, 3, translate.getY());
    t.setValue(2, 3, translate.getZ());
    return t;
}

Matrix Shape::getTransformer() {
    return trans;
}

void Shape::setTransformer() {
    trans = getRotater().multiplyMatrix(getScaler()).multiplyMatrix(getTranslater());
}

void Shape::setAmbient(float r, float g, float b) {
    ambientRGB.setX(r);
    ambientRGB.setY(g);
    ambientRGB.setZ(b);
}

void Shape::setDiffuseRGB(float r, float g, float b) {
    diffuseRGB = Vec3(r, g, b);
}

void Shape::setSpecularRGB(float r, float g, float b) {
    specRGB = Vec3(r, g, b);
}

void Shape::setSpecIntensity(float ni) {
    specInt = ni;
}

void Shape::setReflectance(float r, float g, float b) {
    reflect = Vec3(r, g, b);
}

void Shape::setRotater(float x, float y, float z) {
    rotate = Vec3(x, y, z);
}

void Shape::setScaler(float x, float y, float z) {
    scale = Vec3(x, y, z);
}

void Shape::setTranslater(float x, float y, float z) {
    translate = Vec3(x, y, z);
}