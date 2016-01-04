#ifndef BOX_H
#define BOX_H


class Box
{
    public:
        void display();
        Box(float basePoint[], float);
        virtual ~Box();
        bool containsPoint(float point[]);
    protected:
    private:
        float basePoint[3];
        float size;
        void drawWall(float vertexA[], float vertexB[], float vertexC[], float vertexD[]);
};

#endif // BOX_H
