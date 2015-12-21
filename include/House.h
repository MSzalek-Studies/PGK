#ifndef HOUSE_H
#define HOUSE_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

enum Shape {teapot, sphere, cube};

class House
{
    public:

        House(float size, int shape, float* wallColor, GLuint wallTexture, float* roofColor, GLuint roofTexture);
        void display();
        virtual ~House();
    protected:
    private:
        GLuint wallTexture;
        GLuint roofTexture;
        int shape;
        float size;
        float* wallColor;
        float* roofColor;

        void drawRoof();
        void drawTriangle(float vertexA[], float vertexB[], float vertexC[]);
        void drawCube();
        void drawWall(float[], float[], float[], float[]);
};

#endif // HOUSE_H
