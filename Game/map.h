#ifndef MAP_H
#define MAP_H

#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>

void drawSquare(float x, float y, float size, float r, float g, float b);
void drawStone(void);
void drawTree(void);
void generateMapList(void);
void drawMap(void);

const int NUM_OBJECTS = 500;

const int MAX_CUBE_SIZE = 5;


#endif // MAP_H
