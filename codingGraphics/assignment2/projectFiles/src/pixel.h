//
//  pixel.h
//  
//
//  Created by Akhil Batra on 3/12/14.
//
//

#ifndef ____pixel__
#define ____pixel__

#include "vec3.h"
#include <iostream>
#include <string>
#include <sstream>

#endif /* defined(____pixel__) */

class Pixel {
private:
    Vec3 rgb;

public:
    // default constructor
    Pixel();
    
    // creates a pixel with the given RGB values
    Pixel(float r, float g, float b);
    
    // creates a pixel with the given Vec3 object as the RGB values
    Pixel(Vec3 v);
    
    // sets the red value of this pixel
    void setRed(float newR);
    
    // sets the green value of this pixel
    void setGreen(float newG);
    
    // sets the blue value of this pixel
    void setBlue(float newB);
    
    // sets all 3 RGB values of this pixel
    void setColor(float r, float g, float b);
    
    // sets RGB Vec3 of this pixel
    void setColor(Vec3 c);
    
    // returns the red value of this pixel
    float getRed();
    
    // returns the green value of this pixel
    float getGreen();
    
    // returns the blue value of this pixel
    float getBlue();
    
    // returns the Vec3 object that contains the RGB values
    Vec3 getColor();
    
    // displays this pixel as a string
    string stringify();
};