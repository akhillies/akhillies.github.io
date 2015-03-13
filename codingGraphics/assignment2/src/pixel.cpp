//
//  pixel.cpp
//  
//
//  Created by Akhil Batra on 3/12/14.
//
//

#include "pixel.h"

Pixel::Pixel() {
    rgb = Vec3(0, 0, 0);
}

Pixel::Pixel(Vec3 v) {
    rgb = Vec3(v.getX(), v.getY(), v.getZ());
}

Pixel::Pixel(float r, float g, float b) {
    rgb = Vec3(r, g, b);
}

void Pixel::setRed(float newR) {
    rgb.setX(newR);
}

void Pixel::setGreen(float newG) {
    rgb.setY(newG);
}

void Pixel::setBlue(float newB) {
    rgb.setZ(newB);
}

void Pixel::setColor(float r, float g, float b) {
    setRed(r);
    setGreen(g);
    setBlue(b);
}

void Pixel::setColor(Vec3 c) {
    setRed(c.getX());
    setGreen(c.getY());
    setBlue(c.getZ());
}

Vec3 Pixel::getColor() {
    return rgb;
}

float Pixel::getRed() {
    return rgb.getX();
}

float Pixel::getGreen() {
    return rgb.getY();
}

float Pixel::getBlue() {
    return rgb.getZ();
}

string Pixel::stringify() {
    ostringstream s;
    s << rgb.stringify();
    return s.str();
}