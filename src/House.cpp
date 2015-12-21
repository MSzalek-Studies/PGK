#include "House.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include "imageloader.h"
House::House(float size, int shape, float* wallColor, GLuint wallTexture, float* roofColor, GLuint roofTexture)
{
    this->size=size;
    this->shape=shape;
    this->wallColor=wallColor;
    this->roofColor=roofColor;
    this->wallTexture=wallTexture;
    this->roofTexture=roofTexture;
}

House::~House()
{
    //dtor
}

void House::display()
{
    glPushMatrix();
    glTranslatef(0, size/2, 0);
    glColor3fv(wallColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    switch (shape)
    {
        case cube: drawCube();
                break;
        case teapot: glutSolidTeapot(size);
                break;
        case sphere: glutSolidSphere(size/2,20,20);
                break;
    }
    drawRoof();
    glPopMatrix();
}

void House::drawCube()
{
    float vertex[8][3]={
        0,0,0,
        size,0,0,
        size,0,size,
        0,0,size,
        0,size,0,
        size,size,0,
        size,size,size,
        0,size,size,
        };
    glPushMatrix();
    drawWall(vertex[0],vertex[1],vertex[5],vertex[4]);
    drawWall(vertex[1],vertex[2],vertex[6],vertex[5]);
    drawWall(vertex[2],vertex[3],vertex[7],vertex[6]);
    drawWall(vertex[3],vertex[0],vertex[4],vertex[7]);
    drawWall(vertex[4],vertex[5],vertex[7],vertex[6]);
    glPopMatrix();
}

void House::drawRoof()
{
    float vertex[5][3]={
        0,size,0,
        size,size,0,
        size,size,size,
        0,size,size,
        size/2, size*3/2, size/2
        };
    glPushMatrix();
    glColor3fv(roofColor);
    glBindTexture(GL_TEXTURE_2D,roofTexture);
    drawTriangle(vertex[0],vertex[1], vertex[4]);
    drawTriangle(vertex[1],vertex[2], vertex[4]);
    drawTriangle(vertex[2],vertex[3], vertex[4]);
    drawTriangle(vertex[3],vertex[0], vertex[4]);
    glPopMatrix();

}
void House::drawTriangle(float vertexA[], float vertexB[], float vertexC[])
{
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0,0);
    glVertex3fv(vertexA);
    glTexCoord2f(1,0);
    glVertex3fv(vertexB);
    glTexCoord2f(0.5,1);
    glVertex3fv(vertexC);
    glEnd();
}
void House::drawWall(float vertexA[], float vertexB[], float vertexC[], float vertexD[])
{

    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex3fv(vertexA);
    glTexCoord2f(1,0);
    glVertex3fv(vertexB);
    glTexCoord2f(1,1);
    glVertex3fv(vertexC);
    glTexCoord2f(0,1);
    glVertex3fv(vertexD);
    glEnd();
}
