#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "imageloader.h"
#include "House.h"
void pressedKey(int key, int x, int y);
void Reshape(int width, int height);
void Draw();
void idle();
void drawSquareFloor(int sideSize);
void move(int direction);
void turn(int directionHorizontally, int directionVertically);
void releasedKey(int key, int x, int y);
void onMouseMoved(int x, int y);
void setSkyColor(int color);
void setFloorColor(int color);
float* getColorArray(int color);
void createMenus();
void initColors();
float* setValuesOfColorArray(float R, float G, float B);
void onMouseEntry(int state);
void pointlessFunction(int a);
void onReallyPassiveMouseMoved(int x, int y);
void onMouseButtonPressed(int button, int state, int x, int y);
void drawHouses();
void pressedKeyboardKey(unsigned char key, int x, int y);
void releasedKeyboardKey(unsigned char key, int x, int y);
void initStars();
void drawLight1();
void drawLight0();
void drawLight2();
void drawLight3();
void initTextures();
GLuint loadTexture(Image* image);

enum kolor{
czarny, czerwony, niebieski, zielony, bialy, blekitny, zolty, fioletowy
};

using namespace std;

bool isTurningNormal=false; //DO USTAWIENIA
int windowHeight=600, windowWidth=800;
float angle=1.6, angleY=-0.3, ratio;
float positionX=-5, positionY=6.75, positionZ=10;
float rotateX=0, rotateY=-0.5, rotateZ=0;
int moveDirection=0, rotateDirection=0;
float moveSpeed=0.4, rotateSpeed=0.005;
float *skyColor, *roofColor, *wallColor, *floorColor;
bool menuOpen=false, isLeftButtonPressed=false;
int mouseX=windowWidth/2, mouseY=windowHeight/2;
float step=0;
int houseSize=1, houseShape=cube;
bool light0_enabled=true, light1_enabled=true, light2_enabled=true, light3_enabled=true;
float light_angle=0;
GLuint floorTexture, wallTexture, roofTexture;
bool texturesEnabled=true;
GLint textureEnvMode = GL_DECAL, floorMinFilter=GL_NEAREST, floorMagFilter=GL_NEAREST;

void refreshLooking() {
    glLoadIdentity();
    gluLookAt(positionX, positionY, positionZ,
              positionX+rotateX, positionY+rotateY, positionZ+rotateZ,
              0,1,0);   //look straight
}

int main(int argc, char *argv[])
{
    initColors();
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(200,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Domki");
    initTextures();
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_COLOR_MATERIAL);
    glutIdleFunc(idle);
    if (isTurningNormal)
        glutPassiveMotionFunc(onMouseMoved);
    else {
        glutMotionFunc(onMouseMoved);
        glutPassiveMotionFunc(onReallyPassiveMouseMoved);
    }

    createMenus();
    glClearColor(1,0,0,0);
    glutKeyboardFunc(pressedKeyboardKey);
    glutKeyboardUpFunc(releasedKeyboardKey);
    glutSpecialFunc(pressedKey);
    glutSpecialUpFunc(releasedKey);
    glutEntryFunc(onMouseEntry);
    glutMouseFunc(onMouseButtonPressed);
    glutMainLoop();
    return EXIT_SUCCESS;
}

void idle() {
    glutPostRedisplay();
}

void Draw() {
    glClearColor(skyColor[0],skyColor[1],skyColor[2],0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSquareFloor(100);

    move(moveDirection);
    turn(rotateDirection,0);

    drawHouses();

    light_angle+=2;
    if (light0_enabled)
        drawLight0();
    if (light1_enabled)
        drawLight1();
    if (light2_enabled)
        drawLight2();
    if (light3_enabled)
        drawLight3();
    glutSwapBuffers();
}

void Reshape(int width, int height) {
    windowWidth=width;
    windowHeight=height;

    if (height==0)
       height=1;

    ratio = 1.0 * width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    refreshLooking();
             //  glutPostRedisplay();
}

void initColors() {
    floorColor=getColorArray(zielony);
    skyColor=getColorArray(blekitny);
    roofColor=getColorArray(fioletowy);
    wallColor=getColorArray(niebieski);
}
void initTextures() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureEnvMode);
    float alphaColor[]={1,0,1,0.5};
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, alphaColor);
    Image* obrazek=loadBMP("C:\\Users\\szale_000\\Desktop\\grafika\\grass_texture.bmp");
    floorTexture=loadTexture(obrazek);
    delete obrazek;
    Image* obrazek2=loadBMP("C:\\Users\\szale_000\\Desktop\\grafika\\brick_texture.bmp");
    wallTexture=loadTexture(obrazek2);
    delete obrazek2;
    Image* obrazek3=loadBMP("C:\\Users\\szale_000\\Desktop\\grafika\\roof_texture.bmp");
    roofTexture=loadTexture(obrazek3);
    delete obrazek3;
}

