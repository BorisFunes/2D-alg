#include <GL/glut.h>
#include <math.h>

// Función para  círculos
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

// Función para  rectángulos
void drawRect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Función para  contorno de rectángulo
void drawRectOutline(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Función para  rueda completa
void drawWheel(float x, float y, float radius, float innerRadius)
{
    // negra
    glColor3f(0.1f, 0.1f, 0.1f);
    drawCircle(x, y, radius, 30);
    // Centro gris
    glColor3f(0.5f, 0.5f, 0.5f);
    drawCircle(x, y, innerRadius, 30);
}

// dibujar múltiples ventanas en fila
void drawWindows(float startX, float y, float width, float height, int count, float spacing, float r, float g, float b)
{
    glColor3f(r, g, b);
    for (int i = 0; i < count; i++)
    {
        float x = startX + i * spacing;
        drawRect(x, y, x + width, y + height);
    }
}

// dibujar contornos de múltiples ventanas
void drawWindowsOutline(float startX, float y, float width, float height, int count, float spacing)
{
    for (int i = 0; i < count; i++)
    {
        float x = startX + i * spacing;
        drawRectOutline(x, y, x + width, y + height);
    }
}

// dibujar luces delante y atras
void drawLight(float x, float y, float width, float height, float r, float g, float b)
{
    glColor3f(r, g, b);
    drawRect(x, y, x + width, y + height);
}

//  lIneas horizontales decorativas
void drawHorizontalLines(float x, float y, float width, int count, float spacing)
{
    glBegin(GL_LINES);
    for (int i = 0; i < count; i++)
    {
        float lineY = y + i * spacing;
        glVertex2f(x, lineY);
        glVertex2f(x + width, lineY);
    }
    glEnd();
}
////////////// ****** MOTO PARA DELIVERY ******* ////////
void drawDelivery(int x, int y)
{
    // ruedas
    drawWheel(x + 40, y, 8, 5);
    drawWheel(x, y, 8, 5);
    glColor3f(0.3f, 0.3f, 0.3f);
    drawCircle(x + 40, y, 2, 8);
    drawCircle(x, y, 2, 8);

    // Cuerpo
    glColor3f(0.9f, 0.1f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 42, y);
    glVertex2f(x + 35, y + 8);
    glVertex2f(x + 5, y + 8);
    glVertex2f(x - 2, y);
    glEnd();

    // Tanque
    glColor3f(0.8f, 0.05f, 0.05f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 25, y + 8);
    glVertex2f(x + 15, y + 8);
    glVertex2f(x + 12, y + 18);
    glVertex2f(x + 28, y + 18);
    glEnd();

    // Asiento
    glColor3f(0.1f, 0.1f, 0.1f);
    drawRect(x + 8, y + 8, x + 32, y + 16);

    // Manubrio
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(x, y + 8);
    glVertex2f(x - 5, y + 15);
    glVertex2f(x - 8, y + 15);
    glVertex2f(x - 2, y + 15);
    glEnd();

    // Caja de delivery
    glColor3f(1.0f, 0.85f, 0.1f);
    drawRect(x + 26, y + 16, x + 44, y + 32);
    glColor3f(0.8f, 0.0f, 0.0f);
    drawRect(x + 29, y + 22, x + 41, y + 26);

    // Pedales
    glColor3f(0.2f, 0.2f, 0.2f);
    drawRect(x + 18, y - 3, x + 22, y - 1);

    glLineWidth(1.0f);
}
////////////// ****** CARRO KIA SOUL ******* ////////
void drawCar(int x, int y)
{
    glColor3f(0.8f, 0.2f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 70, y + 15);
    glVertex2f(x + 10, y + 15);
    glVertex2f(x + 10, y + 45);
    glVertex2f(x + 15, y + 50);
    glVertex2f(x + 65, y + 50);
    glVertex2f(x + 70, y + 45);
    glEnd();

    glColor3f(0.6f, 0.7f, 0.9f);
    // Parabrisas
    glBegin(GL_QUADS);
    glVertex2f(x + 10, y + 35);
    glVertex2f(x + 20, y + 35);
    glVertex2f(x + 18, y + 48);
    glVertex2f(x + 12, y + 48);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(x + 60, y + 35);
    glVertex2f(x + 70, y + 35);
    glVertex2f(x + 68, y + 48);

    glEnd();

    // Ventanas
    drawRect(x + 25, y + 35, x + 55, y + 48);
    glColor3f(0.8f, 0.2f, 0.2f);
    drawRect(x + 38, y + 35, x + 42, y + 48);

    // Ruedas
    drawWheel(x + 20, y + 15, 10, 5);
    drawWheel(x + 60, y + 15, 10, 5);

    // Luces
    drawLight(x + 8, y + 20, 4, 8, 1.0f, 1.0f, 0.8f);
    drawLight(x + 68, y + 20, 4, 8, 1.0f, 0.2f, 0.2f);

    // Contornos
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x + 10, y + 35);
    glVertex2f(x + 20, y + 35);
    glVertex2f(x + 18, y + 48);
    glVertex2f(x + 12, y + 48);
    glEnd();

    drawRectOutline(x + 25, y + 35, x + 55, y + 48);

    // Línea del separador
    glBegin(GL_LINES);
    glVertex2f(x + 40, y + 35);
    glVertex2f(x + 40, y + 48);
    glEnd();

    glLineWidth(1.0f);
}
////////////// ****** MICROBUS TIPPO COASTER ******* ////////
void drawCoaster(int x, int y)
{
    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 100, y + 20);
    glVertex2f(x, y + 20);
    glVertex2f(x, y + 70);
    glVertex2f(x + 10, y + 85);
    glVertex2f(x + 90, y + 85);
    glVertex2f(x + 100, y + 70);
    glEnd();

    // Franja
    glColor3f(0.2f, 0.4f, 0.8f);
    drawRect(x, y + 35, x + 100, y + 45);

    // Parabrisas delantero
    glColor3f(0.7f, 0.8f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 25, y + 50);
    glVertex2f(x + 15, y + 50);
    glVertex2f(x + 15, y + 75);
    glVertex2f(x + 20, y + 82);
    glVertex2f(x + 25, y + 80);
    glEnd();

    // Ventanas del medio
    drawWindows(x + 32, y + 50, 18, 25, 2, 20, 0.7f, 0.8f, 0.95f);

    // Ruedas
    drawWheel(x + 80, y + 20, 12, 6);
    drawWheel(x + 25, y + 20, 12, 6);

    // Luces
    drawLight(x, y + 38, 5, 7, 1.0f, 1.0f, 0.3f);      // adelante
    drawLight(x + 97, y + 38, 3, 7, 1.0f, 0.2f, 0.2f); // atras
    glColor3f(0.3f, 0.3f, 0.3f);
    drawRect(x + 13, y + 55, x + 18, y + 60); // espejo

    // techo
    glColor3f(0.4f, 0.4f, 0.4f);
    drawHorizontalLines(x + 10, y + 80, 80, 2, 2);

    // Contornos
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_STRIP);
    glVertex2f(x + 100, y + 20);
    glVertex2f(x, y + 20);
    glVertex2f(x, y + 70);
    glVertex2f(x + 10, y + 85);
    glVertex2f(x + 90, y + 85);
    glVertex2f(x + 100, y + 70);
    glVertex2f(x + 100, y + 20);
    glEnd();

    // Contornos de ventanas
    glLineWidth(0.5f);
    drawWindowsOutline(x + 32, y + 50, 18, 25, 2, 20);

    // Líneas de franja decorativa
    glBegin(GL_LINES);
    glVertex2f(x + 100, y + 35);
    glVertex2f(x, y + 35);
    glVertex2f(x + 100, y + 45);
    glVertex2f(x, y + 45);
    glEnd();

    glLineWidth(1.0f);
}
////////////// ****** BUS  ******* ////////
void drawBus(int x, int y)
{
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 140, y + 15);
    glVertex2f(x, y + 15);
    glVertex2f(x, y + 55);
    glVertex2f(x + 10, y + 70);
    glVertex2f(x + 130, y + 70);
    glVertex2f(x + 140, y + 55);
    glEnd();

    // Franja inferior naranja
    glColor3f(1.0f, 0.5f, 0.1f);
    drawRect(x, y + 15, x + 140, y + 35);

    // Parabrisas delantero
    glColor3f(0.7f, 0.8f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f(x + 20, y + 35);
    glVertex2f(x, y + 35);
    glVertex2f(x, y + 55);
    glVertex2f(x + 10, y + 70);
    glVertex2f(x + 20, y + 65);
    glEnd();

    // ventanas
    drawWindows(x + 35, y + 45, 20, 18, 4, 25, 0.7f, 0.8f, 0.95f);

    // ruedas
    drawWheel(x + 115, y + 15, 18, 10);
    drawWheel(x + 25, y + 15, 18, 10);

    // faros y luces
    drawLight(x, y + 20, 5, 10, 1.0f, 1.0f, 0.3f);
    drawLight(x + 137, y + 20, 3, 8, 1.0f, 0.2f, 0.2f);

    // Contornos
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    // Contornos de ventanas
    glLineWidth(1.5f);
    drawWindowsOutline(x + 35, y + 45, 20, 18, 4, 25);

    // Contorno del parabrisas
    glBegin(GL_LINE_LOOP);
    glVertex2f(x + 20, y + 35);
    glVertex2f(x, y + 35);
    glVertex2f(x, y + 55);
    glVertex2f(x + 10, y + 70);
    glVertex2f(x + 20, y + 65);
    glEnd();
    glLineWidth(1.0f);
}
