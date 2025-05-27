#include <GL/glut.h>
#include <math.h>
#include "header.h"

// dibuja un circulo con lineas
void drawCircle(float cx, float cy, float r, int segments)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++)
    {
        float theta = 2.0f * 3.1415926f * i / segments;
        float dx = r * cosf(theta);
        float dy = r * sinf(theta);
        glVertex2f(cx + dx, cy + dy);
    }
    glEnd();
}

// dibuja rectangulo relleno
void drawRect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// borde para dibujado del rectangulo
void drawRectOutline(float x1, float y1, float x2, float y2)
{
    drawLineBresenham((int)x1, (int)y1, (int)x2, (int)y1);
    drawLineBresenham((int)x2, (int)y1, (int)x2, (int)y2);
    drawLineBresenham((int)x2, (int)y2, (int)x1, (int)y2);
    drawLineBresenham((int)x1, (int)y2, (int)x1, (int)y1);
}
// circulo de relleno con el algoritmo de punto medio
void drawFilledCircleMidpoint(int cx, int cy, int radius)
{
    int x = radius;
    int y = 0;
    int err = 0;

    glBegin(GL_LINES);
    while (x >= y)
    {
        glVertex2i(cx - x, cy + y);
        glVertex2i(cx + x, cy + y);
        glVertex2i(cx - x, cy - y);
        glVertex2i(cx + x, cy - y);
        glVertex2i(cx - y, cy + x);
        glVertex2i(cx + y, cy + x);
        glVertex2i(cx - y, cy - x);
        glVertex2i(cx + y, cy - x);

        if (err <= 0)
        {
            y += 1;
            err += 2 * y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
    glEnd();
}
// dibuja un rueda con centro
void drawWheel(float x, float y, float radius, float innerRadius)
{

    glColor3f(0.1f, 0.1f, 0.1f);
    drawFilledCircleMidpoint((int)x, (int)y, (int)radius);

    glColor3f(0.5f, 0.5f, 0.5f);
    drawFilledCircleMidpoint((int)x, (int)y, (int)innerRadius);
}

// dibuja varias ventanas seguidas
void drawWindows(float startX, float y, float width, float height, int count, float spacing, float r, float g, float b)
{
    glColor3f(r, g, b);
    for (int i = 0; i < count; i++)
    {
        float x = startX + i * spacing;
        drawRect(x, y, x + width, y + height);
    }
}

// contorno de ventanas
void drawWindowsOutline(float startX, float y, float width, float height, int count, float spacing)
{
    for (int i = 0; i < count; i++)
    {
        float x = startX + i * spacing;
        drawRectOutline(x, y, x + width, y + height);
    }
}

// luz para los vehiculos, ya sea delantera o trasera
void drawLight(float x, float y, float width, float height, float r, float g, float b)
{
    glColor3f(r, g, b);
    drawRect(x, y, x + width, y + height);
}

// dibuja lineas horizontales de decoracion
void drawHorizontalLines(float x, float y, float width, int count, float spacing)
{
    for (int i = 0; i < count; i++)
    {
        float lineY = y + i * spacing;
        drawLineBresenham((int)x, (int)lineY, (int)(x + width), (int)lineY);
    }
}

////////////// ****** MOTO PARA DELIVERY ******* ////////
void drawDelivery(int x, int y)
{
    // ruedas y cuerpo
    drawWheel(x + 2, y, 8, 5);
    drawWheel(x + 42, y, 8, 5);
    glColor3f(0.3f, 0.3f, 0.3f);
    drawCircle(x + 2, y, 2, 8);
    drawCircle(x + 42, y, 2, 8);

    glColor3f(0.9f, 0.1f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + 7, y + 8);
    glVertex2f(x + 37, y + 8);
    glVertex2f(x + 44, y);
    glEnd();

    // asiento y manubrio
    glColor3f(0.8f, 0.05f, 0.05f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 17, y + 8);
    glVertex2f(x + 27, y + 8);
    glVertex2f(x + 30, y + 18);
    glVertex2f(x + 14, y + 18);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);
    drawRect(x + 10, y + 8, x + 34, y + 16);

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(x + 42, y + 8);
    glVertex2f(x + 47, y + 15);
    glVertex2f(x + 44, y + 15);
    glVertex2f(x + 50, y + 15);
    glEnd();

    // caja y pedales
    glColor3f(1.0f, 0.85f, 0.1f);
    drawRect(x - 2, y + 16, x + 16, y + 32);
    glColor3f(0.8f, 0.0f, 0.0f);
    drawRect(x + 1, y + 22, x + 13, y + 26);

    glColor3f(0.2f, 0.2f, 0.2f);
    drawRect(x + 20, y - 3, x + 24, y - 1);

    glLineWidth(1.0f);
}
////////////// ****** CARRO KIA SOUL ******* ////////
void drawCar(int x, int y)
{
    glColor3f(0.8f, 0.2f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 10, y + 15);
    glVertex2f(x + 70, y + 15);
    glVertex2f(x + 70, y + 45);
    glVertex2f(x + 65, y + 50);
    glVertex2f(x + 15, y + 50);
    glVertex2f(x + 10, y + 45);
    glEnd();

    glColor3f(0.6f, 0.7f, 0.9f);
    // Parabrisas
    glBegin(GL_QUADS);
    glVertex2f(x + 70, y + 35);
    glVertex2f(x + 60, y + 35);
    glVertex2f(x + 62, y + 48);
    glVertex2f(x + 68, y + 48);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(x + 20, y + 35);
    glVertex2f(x + 10, y + 35);
    glVertex2f(x + 12, y + 48);
    glVertex2f(x + 18, y + 48);
    glEnd();

    // ventanas y ruedas
    drawRect(x + 25, y + 35, x + 55, y + 48);
    glColor3f(0.8f, 0.2f, 0.2f);
    drawRect(x + 38, y + 35, x + 42, y + 48);

    drawWheel(x + 20, y + 15, 10, 5);
    drawWheel(x + 60, y + 15, 10, 5);

    drawLight(x + 68, y + 20, 4, 8, 1.0f, 1.0f, 0.8f);
    drawLight(x + 8, y + 20, 4, 8, 1.0f, 0.2f, 0.2f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x + 70, y + 35);
    glVertex2f(x + 60, y + 35);
    glVertex2f(x + 62, y + 48);
    glVertex2f(x + 68, y + 48);
    glEnd();

    drawRectOutline(x + 25, y + 35, x + 55, y + 48);
    drawLineBresenham((int)(x + 40), (int)(y + 35), (int)(x + 40), (int)(y + 48));

    glLineWidth(1.0f);
}
////////////// ****** MICROBUS TIPPO COASTER ******* ////////
void drawCoaster(int x, int y)
{
    // cuerpoPrincipal
    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f(x, y + 20);
    glVertex2f(x + 100, y + 20);
    glVertex2f(x + 100, y + 70);
    glVertex2f(x + 90, y + 85);
    glVertex2f(x + 10, y + 85);
    glVertex2f(x, y + 70);
    glEnd();

    glColor3f(0.2f, 0.4f, 0.8f);
    drawRect(x, y + 35, x + 100, y + 45);

    glColor3f(0.7f, 0.8f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 75, y + 50);
    glVertex2f(x + 85, y + 50);
    glVertex2f(x + 85, y + 75);
    glVertex2f(x + 80, y + 82);
    glVertex2f(x + 75, y + 80);
    glEnd();

    // ventanas y ruedas
    drawWindows(x + 50, y + 50, 18, 25, 2, -20, 0.7f, 0.8f, 0.95f);

    drawWheel(x + 20, y + 20, 12, 6);
    drawWheel(x + 75, y + 20, 12, 6);

    // Luces y detalles
    drawLight(x + 95, y + 38, 5, 7, 1.0f, 1.0f, 0.3f);
    drawLight(x, y + 38, 3, 7, 1.0f, 0.2f, 0.2f);
    glColor3f(0.3f, 0.3f, 0.3f);
    drawRect(x + 82, y + 55, x + 87, y + 60);

    glColor3f(0.4f, 0.4f, 0.4f);
    drawHorizontalLines(x + 10, y + 80, 80, 2, 2);

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_STRIP);
    glVertex2f(x, y + 20);
    glVertex2f(x + 100, y + 20);
    glVertex2f(x + 100, y + 70);
    glVertex2f(x + 90, y + 85);
    glVertex2f(x + 10, y + 85);
    glVertex2f(x, y + 70);
    glVertex2f(x, y + 20);
    glEnd();

    glLineWidth(0.5f);
    drawWindowsOutline(x + 50, y + 50, 18, 25, 2, -20);

    glBegin(GL_LINES);
    glVertex2f(x, y + 35);
    glVertex2f(x + 100, y + 35);
    glVertex2f(x, y + 45);
    glVertex2f(x + 100, y + 45);
    glEnd();

    glLineWidth(1.0f);
}
////////////// ****** BUS  ******* ////////
void drawBus(int x, int y)
{
    // cuerpoBus
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(x, y + 15);
    glVertex2f(x + 140, y + 15);
    glVertex2f(x + 140, y + 55);
    glVertex2f(x + 130, y + 70);
    glVertex2f(x + 10, y + 70);
    glVertex2f(x, y + 55);
    glEnd();

    glColor3f(1.0f, 0.5f, 0.1f);
    drawRect(x, y + 15, x + 140, y + 35);

    glColor3f(0.7f, 0.8f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 120, y + 35);
    glVertex2f(x + 140, y + 35);
    glVertex2f(x + 140, y + 55);
    glVertex2f(x + 130, y + 70);
    glVertex2f(x + 120, y + 65);
    glEnd();

    // ventanas,ruedas,lucess
    drawWindows(x + 90, y + 45, 20, 18, 4, -25, 0.7f, 0.8f, 0.95f);

    drawWheel(x + 25, y + 15, 18, 10);
    drawWheel(x + 115, y + 15, 18, 10);

    drawLight(x + 135, y + 20, 5, 10, 1.0f, 1.0f, 0.3f);
    drawLight(x, y + 20, 3, 8, 1.0f, 0.2f, 0.2f);

    // Contornos
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    glLineWidth(1.5f);
    drawWindowsOutline(x + 90, y + 45, 20, 18, 4, -25);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x + 120, y + 35);
    glVertex2f(x + 140, y + 35);
    glVertex2f(x + 140, y + 55);
    glVertex2f(x + 130, y + 70);
    glVertex2f(x + 120, y + 65);
    glEnd();
    glLineWidth(1.0f);
}