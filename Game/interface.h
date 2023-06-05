#ifndef INTERFACE_H                             //не доделано, работает только таймер
#define INTERFACE_H

#include <GL/glut.h>
#include <string>
#include <ctime>

// Глобальные переменные
int windowWidth = 800;
int windowHeight = 800;
int score = 0;
int health = 100;
time_t startTime;

//Отрисовка текста
void drawText(const std::string& text, float x, float y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0, 0.84, 0); // Золотой цвет (Gold)
    glRasterPos2f(x, y);

    // Увеличение шрифта
    glScalef(3, 3, 2.5);

    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawUI() {
    // Расчет прошедшего времени
    clock_t currentTime = clock();
    double elapsedTime = (currentTime - startTime) / (double)CLOCKS_PER_SEC * 1000;

    // Расчет минут и секунд
    int minutes = static_cast<int>(elapsedTime / 60000);
    int seconds = static_cast<int>((elapsedTime - minutes * 60000) / 1000);

    // Форматирование времени
    std::string timerText = "Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    drawText(timerText, 10, 10);

    // Отрисовка очков
    std::string scoreText = "Score: " + std::to_string(score);
    drawText(scoreText, windowWidth / 2 - 40, 10);

    // Отрисовка жизней
    std::string livesText = "Health: " + std::to_string(health);
    drawText(livesText, windowWidth - 80, 10);
}

#endif // INTERFACE_H
