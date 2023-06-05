#include <GL/glut.h>
#include <iostream>
#include "map.h"
#include "drawpers.h"
#include "drawattribute.h"
#include <cstdlib>
#include <ctime>
#include "interface.h"
#include <algorithm>

struct Position {
    float x, y;
};

struct Figur {
    Position pos;
    bool isRight;
    GLfloat health;
} figur;

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



const int MAX_KNIVES = 10;
Knife knives[MAX_KNIVES];
int knifeSpawnDelay = 700; // Задержка между спавном ножей
int lastKnifeSpawnTime = 0; // Время последнего спавна ножа


int napr_nog1 = 0;
int napr_nog2 = 0;

const int MAX_ENEMIES = 100;
Enemy enemies[MAX_ENEMIES];

Position pos = {0.0, 0.0};

bool isGameOver = false;
bool immune = false;

float legStep = 15; //поворота ноги
float legAngle = 0;

float mapMinX = -10.0;
float mapMaxX = 10.0;
float mapMinY = -10.0;
float mapMaxY = 10.0;

void renderScene(void);
void geralt();
void cirila();
void yovert();
void drawMap();
void leg1g();
void leg2g();
void ghost();
void wildhunt();
void processKeys(unsigned char key, int x, int y);
void drawEnemies();
void drawGameOver();
void drawHealthBar();
void update(int value);
void generateRandomEnemies();
bool checkCollision(Position pos1, Position pos2, float radius1, float radius2);
void handleCollisions();
void drawUI();
void legTimer(int value);
void drawKnives();


int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Game");
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(processKeys);
    glClearColor(0, 0, 0, 0);

    //начальные значений игрока
    figur.pos.x = 0.0f;
    figur.pos.y = 0.0f;

    pos.x = 0.0f;
    pos.y = 0.0f;
    figur.health=10;

    //начальные значений мобов
    for (int i = 0; i < MAX_ENEMIES; ++i) {
        enemies[i].pos.x = 0.05;
        enemies[i].pos.y = 0;
        enemies[i].health = 3;
    }

    //генерация рандомных мобов
    generateRandomEnemies();

    glutTimerFunc(16, update, 0);   // движение мобов
    glutTimerFunc(1, legTimer, 0);

    glutMainLoop();
    return 0;
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (isGameOver) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glPushMatrix();
        drawGameOver();
        glPopMatrix();
    } else {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Перемещение камеры
        glTranslatef(-figur.pos.x, -figur.pos.y, 0.0f);
        drawMap();
        //границы
        if (figur.pos.x < mapMinX) {
            figur.pos.x = mapMinX; // Ограничение движения по X влево
        } else if (figur.pos.x > mapMaxX) {
            figur.pos.x = mapMaxX; // Ограничение движения по X вправо
        }

        if (figur.pos.y < mapMinY) {
            figur.pos.y = mapMinY; // Ограничение движения по Y вниз
        } else if (figur.pos.y > mapMaxY) {
            figur.pos.y = mapMaxY; // Ограничение движения по Y вверх
        }
        drawUI();

        glPushMatrix();
        glTranslatef(figur.pos.x, figur.pos.y, 0.0f);
        if (figur.isRight) {
            glRotatef(180, 0.0f, 1.0f, 0.0f);
        } else {
            glRotatef(0, 0.0f, 1.0f, 0.0f);
        }
        glScalef(0.3, 0.3, 0.3);
        glPushMatrix();
        geralt();
        //ciri
        //yovert
        glPushMatrix();
        glRotatef(legAngle, 0.0, 0.0, 1.0);

        if (legAngle >= -45.0f) {
            napr_nog1 = -1;
        } else if (legAngle <= 45.0f) {
            napr_nog1 = 1;
        }
        leg1g();
        if (napr_nog1 == 1) {
            glTranslatef(0.0, 0.0, 0.0);
            glRotatef(legAngle, 0.0, 0.0, 1.0);
        }
        if (napr_nog1 == -1) {
            glRotatef(-legAngle, 0.0, 0.0, 1.0);  // Возвращение ноги в исходное положение
            glTranslatef(0.0, 0.0, 0.0);
        }
        leg2g();
        if (napr_nog1 == 1) {
            glTranslatef(0.0, 0.0, 0.0);
            glRotatef(-legAngle, 0.0, 0.0, 1.0);
        }
        if (napr_nog1 == -1) {
            glRotatef(legAngle, 0.0, 0.0, 1.0);  // Возвращение ноги в исходное положение
            glTranslatef(0.0, 0.0, 0.0);
        }
        glPopMatrix();
        drawEnemies();
        glPushMatrix();
        drawKnives();
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();

        drawHealthBar();
    }

    glutSwapBuffers();
}
void processKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            figur.pos.y += 0.1;
            legAngle = 0;
            legAngle += legStep;
            break;
        case 's':
            figur.pos.y -= 0.1;
            legAngle = 0;
            legAngle += legStep;
            break;
        case 'a':
            figur.pos.x -= 0.1;
            figur.isRight = false;
            napr_nog1 = 1;
            legAngle += legStep;
            break;
        case 'd':
            figur.pos.x += 0.1;
            figur.isRight = true;
            napr_nog1 = -1;
            legAngle += legStep;
            break;
            for (int i = 0; i < MAX_KNIVES; ++i) {
                if (!knives[i].isAlive) {
                    knives[i].pos = figur.pos;
                    knives[i].isAlive = true;
                    knives[i].isRight = figur.isRight;
                    break;
                }
            }
            break;

        default:
            return;

    glutPostRedisplay();
}

}
void legTimer(int value) {
    glutPostRedisplay();

    // Ограничение угла
    if (legAngle > 45.0f) {
        legAngle = -15;
    }

    // Переключение направления ноги
    if (legAngle >= 45.0f || legAngle <= -15) {
        legStep = -legStep;
    }

    glutTimerFunc(10, legTimer, 0);
}
void spawnRandomEnemy(void) {
    int randomNum = rand() % 2; // генерация случайных видов мобов

    if (randomNum == 0) {
        wildhunt();
    } else {
        ghost();
    }
}

