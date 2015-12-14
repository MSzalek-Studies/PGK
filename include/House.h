#ifndef HOUSE_H
#define HOUSE_H

enum Shape {teapot, sphere, cube};

class House
{
    public:
        House(float size, int shape, float* wallColor, float* roofColor);
        void display();
        virtual ~House();
    protected:
    private:
        int shape;
        float size;
        float* wallColor;
        float* roofColor;
};

#endif // HOUSE_H
