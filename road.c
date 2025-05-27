#include <GL/glut.h>
#include <math.h>
#include "header.h"

// ========== DECLARACIONES DE FUNCIONES ==========
void drawLineBresenham(int x0, int y0, int x1, int y1);
void drawLineDDA(int x0, int y0, int x1, int y1);
void drawThickLine(int x0, int y0, int x1, int y1, int thickness);
void drawLetterL(int x, int y);
void drawLetterO(int x, int y);
void drawLetterS(int x, int y);
void drawLetterC(int x, int y);
void drawLetterH(int x, int y);
void drawLetterR(int x, int y);
void drawRoadBorders();
void drawRoadTexture();

// ========== ALGORITMOS DE LÍNEA RECTA ==========

// Algoritmo de Bresenham para líneas rectas
void drawLineBresenham(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    int x = x0, y = y0;

    glBegin(GL_POINTS);
    while (1)
    {
        glVertex2i(x, y);

        if (x == x1 && y == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
    glEnd();
}

// Algoritmo DDA (Digital Differential Analyzer) para líneas rectas
void drawLineDDA(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float xIncrement = (float)dx / steps;
    float yIncrement = (float)dy / steps;

    float x = x0;
    float y = y0;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++)
    {
        glVertex2i((int)(x + 0.5), (int)(y + 0.5));
        x += xIncrement;
        y += yIncrement;
    }
    glEnd();
}

// Función auxiliar para dibujar líneas gruesas usando algoritmo de línea
void drawThickLine(int x0, int y0, int x1, int y1, int thickness)
{
    // Dibujar múltiples líneas paralelas para crear grosor
    for (int i = -thickness / 2; i <= thickness / 2; i++)
    {
        if (abs(x1 - x0) > abs(y1 - y0))
        {
            // Línea más horizontal - desplazar en Y
            drawLineBresenham(x0, y0 + i, x1, y1 + i);
        }
        else
        {
            // Línea más vertical - desplazar en X
            drawLineBresenham(x0 + i, y0, x1 + i, y1);
        }
    }
}

// ========== FUNCIONES DE CARRETERA REFACTORIZADAS ==========

void drawLaneLines(float y)
{
    glColor3f(1.0, 1.0, 1.0); // Color blanco
    glPointSize(2.0f);

    // Usar algoritmo de línea para dibujar líneas discontinuas
    for (float x = 0; x < 800; x += 40)
    {
        int startX = (int)(x + offset);
        int endX = (int)(x + offset + 20);
        int lineY = (int)y;

        // Dibujar línea horizontal usando Bresenham
        drawLineBresenham(startX, lineY, endX, lineY);

        // Agregar grosor dibujando líneas adicionales arriba y abajo
        drawLineBresenham(startX, lineY + 1, endX, lineY + 1);
        drawLineBresenham(startX, lineY - 1, endX, lineY - 1);
    }
}

void drawRoadBorders()
{
    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo para bordes
    glPointSize(3.0f);

    // Borde superior de la carretera usando algoritmo de línea
    int borderY_top = 410;
    for (int x = 0; x < 800; x += 5)
    {
        int currentX = (int)(x + pointOffset);
        if (currentX >= 0 && currentX < 800)
        {
            drawLineBresenham(currentX, borderY_top, currentX + 3, borderY_top);
        }
    }

    // Borde inferior de la carretera
    int borderY_bottom = 190;
    for (int x = 0; x < 800; x += 5)
    {
        int currentX = (int)(x + pointOffset);
        if (currentX >= 0 && currentX < 800)
        {
            drawLineBresenham(currentX, borderY_bottom, currentX + 3, borderY_bottom);
        }
    }
}

void drawRoadTexture()
{
    glColor3f(0.5f, 0.5f, 0.5f); // Gris para textura
    glPointSize(1.5f);

    // Crear textura de carretera usando puntos distribuidos con algoritmo de línea
    for (int x = 10; x < 800; x += 35)
    {
        for (int y = 210; y < 390; y += 25)
        {
            // Agregar variación pseudo-aleatoria
            float offsetX = ((int)(x + y) % 7) - 3;
            float offsetY = ((int)(x * y) % 5) - 2;

            int pointX = (int)(x + pointOffset + offsetX);
            int pointY = (int)(y + offsetY);

            // Dibujar pequeñas líneas en lugar de puntos simples para mejor textura
            if (pointX >= 0 && pointX < 795)
            {
                drawLineBresenham(pointX, pointY, pointX + 2, pointY);
                drawLineBresenham(pointX, pointY, pointX, pointY + 1);
            }
        }
    }
}

void drawRoadPoints()
{
    // Dibujar bordes usando algoritmo de línea
    drawRoadBorders();

    // Dibujar textura de carretera
    drawRoadTexture();
}

void drawSign()
{
    // Cartel "LOS CHORROS" que se mueve de derecha a izquierda
    int cycleTime = frameCount % 900;

    if (cycleTime < 600)
    {
        float signX = 900 - (cycleTime * 1.5f);

        if (signX > -200 && signX < 900)
        {
            // Poste del cartel usando líneas gruesas
            glColor3f(0.4f, 0.2f, 0.1f); // Café
            int poleX = (int)(signX + 50);
            drawThickLine(poleX, 480, poleX, 530, 8);

            // Fondo del cartel - dibujar rectángulo usando líneas
            glColor3f(0.9f, 0.9f, 0.7f); // Beige claro

            // Dibujar rectángulo línea por línea
            int rectLeft = (int)signX;
            int rectRight = (int)(signX + 200);
            int rectBottom = 520;
            int rectTop = 545;

            // Rellenar rectángulo dibujando líneas horizontales
            for (int y = rectBottom; y <= rectTop; y++)
            {
                drawLineBresenham(rectLeft, y, rectRight, y);
            }

            // Borde del cartel usando líneas
            glColor3f(0.3f, 0.2f, 0.1f);                                   // Café oscuro
            drawThickLine(rectLeft, rectBottom, rectRight, rectBottom, 3); // Línea inferior
            drawThickLine(rectLeft, rectTop, rectRight, rectTop, 3);       // Línea superior
            drawThickLine(rectLeft, rectBottom, rectLeft, rectTop, 3);     // Línea izquierda
            drawThickLine(rectRight, rectBottom, rectRight, rectTop, 3);   // Línea derecha

            // Texto "LOS CHORROS" usando algoritmo de línea para formar las letras
            glColor3f(0.8f, 0.0f, 0.0f); // Rojo
            glPointSize(2.0f);

            int textX = (int)(signX + 15);
            int textY = 532;

            // Dibujar cada letra usando líneas
            drawLetterL(textX, textY);
            textX += 18;

            drawLetterO(textX, textY);
            textX += 18;

            drawLetterS(textX, textY);
            textX += 25; // Espacio entre palabras

            drawLetterC(textX, textY);
            textX += 18;

            drawLetterH(textX, textY);
            textX += 18;

            drawLetterO(textX, textY);
            textX += 18;

            drawLetterR(textX, textY);
            textX += 18;

            drawLetterR(textX, textY);
            textX += 18;

            drawLetterO(textX, textY);
            textX += 18;

            drawLetterS(textX, textY);
        }
    }
}

// ========== FUNCIONES PARA DIBUJAR LETRAS USANDO ALGORITMO DE LÍNEA ==========

void drawLetterL(int x, int y)
{
    drawThickLine(x, y - 7, x, y + 5, 2);     // Línea vertical
    drawThickLine(x, y - 7, x + 8, y - 7, 2); // Línea horizontal inferior
}

void drawLetterO(int x, int y)
{
    // Dibujar 'O' usando líneas para formar un rectángulo
    drawThickLine(x, y - 6, x, y + 4, 2);         // Izquierda
    drawThickLine(x + 7, y - 6, x + 7, y + 4, 2); // Derecha
    drawThickLine(x + 1, y + 4, x + 6, y + 4, 2); // Superior
    drawThickLine(x + 1, y - 6, x + 6, y - 6, 2); // Inferior
}

void drawLetterS(int x, int y)
{
    drawThickLine(x, y + 4, x + 7, y + 4, 2);     // Superior
    drawThickLine(x, y + 1, x, y + 4, 2);         // Izquierda superior
    drawThickLine(x, y - 1, x + 7, y - 1, 2);     // Medio
    drawThickLine(x + 7, y - 4, x + 7, y - 1, 2); // Derecha inferior
    drawThickLine(x, y - 6, x + 7, y - 6, 2);     // Inferior
}

void drawLetterC(int x, int y)
{
    drawThickLine(x, y - 6, x, y + 4, 2);     // Línea vertical izquierda
    drawThickLine(x, y + 4, x + 7, y + 4, 2); // Línea superior
    drawThickLine(x, y - 6, x + 7, y - 6, 2); // Línea inferior
}

void drawLetterH(int x, int y)
{
    drawThickLine(x, y - 7, x, y + 5, 2);         // Línea izquierda
    drawThickLine(x + 7, y - 7, x + 7, y + 5, 2); // Línea derecha
    drawThickLine(x, y - 1, x + 7, y - 1, 2);     // Línea horizontal media
}

void drawLetterR(int x, int y)
{
    drawThickLine(x, y - 7, x, y + 5, 2);         // Línea vertical izquierda
    drawThickLine(x, y + 4, x + 7, y + 4, 2);     // Línea superior
    drawThickLine(x, y - 1, x + 7, y - 1, 2);     // Línea media
    drawThickLine(x + 7, y, x + 7, y + 4, 2);     // Derecha superior
    drawThickLine(x + 3, y - 7, x + 7, y - 3, 2); // Diagonal inferior
}

// ========== FUNCIÓN PRINCIPAL DE CARRETERA ==========

void drawRoad()
{
    // Dibuja el paisaje de fondo primero
    drawMountain();
    drawForest();

    // Dibuja el fondo de la carretera usando líneas horizontales
    glColor3f(0.2f, 0.2f, 0.2f); // Gris oscuro

    // Rellenar carretera línea por línea usando algoritmo de línea
    for (int y = 200; y <= 400; y++)
    {
        drawLineBresenham(0, y, 800, y);
    }

    // Dibuja los puntos decorativos y reflectores usando algoritmos de línea
    drawRoadPoints();

    // Dibuja línea central entre los 2 carriles usando algoritmo de línea
    drawLaneLines(300);

    // Dibuja el cartel
    drawSign();

    // Actualización de animaciones
    frameCount++;

    // Movimiento de las líneas de derecha a izquierda
    offset -= 1.0f;
    if (offset <= -40.0f)
        offset = 0.0f;

    // Movimiento de los puntos (más lento que las líneas)
    pointOffset -= 0.5f;
    if (pointOffset <= -60.0f)
        pointOffset = 0.0f;

    // Movimiento del paisaje (efecto de avance)
    landscapeOffset -= 0.8f;
    if (landscapeOffset <= -200.0f)
        landscapeOffset = 0.0f;
}