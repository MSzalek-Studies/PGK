#include "House.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

House::House(float size, int shape, float* wallColor, float* roofColor)
{
    this->size=size;
    this->shape=shape;
    this->wallColor=wallColor;
    this->roofColor=roofColor;
}

House::~House()
{
    //dtor
}

void House::display()
{
    glPushMatrix();
    glTranslatef(0,size/2,0);
    glColor3fv(wallColor);
    switch (shape)
    {
        case cube: glutSolidCube(size);
                break;
        case teapot: glutSolidTeapot(size);
                break;
        case sphere: glutSolidSphere(size/2,20,20);
                break;
    }
    glPushMatrix();
    glTranslatef(0,size/2,0);
    glRotatef(-90,1,0,0);
    glColor3fv(roofColor);
    glutSolidCone(size/2,size,20,20);
    glPopMatrix();
    glPopMatrix();
}

