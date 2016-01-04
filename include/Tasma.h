#ifndef TASMA_H
#define TASMA_H
#include <GL/gl.h>
#include <GL/glut.h>
#include "imageloader.h"

class Tasma
{
    public:
        Tasma(float startingPoint[]);
        Tasma(float startingPoint[] ,GLuint texture);
        virtual ~Tasma();
        void draw();
        void increaseDisplacement();
    protected:
    private:
        GLuint mainTexture;
        float startingPoint[3];
        float length=24;
        float height=3;
        float width=3;
        float thickness=0.5;
        float displacement=0;

        GLuint loadTexture(Image* image);
        void drawMainCos();
        void drawSquarish(float length);
        void loadTexture();
        void drawPlane();
        void drawCylinder();
};

#endif // TASMA_H