//****DRAWING FUNCTIONS*****//
GLuint loadTexture(Image* image) {
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
void switchTexturing(){
    if (texturesEnabled)
        glDisable(GL_TEXTURE_2D);
    else
        glEnable(GL_TEXTURE_2D);
    texturesEnabled=!texturesEnabled;
}
void initStars() {
    if (skyColor[0]==0 && skyColor[1]==0 && skyColor[2]==0)
    {
        glPointSize(2);
        glColor3f(1,1,1);
        for (int i=-1000; i<1000; i+=10)
        {
            for (int j=-1000; j<1000; j+=10)
            {
                glBegin(GL_POINTS);
                glVertex3f(i,100,j);
                glEnd();
            }
        }
    }
}

void drawLight0(){
    //glPushMatrix();
    float x[]={-0.5,1,-1,0};
    float ambient[]={0.2,0.2,0.2,1};
    float diffuse[]={1,1,1,1};
    float specular[]={1,1,1,1};
    glLightfv(GL_LIGHT0,GL_POSITION,x);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    refreshLooking();
    //glPopMatrix();
}
void drawLight1(){
    //glPushMatrix();
    float position[]={10,5,0,1};
    float ambient[]={0.2,0.2,0.2,1};
    float diffuse[]={1,1,1,1};
    float specular[]={1,1,1,1};
    float spotDirection[]={0,-1,1};
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0f);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 64);
    glLightfv(GL_LIGHT1, GL_POSITION,position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    refreshLooking();
   // glPopMatrix();
}
void drawLight2(){
    //glPushMatrix();
    float position[]={10,0.5,12,1};
    float ambient[]={1,1,1,1};
    float diffuse[]={1,1,1,1};
    float specular[]={1,1,1,1};
    float spotDirection[]={0,0,1};
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0f);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10);
    glLightfv(GL_LIGHT2,GL_POSITION,position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.3);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.1);
    refreshLooking();
    //glPopMatrix();
}
void drawLight3(){

    glPushMatrix();
    float position[]={0,0,0,1};
    float ambient[]={0.5,0.5,0.5,0.5};
    float diffuse[]={0.2,0.2,0.2,1};
    float specular[]={1,1,1,1};
    glTranslatef(10,1,10);
    glRotatef(light_angle,0,1,0);
    glTranslatef(3,0,0);
        glPointSize(5);
        glColor3f(1,1,1);
        glBegin(GL_POINTS);
        glVertex3f(0,0,0);
        glEnd();
    glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.5);
    glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.5);
    glLightfv(GL_LIGHT3,GL_POSITION,position);
    glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, specular);
    refreshLooking();
    glPopMatrix();
}
void switchLightning(bool &light_enabled, GLenum mGL_Light) {
    if (light_enabled)
        glDisable(mGL_Light);
    else
        glEnable(mGL_Light);
    light_enabled=!light_enabled;
}
void switchTextureEnv(){
    if (textureEnvMode==GL_DECAL)
        textureEnvMode=GL_MODULATE;
    else if (textureEnvMode==GL_MODULATE)
        textureEnvMode=GL_BLEND;
    else
        textureEnvMode=GL_DECAL;

    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, textureEnvMode);
}
void switchFloorTextureMode(){
    floorMinFilter= floorMinFilter == GL_NEAREST ? GL_LINEAR : GL_NEAREST;
    floorMagFilter= floorMagFilter == GL_NEAREST ? GL_LINEAR : GL_NEAREST;
}
void drawHouses() {
    int housesInRow=10;
    int distanceBetween=5;
    Image* obrazek2=loadBMP("C:\\Users\\szale_000\\Desktop\\grafika\\brick_texture.bmp");
    wallTexture=loadTexture(obrazek2);
    delete obrazek2;
    for (int i=0; i<housesInRow; i++)
    {
        for (int j=0; j<housesInRow; j++)
        {
            glPushMatrix();
            glTranslatef(distanceBetween*i,0,distanceBetween*j);

            House house(houseSize,houseShape,wallColor,wallTexture,roofColor,roofTexture);
            house.display();
            glPopMatrix();
        }
    }
}