void drawEnemies(void) {
    srand(42);

    for (int i = 0; i < MAX_ENEMIES; ++i) {
        if (enemies[i].health > 0) {
        glPushMatrix();
        glTranslatef(enemies[i].pos.x, enemies[i].pos.y, 0.0f);
        glScalef(0.4, 0.4, 0.4);

        if (enemies[i].isRight) {
            glRotatef(180, 0.0f, 1.0f, 0.0f);
        }

        spawnRandomEnemy();
}
        glPopMatrix();
    }
}

bool checkCollision(Position pos1, Position pos2, float radius1, float radius2) {
    float distance = sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
    return distance <= radius1 + radius2;
}

void handleCollisions(void) {
    for (int i = 0; i < MAX_ENEMIES; ++i) {
        if (checkCollision(figur.pos, enemies[i].pos, 0.1f, 0.1f)) {
            float enemyDirectionX = figur.pos.x - enemies[i].pos.x;
            float enemyDirectionY = figur.pos.y - enemies[i].pos.y;
            figur.pos.x += enemyDirectionX * 0.05;
            figur.pos.y += enemyDirectionY * 0.05;

            figur.health -= 0.5f; // Уменьшение здоровья персонажа
            if (figur.health <= 0.0f) {
                isGameOver = true; // Игра окончена, если здоровье кончилось
            }
        }

        for (int j = i + 1; j < MAX_ENEMIES; ++j) {
            if (checkCollision(enemies[i].pos, enemies[j].pos, 0.09f, 0.09f)) {
                float enemyDirectionX = enemies[i].pos.x - enemies[j].pos.x;
                float enemyDirectionY = enemies[i].pos.y - enemies[j].pos.y;
                enemies[i].pos.x += enemyDirectionX * 0.3f;
                enemies[i].pos.y += enemyDirectionY * 0.3f;

                enemyDirectionX = enemies[j].pos.x - enemies[i].pos.x;
                enemyDirectionY = enemies[j].pos.y - enemies[i].pos.y;
                enemies[j].pos.x += enemyDirectionX * 0.2f;
                enemies[j].pos.y += enemyDirectionY * 0.2f;
            }
        }
    }
}

