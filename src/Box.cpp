#include "Box.h"
#include "GL/glu.h"
#include "Gl/glut.h"
Box::Box(float basePoint[])
{
    for (int i=0; i<3; i++)
        this->basePoint[i]=basePoint[i];
}

Box::Box()
{
    for (int i=0; i<3; i++)
        basePoint[i]=0;
}

Box::~Box()
{
    //dtor
}

float Box::getPositionX()
{
    return basePoint[0];
}

float Box::getPositionY()
{
    return basePoint[1];
}
float Box::getPositionZ()
{
    return basePoint[2];
}
void Box::rotateZ(float rotationValue)
{
    rotationZ+=rotationValue;
}
bool Box::containsPoint(float point[])
{
    bool result=true;
    for (int i=0; i<3; i++)
        result=result && point[i]>basePoint[i] && point[i]<basePoint[i]+size;
    return result;
}
void Box::move(float directionX, float directionY, float directionZ)
{
    basePoint[0]+=directionX;
    basePoint[1]+=directionY;
    basePoint[2]+=directionZ;
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
    glTranslatef(size/2,size/2,size/2);
    glRotatef(rotationZ,0,0,1);
    glTranslatef(-size/2,-size/2,-size/2);
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