void drawSquareFloor(int sideSize) {
    int startingX=-50;
    int startingZ=-50;
    float squareSize=10;

    glColor3fv(floorColor);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, floorMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, floorMagFilter);

    for (int x=startingX; x<startingX+sideSize; x+=squareSize)
    {
        for(int z=startingZ; z<startingZ+sideSize; z+=squareSize)
        {
            glBegin(GL_QUADS);
                glTexCoord2f(0,0);
                glVertex3f(x,0,z);
                glTexCoord2f(1,0);
                glVertex3f(x+squareSize,0,z);
                glTexCoord2f(1,1);
                glVertex3f(x+squareSize,0,z+squareSize);
                glTexCoord2f(0,1);
                glVertex3f(x,0,z+squareSize);
            glEnd();
        }
    }
}

//*****KEYBOARD FUNCTIONS*****//
void pressedKeyboardKey(unsigned char key, int x, int y) {
    switch (key)
    {
        case 'a':
        case 'A': rotateDirection=-1; break;
        case 'd':
        case 'D': rotateDirection=1; break;
        case 'w':
        case 'W': moveDirection=1; break;
        case 's':
        case 'S': moveDirection=-1; break;
        case 'q':
        case 'Q': positionY+=0.1; break;
        case 'e':
        case 'E': positionY-=0.1; break;
        case '1': switchLightning(light0_enabled, GL_LIGHT0); break;
        case '2': switchLightning(light1_enabled, GL_LIGHT1); break;
        case '3': switchLightning(light2_enabled, GL_LIGHT2); break;
        case '4': switchLightning(light3_enabled, GL_LIGHT3); break;
    }
}

void releasedKeyboardKey(unsigned char key, int x, int y) {
    switch (key)
    {
        case 'a':
        case 'A':
        case 'd':
        case 'D': rotateDirection=0; break;
        case 'w':
        case 'W':
        case 's':
        case 'S': moveDirection=0; break;
    }
}

void pressedKey(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_LEFT: rotateDirection=-1; break;
        case GLUT_KEY_RIGHT: rotateDirection=1; break;
        case GLUT_KEY_UP: moveDirection=1; break;
        case GLUT_KEY_DOWN: moveDirection=-1; break;
        case GLUT_KEY_F1: House::switchFilters(); break;
        case GLUT_KEY_F2: switchFloorTextureMode(); break;
        case GLUT_KEY_F3: switchTextureEnv(); break;
        case GLUT_KEY_F4: switchTexturing(); break;
    }
}

void releasedKey(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT: rotateDirection=0; break;
        case GLUT_KEY_DOWN:
        case GLUT_KEY_UP: moveDirection=0; break;
    }
}

//*****MOUSE FUNCTIONS*****//
void onReallyPassiveMouseMoved(int x, int y) {
    mouseX=x;
    mouseY=y;
}

void onMouseMoved(int x, int y) {
    if(isLeftButtonPressed || isTurningNormal)
    {
        if (!menuOpen)
        {
           int horizontalDirection = isTurningNormal ? x-mouseX : mouseX-x ;
            int verticalDirection = isTurningNormal ? mouseY-y : y-mouseY ;
            turn(horizontalDirection,verticalDirection);
        }
        else
            menuOpen=false;

    }
    mouseX=x;
    mouseY=y;
}

void onMouseEntry(int state) {
    if (state==GLUT_LEFT)
    {
        skyColor=getColorArray(czarny);
    }
    else if (state==GLUT_ENTERED)
    {
        skyColor=getColorArray(blekitny);
    }

}

void onMouseButtonPressed(int button, int state, int x, int y) {

    isLeftButtonPressed = state == GLUT_DOWN;
    if (isLeftButtonPressed){
        mouseX=x;
        mouseY=y;
    }
}

//*****MOVEMENT FUNCTIONS*****//
void move(int direction) {
    positionX=positionX+direction*rotateX*moveSpeed;
    positionZ=positionZ+direction*rotateZ*moveSpeed;
    if (direction){
        float szybkosc_skakania=0.3;
        float wysokosc_skakania=0.025;
        step+=szybkosc_skakania;
        positionY=positionY+sin(step)*wysokosc_skakania;
    }

    refreshLooking();
}

