#include <GL/glut.h>

void drawBus(float x, float y)
{
    // cuerpo bus
    glColor3f(0.0f, 0.5f, 1.0f); // amarillo
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 100, y);
    glVertex2f(x + 100, y + 50);
    glVertex2f(x, y + 50);
    glEnd();

    // ventanas
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 3; i++)
    {
        float wx = x + 10 + i * 30;
        float wy = y + 20;
        glBegin(GL_QUADS);
        glVertex2f(wx, wy);
        glVertex2f(wx + 20, wy);
        glVertex2f(wx + 20, wy + 15);
        glVertex2f(wx, wy + 15);
        glEnd();
    }

    // num
    glColor3f(1.0f, 1.0f, 1.0f); 
    glBegin(GL_LINE_LOOP);
    glVertex2f(x + 5, y + 44);
    glVertex2f(x + 15, y + 44);
    glVertex2f(x + 15, y + 48);
    glVertex2f(x + 5, y + 48);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(x + 5, y + 40);
    glVertex2f(x + 15, y + 40);
    glVertex2f(x + 15, y + 44);
    glVertex2f(x + 5, y + 44);
    glEnd();

    // ruedas
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(15);
    glBegin(GL_POINTS);
    glVertex2f(x + 20, y);
    glVertex2f(x + 80, y);
    glEnd();
}
