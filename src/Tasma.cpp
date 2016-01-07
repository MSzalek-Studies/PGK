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
        glTranslatef(startingPoint[0],startingPoint[1]+height, startingPoint[2]+width/2);
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D,mainTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri( GL_TEXTURE_2D, mainTexture, GL_REPEAT);

        drawMainCos();
        drawBoxMaker();

        glPopMatrix();
    glPopMatrix();
    displayBoxes();

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
    glPushMatrix();
    glTranslatef(0,-thickness,(width+thickness)/2);
    glColor3f(0.5,0.5,0.5);
    glutSolidCube(height+thickness+Box::size+boxPadding);
    glPopMatrix();
}

void Tasma::keepProduction()
{
    increaseDisplacement();
    if (shouldMakeNewBox)
    {
        float tab[]={startingPoint[0],startingPoint[1]+height,startingPoint[2]+width/2+Box::size};
        /*cout<<"przy tworzeniu pudelka:";
        for (int i=0; i<3; i++)
            cout<<startingPoint[i]<<" ";
        cout<<endl;*/
        onLineBoxes.push_back(new Box(tab));
        //cout<<startingPoint[0]<<" "<<startingPoint[1]<<" "<<startingPoint[2]<<endl;
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
    if (box->getPositionX()<startingPoint[0]+length-2*Box::size/3)
    {
        box->move(3*displacementModifier,0,0);
    }
    else
    {
        box->rotateZ(-100*displacementModifier);
        box->move(2*displacementModifier,-0.01,0);
    }

    if (box->getPositionY()<-3)
    {
        cout<<"przed"<<onLineBoxes.size()<<endl;
        destroyBox(box);
        cout<<"po"<<onLineBoxes.size()<<endl;
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

    //cout<<"coos"<<endl;
    //    cout<<"BOX1"<<" "<<onLineBoxes[0]->getPositionX()<<" "<<onLineBoxes[0]->getPositionY()<<" "<<onLineBoxes[0]->getPositionZ()<<endl;

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
            for (int i=0; i<60; i+=1)
            {
  //              if (j==0)
//                cout<<point[0]<<" "<<point[1]<<" "<<point[2]<<endl;
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
    cout<<"przed"<<mVector.size()<<endl;
    vector<Box*>::iterator it=mVector.begin();
    while (it!=mVector.end() && *it!=box )
        it++;


    if (it!=mVector.end())
        mVector.erase(it);

    cout<<"po"<<mVector.size()<<endl;
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
void Tasma::loadTexture()
{
    Image* obrazek=loadBMP("D:\\Prywatne\\grafika\\brick_texture.bmp");
    mainTexture=loadTexture(obrazek);
    delete obrazek;
}
