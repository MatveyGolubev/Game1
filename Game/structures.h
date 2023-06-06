#ifndef STRUCTUR_H
#define STRUCTUR_H
#include <GL/glut.h>

struct Position {
    float x, y;
};

struct Figur {
    Position pos;
    bool isRight;
    GLfloat health;
};

struct Enemy {
    Position pos;
    bool isRight;
    GLfloat health;
};

struct Knife {
    Position pos;
    bool isAlive;
    bool isRight;
};

extern Figur figur;

#endif // STRUCTUR_H
