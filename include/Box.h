#ifndef BOX_H
#define BOX_H


class Box
{
    public:
        static const float size=1;
        void display();
        Box(float basePoint[]);
        Box();
        void rotateZ(float rotationValue);
        bool containsPoint(float point[]);
        void move(float directionX, float directionY, float directionZ);
        float getPositionX();
        float getPositionY();
        float getPositionZ();
        ~Box();

    protected:
    private:
        float basePoint[3];
        float rotationZ=0;
        void drawWall(float vertexA[], float vertexB[], float vertexC[], float vertexD[]);
};

#endif // BOX_H