void turn(int directionHorizontally, int directionVertically) {

    angle+=directionHorizontally*rotateSpeed;
    angleY+=directionVertically*rotateSpeed;

    if (angleY>M_PI_2 || angleY<-M_PI_2)
        angleY-=directionVertically*rotateSpeed;

    rotateY=sin(angleY);
    rotateX=sin(angle)*cos(angleY);
    rotateZ=-cos(angle)*cos(angleY);
    refreshLooking();
}

//*****UTILS*****//
float* getColorArray(int color) {
    switch (color)
    {
        case czerwony: { return setValuesOfColorArray(1,0,0); }
        case niebieski: { return setValuesOfColorArray(0,0,1); }
        case zielony: { return setValuesOfColorArray(0,1,0); }
        case bialy: { return setValuesOfColorArray(1,1,1); }
        case blekitny: { return setValuesOfColorArray(0,1,1); }
        case zolty: { return setValuesOfColorArray(1,1,0); }
        case fioletowy: { return setValuesOfColorArray(0.7,0,0.7); }
        case czarny:
            default: { return setValuesOfColorArray(0,0,0); }
    }
}
float* setValuesOfColorArray(float R, float G, float B) {
    float* tab=new float[3];
    tab[0]=R;
    tab[1]=G;
    tab[2]=B;
    return tab;
}
void setWallColor(int color) { wallColor=getColorArray(color); }
void setRoofColor(int color) { roofColor=getColorArray(color); }
void setFloorColor(int color) { floorColor=getColorArray(color); }
void setSkyColor(int color) { skyColor=getColorArray(color); }
void setHouseSize(int size) { houseSize=size; }
void setHouseShape(int shape) { houseShape=shape; }


//*****MENU FUNCTIONS****//
void menu_setWallColor(int color) {
    setWallColor(color);
    menuOpen=true;
}
void menu_setRoofColor(int color) {
    setRoofColor(color);
    menuOpen=true;
}
void menu_setSkyColor(int color) {
    setSkyColor(color);
    menuOpen=true;
}
void menu_setFloorColor(int color) {
    setFloorColor(color);
    menuOpen=true;
}
void menu_setHouseShape(int shape) {
    setHouseShape(shape);
    menuOpen=true;
}
void menu_setHouseSize(int size) {
    setHouseSize(size);
    menuOpen=true;
}

void addColorMenuEntries() {
    glutAddMenuEntry("Czerwony",czerwony);
    glutAddMenuEntry("Zielony",zielony);
    glutAddMenuEntry("Niebieski",niebieski);
    glutAddMenuEntry("Zolty",zolty);
    glutAddMenuEntry("Czarny", czarny);
    glutAddMenuEntry("Fioletowy", fioletowy);
    glutAddMenuEntry("Bialy", bialy);
    glutAddMenuEntry("Blekitny", blekitny);
}

void createMenus() {
    int houseMenu = glutCreateMenu(pointlessFunction);
    int floorMenu = glutCreateMenu(menu_setFloorColor);
    int skyMenu = glutCreateMenu(menu_setSkyColor);
    addColorMenuEntries();

    glutSetMenu(floorMenu);
    addColorMenuEntries();

    glutSetMenu(houseMenu);
    int houseSizeMenu = glutCreateMenu(menu_setHouseSize);
    glutAddMenuEntry("1",1);
    glutAddMenuEntry("2",2);
    glutAddMenuEntry("3",3);
    int houseShapeMenu = glutCreateMenu(menu_setHouseShape);
    glutAddMenuEntry("Kosc",cube);
    glutAddMenuEntry("Czajnik",teapot);
    glutAddMenuEntry("Kula", sphere);
    int wallColorMenu = glutCreateMenu(menu_setWallColor);
    addColorMenuEntries();
    int roofColorMenu = glutCreateMenu(menu_setRoofColor);
    addColorMenuEntries();

    glutSetMenu(houseMenu);
    glutAddSubMenu("Zmien rozmiar", houseSizeMenu);
    glutAddSubMenu("Zmien ksztalt", houseShapeMenu);
    glutAddSubMenu("Zmien kolor scian", wallColorMenu);
    glutAddSubMenu("Zmien kolor dachu", roofColorMenu);

    glutCreateMenu(pointlessFunction);

    glutAddSubMenu("Zmien ziemie",floorMenu);
    glutAddSubMenu("Zmien tlo", skyMenu);
    glutAddSubMenu("Zmien domki", houseMenu);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void pointlessFunction(int a) {}
