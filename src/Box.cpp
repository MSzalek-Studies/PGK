#include "Box.h"
#include "GL/glu.h"
#include "Gl/glut.h"
Box::Box(float basePoint[], float size)
{
    for (int i=0; i<3; i++)
        this->basePoint[i]=basePoint[i];
    this->size=size;
}

Box::~Box()
{
    //dtor
}

bool Box::containsPoint(float point[])
{
    bool result=true;
    for (int i=0; i<3; i++)
        result=result && point[i]>basePoint[i] && point[i]<basePoint[i]+size;
    return result;
}

void Box::display(){

    float vertex[8][3]={
        0,0,0,
        size,0,0,
        size,0,size,
        0,0,size,
        0,size,0,
        size,size,0,
        size,size,size,
        0,size,size
        };
    glPushMatrix();
    glColor3f(1,1,0);
    glTranslatef(basePoint[0],basePoint[1],basePoint[2]);
    drawWall(vertex[0],vertex[1],vertex[2],vertex[3]);
    drawWall(vertex[0],vertex[1],vertex[5],vertex[4]);
    drawWall(vertex[1],vertex[2],vertex[6],vertex[5]);
    drawWall(vertex[2],vertex[3],vertex[7],vertex[6]);
    drawWall(vertex[3],vertex[0],vertex[4],vertex[7]);
    drawWall(vertex[4],vertex[5],vertex[7],vertex[6]);
    glPopMatrix();
}

void Box::drawWall(float vertexA[], float vertexB[], float vertexC[], float vertexD[])
{
    glBegin(GL_QUADS);
    //glTexCoord2f(0,0);
    glVertex3fv(vertexA);
    //glTexCoord2f(1,0);
    glVertex3fv(vertexB);
    //glTexCoord2f(1,1);
    glVertex3fv(vertexC);
    //glTexCoord2f(0,1);
    glVertex3fv(vertexD);
    glEnd();
}
