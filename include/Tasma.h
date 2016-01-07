#ifndef TASMA_H
#define TASMA_H
#include <GL/gl.h>
#include <GL/glut.h>
#include "imageloader.h"
#include "Box.h"
#include <vector>

using namespace std;
class Tasma
{
    public:
                void Click(int x, int y, int windowWidth, int windowHeight, float angleXZ, float angleY, float positionXYZ[3]);

        Tasma(float startingPoint[]);
        Tasma(float startingPoint[] ,GLuint texture);
        virtual ~Tasma();
        void draw();
        void keepProduction();
    protected:
    private:
        GLuint mainTexture;
        float startingPoint[3];
        float length=24;
        float height=3;
        float width=3;
        float thickness=0.5;
        float displacement=0;
        int probablyWrongDisplacementCounter=0;
        bool shouldMakeNewBox=true;
        vector<Box*> onLineBoxes;
        vector<Box*> underLineBoxes;
        const float displacementModifier=0.001;

        void switchBoxState(Box* box, vector<Box*> &sourceVector, vector<Box*> &destinyVector);
        void displayVectorOfBoxes(vector<Box*> myVector);
        void removeBoxFromVector(Box* box, vector<Box*> &mVector);
        Box* findBox(float point[3],float mRotateX, float myRotateY, float myRotateZ, vector<Box*> boxVector);
        void destroyBox(Box* box);
        void moveBoxes();
        void moveSingleBox(Box* box);
        void displayBoxes();
        void drawBoxMaker();
        GLuint loadTexture(Image* image);
        void drawMainCos();
        void drawSquarish(float length);
        void loadTexture();
        void drawPlane();
        void drawCylinder();
        void increaseDisplacement();
};

#endif // TASMA_H
