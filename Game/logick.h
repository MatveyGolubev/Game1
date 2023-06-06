#ifndef LOGICK_H
#define LOGICK_H

#include <GL/glut.h>
#include <iostream>
#include "map.h"
#include "drawpers.h"
#include "drawattribute.h"
#include <cstdlib>
#include <ctime>
#include "interface.h"
#include <algorithm>
#include "structures.h"

bool isGameOver = false;
bool immune = false;

float legStep = 15; //поворота ноги
float legAngle = 0;

#endif // LOGICK_H