void update(int value) {
    for (int i = 0; i < MAX_ENEMIES; ++i) {
        // Определение направлениия мобов к персонажу
        float directionX = figur.pos.x - enemies[i].pos.x;
        float directionY = figur.pos.y - enemies[i].pos.y;
        float length = sqrt(directionX * directionX + directionY * directionY);
        directionX /= length;
        directionY /= length;

        float speed = 0.02;
        float offsetX = speed * directionX;
        float offsetY = speed * directionY;

        // позиция моба изменяется
        enemies[i].pos.x += offsetX;
        enemies[i].pos.y += offsetY;

        if (directionX > 0) {
            enemies[i].isRight = false;
        } else {
            enemies[i].isRight = true;
        }
    }
    //статичная анимация для персонажа
    static bool moveUp = true;
    static float delta = 0.005f;

    if (moveUp) {
        figur.pos.y += delta;
    } else {
        figur.pos.y -= delta;
    }

    if (figur.pos.y >= 0.001f || figur.pos.y <= -0.001f) {
        moveUp = !moveUp;
    }
    //статичная для мобов
    static bool mobMoveUp[MAX_ENEMIES] = { true };
    static float deltam = 0.01f;

    for (int i = 0; i < MAX_ENEMIES; ++i) {
        if (mobMoveUp[i]) {
            enemies[i].pos.y += deltam;
        } else {
            enemies[i].pos.y -= deltam;
        }

        if (enemies[i].pos.y >= 0.03f || enemies[i].pos.y <= -0.03f) {
            mobMoveUp[i] = !mobMoveUp[i];
        }
    }
    for (int i = 0; i < MAX_KNIVES; ++i) {
         if (knives[i].isAlive) {
             knives[i].pos.x += 0.1; //обновления позиции ножа

             // Проверка столкновений ножа с врагами
             for (int j = 0; j < MAX_ENEMIES; ++j) {
                 if (enemies[j].health > 0 && checkCollision(knives[i].pos, enemies[j].pos, 0.05, 0.05) && !immune) {
                     enemies[j].health = 0; // Убийство врага
                     knives[i].isAlive = false;
                     immune = true; // Устанавливаем иммунитет для игрока (Долго же я не понимал почему просто так сливаюсь)
                     break;
                 }
             }
         }
     }
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if (currentTime - lastKnifeSpawnTime >= knifeSpawnDelay) {
        // Создание нового ножа
        for (int i = 0; i < MAX_KNIVES; ++i) {
            if (!knives[i].isAlive) {
                knives[i].pos = figur.pos;
                knives[i].isAlive = true;
                knives[i].isRight = figur.isRight;
                break;
            }
        }
        lastKnifeSpawnTime = currentTime; // Обновление времени последнего спавна ножа
    }
    for (int i = 0; i < MAX_KNIVES; ++i) {
        if (knives[i].isAlive) {
            Enemy* target = nullptr;
            float minDistance = std::numeric_limits<float>::max();

            // Поиск ближайшего врага
            for (int j = 0; j < MAX_ENEMIES; ++j) {
                if (enemies[j].health > 0) {
                    float distance = std::sqrt(std::pow(enemies[j].pos.x - knives[i].pos.x, 2) +
                                               std::pow(enemies[j].pos.y - knives[i].pos.y, 2));
                    if (distance < minDistance) {
                        minDistance = distance;
                        target = &enemies[j];
                    }
                }
            }
            // Обновление позиции ножа в направлении моба(цели)
            if (target) {
                float dx = target->pos.x - knives[i].pos.x;
                float dy = target->pos.y - knives[i].pos.y;
                float distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
                float speed = 0.1f;

                if (distance > 0.01f) {
                    knives[i].pos.x += speed * dx / distance;
                    knives[i].pos.y += speed * dy / distance;
                } else {
                    target->health = 0;
                    knives[i].isAlive = false;
                }
            }
        }
    }
    static int immuneTime = 3000;
    static int lastImmuneTime = 0;


    if (currentTime - lastImmuneTime >= immuneTime) {
        immune = false;
        lastImmuneTime = currentTime;
    }


    // Планирование следующего обновления
    glutTimerFunc(10, update, 0);

        handleCollisions();

    glutPostRedisplay();
}

void drawKnives() {
    for (int i = 0; i < MAX_KNIVES; ++i) {
        if (knives[i].isAlive) {
            glPushMatrix();
            glTranslatef(knives[i].pos.x, knives[i].pos.y, 0.0f);
            knife();
            glEnd();
            glPopMatrix();
        }
    }
}

void drawGameOver(void) {
    glPushMatrix();
    glScalef(1, 1, 1);
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.15f, 0.1f);
    const char* text = "Game Over!";
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
    glRasterPos2f(-0.15f, 0.1f);
    glPopMatrix();
}

void drawHealthBar(void) {
    glPushMatrix();
    glTranslatef(figur.pos.x, figur.pos.y+0.15, 0.0f);
    glScalef(0.2, 0.2, 0.2);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); //здоровье

    glVertex2f(-0.3f, 0.2f);
    glVertex2f(0.3f, 0.2f);
    glVertex2f(0.3f, 0.25f);
    glVertex2f(-0.3f, 0.25f);

    glColor3f(0.0f, 1.0f, 0.0f);

    //значения health
    glVertex2f(-0.3f, 0.2f);
    glVertex2f(figur.health * 0.2f - 0.3f, 0.2f);
    glVertex2f(figur.health * 0.2f - 0.3f, 0.25f);
    glVertex2f(-0.3f, 0.25f);

    glEnd();

    glPopMatrix();
}

void generateRandomEnemies(void) {          //случайная генерация мобов
    std::srand(std::time(nullptr));

    for (int i = 0; i < MAX_ENEMIES; ++i) {
        float posX, posY;
        bool overlap;

        do {
            overlap = false;
            posX = static_cast<float>(std::rand() % 20) - 10;
            posY = static_cast<float>(std::rand() % 20) - 10;

            for (int j = 0; j < i; ++j) {
                if (checkCollision({posX, posY}, enemies[j].pos, 0.4f, 0.4f)) {
                    overlap = true;
                    break;
                }
            }
        } while (overlap);

        enemies[i].pos.x = posX;
        enemies[i].pos.y = posY;
    }
}
