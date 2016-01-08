#include "Tasma.h"
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include "imageloader.h"
#include "Box.h"
#include <iostream>
#include <vector>
using namespace std;

Tasma::Tasma(float startingPoint[])
{
    loadTextures();
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
        glTranslatef(startingPoint[0],startingPoint[1]+height, startingPoint[2]+width/2);
        glPushMatrix();

        drawMainCos();
        drawNiszczarka();

        glDisable(GL_TEXTURE_2D);
        drawBoxMaker();
        displaySideWalls();
        drawStojaks();
        glPopMatrix();
    glPopMatrix();
    displayBoxes();
    if(texturesEnabled)
    glEnable(GL_TEXTURE_2D);
}
void Tasma::displaySideWalls()
{
    displaySideWall();
    glPushMatrix();
    glTranslatef(0,0,width);
    displaySideWall();
    glPopMatrix();
}
void Tasma::displaySideWall()
{
    glColor3f(0.1,0.1,0.1);
    glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(length,0,0);
    glVertex3f(length,-thickness,0);
    glVertex3f(0,-thickness,0);
    glEnd();

    drawCircle();
}

void Tasma::drawNiszczarka()
{
    glPushMatrix();
    glTranslatef(length+2.4*height,-height+0.1,width/2);
    glBindTexture(GL_TEXTURE_2D,blackHoleTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri( GL_TEXTURE_2D, blackHoleTexture, GL_REPEAT);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex3f(-width*0.7,0,-width/2);
    glTexCoord2f(0,1);
    glVertex3f(-width*0.7,0,width/2);
    glTexCoord2f(1,1);
    glVertex3f(width*0.7,0,width/2);
    glTexCoord2f(1,0);
    glVertex3f(width*0.7,0,-width/2);
    glEnd();
    glPopMatrix();
}
void Tasma::drawCircle()
{
    glPushMatrix();
    glTranslatef(length,-thickness/2,0);
    for (int i=0; i<360; i+=3)
    {
        glPushMatrix();
        glRotatef(i,0,0,1);
        glBegin(GL_QUADS);
        glVertex3f(-thickness/100,thickness/2,0);
        glVertex3f(thickness/100,thickness/2,0);
        glVertex3f(thickness/100,-thickness/2,0);
        glVertex3f(-thickness/100,-thickness/2,0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();
}

void Tasma::drawStojaks()
{
    for (int j=0; j<2; j++)
    {
        glPushMatrix();
        if (j==0)
            glTranslatef(0,0,-0.1);
        else
            glTranslatef(0,0,width+0.1);

        for (int i=length/4; i<=length; i+=length/4)
        {
            glPushMatrix();
            glTranslatef(i,-height,0);
            drawStojak();
            glPopMatrix();
        }
        glPopMatrix();
    }

}
void Tasma::drawStojak()
{
    float wysoksocPodkladki=0.3;
    drawProstopadloscian(1,wysoksocPodkladki,1);
    glPushMatrix();
    glTranslatef(0,wysoksocPodkladki,0);
    drawProstopadloscian(0.2,height-wysoksocPodkladki,0.2);
    glPopMatrix();
}
void Tasma::drawProstopadloscian(float mLength, float mHeight, float mWidth)
{
    glPushMatrix();
    glTranslatef(-mLength/2,0,-mWidth/2);
    float vertex[8][3]={
        0,0,0,
        mLength,0,0,
        mLength,0,mWidth,
        0,0,mWidth,
        0,mHeight,0,
        mLength,mHeight,0,
        mLength,mHeight,mWidth,
        0,mHeight,mWidth
        };
    drawWall(vertex[0],vertex[1],vertex[2],vertex[3]);
    drawWall(vertex[0],vertex[1],vertex[5],vertex[4]);
    drawWall(vertex[1],vertex[2],vertex[6],vertex[5]);
    drawWall(vertex[2],vertex[3],vertex[7],vertex[6]);
    drawWall(vertex[3],vertex[0],vertex[4],vertex[7]);
    drawWall(vertex[4],vertex[5],vertex[7],vertex[6]);
    glPopMatrix();
}
void Tasma::drawWall(float vertexA[], float vertexB[], float vertexC[], float vertexD[])
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

void Tasma::displayBoxes()
{
    displayVectorOfBoxes(onLineBoxes);
    displayVectorOfBoxes(underLineBoxes);
}
void Tasma::displayVectorOfBoxes(vector<Box*> myVector)
{
    if (myVector.size()!=0)
    {
        for (int i=0; i<myVector.size(); i++)
        {
            myVector[i]->display();
        }
    }
}
void Tasma::drawMainCos()
{
    glBindTexture(GL_TEXTURE_2D,mainTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri( GL_TEXTURE_2D, mainTexture, GL_REPEAT);

    drawPlane();

    glPushMatrix();
        glTranslatef(length,-1*thickness,width);
        glRotatef(180,0,1,0);
        drawPlane();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(length,-0.5*thickness,0);
        glRotatef(displacement*-1*360,0,0,1);
        drawCylinder();
    glPopMatrix();
}
void Tasma::drawBoxMaker()
{
    float boxPadding=0.2;
    float boxMakerSize=height+thickness+Box::size+boxPadding;
    glPushMatrix();
    glTranslatef(0,-thickness,(width)/2);
    glColor3f(0.5,0.5,0.5);
    glutSolidCube(boxMakerSize);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(boxMakerSize/2+0.1,0,0);
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glVertex3f(0,-thickness,-boxPadding);
    glVertex3f(0,Box::size+boxPadding,-boxPadding);
    glVertex3f(0,Box::size+boxPadding,width+boxPadding);
    glVertex3f(0,-thickness,width+boxPadding);
    glEnd();
    glPopMatrix();
}

void Tasma::keepProduction()
{
    increaseDisplacement();
    if (shouldMakeNewBox)
    {
        float tab[]={startingPoint[0],startingPoint[1]+height,startingPoint[2]+width/2+Box::size};
        onLineBoxes.push_back(new Box(tab));
        cout<<displacement<<endl;
        shouldMakeNewBox=false;
    }
    moveBoxes();

}
void Tasma::moveBoxes()
{
    if (onLineBoxes.size()!=0)
    {
        int onLineBoxesSize=onLineBoxes.size();
        for (int i=0; i<onLineBoxesSize; i++)
        {
            moveSingleBox(onLineBoxes[i]);
        }
    }
}
void Tasma::moveSingleBox(Box* box)
{
    if (box->getPositionX()<startingPoint[0]+length-Box::size/3)
    {
        box->move(3*displacementModifier,0,0);
    }
    else
    {
        box->rotateZ(-80*displacementModifier);
        box->move(2*displacementModifier,-0.01,0);
    }

    if (box->getPositionY()<-3)
    {
        destroyBox(box);
    }
}
void Tasma::destroyBox(Box* box)
{
    removeBoxFromVector(box,onLineBoxes);
    delete box;
}

void Tasma::Click(int x, int y, int windowWidth, int windowHeight, float angleXZ, float angleY, float positionXYZ[3])
{
    int horizontalDirection = x-windowWidth/2 ;
    int verticalDirection =  windowHeight/2-y ;
    float mAngleXZ=angleXZ+horizontalDirection*M_PI_4/(1.0*windowWidth);
    float mAngleY=angleY+verticalDirection*M_PI_4/(1.0*windowHeight);
    float mRotateY=sin(mAngleY);
    float mRotateX=sin(mAngleXZ)*cos(mAngleY);
    float mRotateZ=cos(mAngleXZ)*cos(mAngleY);

    Box* boksik=findBox(positionXYZ,mRotateX,mRotateY,mRotateZ,onLineBoxes);
    if (boksik!=NULL)
    {
        switchBoxState(boksik,onLineBoxes,underLineBoxes);
    }
    else
    {
        boksik=findBox(positionXYZ,mRotateX,mRotateY,mRotateZ,underLineBoxes);
        if (boksik!=NULL)
        {
            switchBoxState(boksik,underLineBoxes,onLineBoxes);
        }
    }
}
void Tasma::switchBoxState(Box* box, vector<Box*> &sourceVector, vector<Box*> &destinationVector)
{
    removeBoxFromVector(box, sourceVector);
    destinationVector.push_back(box);
    box->move(0,box->getPositionY()==height ? -height : height,0);
}


Box* Tasma::findBox(float positionXYZ[3], float mRotateX, float mRotateY, float mRotateZ, vector<Box*> boxVector){

    if (boxVector.size()!=0)
    {
        for (int j=0; j<boxVector.size(); j++)
        {
            float point[]={positionXYZ[0], positionXYZ[1], positionXYZ[2]};
            for (int i=0; i<100; i+=1)
            {
                if (boxVector[j]->containsPoint(point))
                    return boxVector[j];

                point[0]+=mRotateX/2;
                point[1]+=mRotateY/2;
                point[2]-=mRotateZ/2;
            }
        }
    }

    return NULL;
}

void Tasma::removeBoxFromVector(Box* box, vector<Box*> &mVector)
{
    vector<Box*>::iterator it=mVector.begin();
    while (it!=mVector.end() && *it!=box )
        it++;

    if (it!=mVector.end())
        mVector.erase(it);
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
    displacement+=displacementModifier;
    if (displacement>probablyWrongDisplacementCounter+2)
    {
        probablyWrongDisplacementCounter+=2;
        shouldMakeNewBox=true;
    }
}

void Tasma::drawSquarish(float length)
{
    glBindTexture(GL_TEXTURE_2D,mainTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0-displacement);
    glVertex3f(0,0,0);
    glTexCoord2f(0,1-displacement);
    glVertex3f(length,0,0);
    glTexCoord2f(1,1-displacement);
    glVertex3f(length,0,width);
    glTexCoord2f(1,0-displacement);
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
void Tasma::loadTextures()
{
    Image* obrazek=loadBMP("D:\\Prywatne\\grafika\\tasma_texture.bmp");
    mainTexture=loadTexture(obrazek);
    obrazek=loadBMP("D:\\Prywatne\\grafika\\blackhole_texture.bmp");
    blackHoleTexture=loadTexture(obrazek);
    delete obrazek;
}
