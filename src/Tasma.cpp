#include "Tasma.h"
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include "imageloader.h"
#include <iostream>
Tasma::Tasma(float startingPoint[])
{
    loadTexture();
    this->startingPoint[0]=startingPoint[0];
    this->startingPoint[1]=startingPoint[1];
    this->startingPoint[2]=startingPoint[2];
}
Tasma::Tasma(float startingPoint[] ,GLuint texture)
{
    mainTexture=texture;
    this->startingPoint[0]=startingPoint[0];
    this->startingPoint[1]=startingPoint[1];
    this->startingPoint[2]=startingPoint[2];
}

Tasma::~Tasma()
{
    //dtor
}
void Tasma::draw(){
    glPushMatrix();
    glTranslatef(startingPoint[0],startingPoint[1]+height, startingPoint[2]);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,mainTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri( GL_TEXTURE_2D, mainTexture, GL_REPEAT);

    drawMainCos();
    glPopMatrix();
    glPopMatrix();
}
void Tasma::drawMainCos()
{
    drawPlane();
    glPushMatrix();
        glTranslatef(length,-1*thickness,width);
        glRotatef(180,0,1,0);
        drawPlane();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,-0.5*thickness,0);
        glRotatef(displacement*360,0,0,1);
        drawCylinder();
    glPopMatrix();
}

void Tasma::drawCylinder()
{
    float radius=thickness/2;
    int slices=36;
    float side=sqrt(2*radius*radius*(1-cos((360.0/slices)*M_PI/180.0)));
    float sumOfSides=0;
    for (float rotation=0; rotation<360; rotation+=(360.0/slices))
    {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,mainTexture);
        glRotatef(rotation,0,0,1);
        glTranslatef(0,radius,0);

        glBegin(GL_QUADS);
        glTexCoord2f(0,sumOfSides/width);
        glVertex3f(-side/2,0,0);
        glTexCoord2f(0,sumOfSides/width+side/width);
        glVertex3f(side/2,0,0);
        glTexCoord2f(1,sumOfSides/width+side/width);
        glVertex3f(side/2,0,width);
        glTexCoord2f(1,sumOfSides/width);
        glVertex3f(-side/2,0,width);
        glEnd();
        glPopMatrix();

        sumOfSides+=side;
        if (sumOfSides>=width)
            sumOfSides=0;
    }

}

void Tasma::drawPlane()
{
    for (int squareDisplacement=0; squareDisplacement<length; squareDisplacement+=width)
    {
        glPushMatrix();
        glTranslatef(squareDisplacement,0,0);
        drawSquarish(width);
        glPopMatrix();
    }
}

void Tasma::increaseDisplacement(){
    displacement+=0.001;
}

void Tasma::drawSquarish(float length)
{
    glBindTexture(GL_TEXTURE_2D,mainTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0+displacement);
    glVertex3f(0,0,0);
    glTexCoord2f(0,1+displacement);
    glVertex3f(length,0,0);
    glTexCoord2f(1,1+displacement);
    glVertex3f(length,0,width);
    glTexCoord2f(1,0+displacement);
    glVertex3f(0,0,width);
    glEnd();
}
GLuint Tasma::loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}
void Tasma::loadTexture()
{
    Image* obrazek=loadBMP("D:\\Prywatne\\grafika\\brick_texture.bmp");
    mainTexture=loadTexture(obrazek);
    delete obrazek;
}
