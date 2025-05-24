#include <GL/glut.h>
#include <math.h>
#include "header.h"

// Definición de variables globales
float offset = 0.0f;
float pointOffset = 0.0f;
float landscapeOffset = 0.0f;
int frameCount = 0;

void drawLaneLines(float y)
{
    for (float x = 0; x < 800; x += 40)
    {
        glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 1.0); // Color blanco
        glVertex2f(x + offset, y);
        glVertex2f(x + offset + 20, y); // Línea horizontal de 20px
        glEnd();
    }
}

void drawRoadPoints()
{
    // Configurar tamaño de los puntos
    glPointSize(3.0f);

    glBegin(GL_POINTS);
    glColor3f(0.8f, 0.8f, 0.0f); // Color amarillo para simular reflectores

    // Puntos en el borde superior de la carretera
    for (float x = 0; x < 800; x += 60)
    {
        glVertex2f(x + pointOffset, 410); // Borde superior
    }

    // Puntos en el borde inferior de la carretera
    for (float x = 0; x < 800; x += 60)
    {
        glVertex2f(x + pointOffset, 190); // Borde inferior
    }

    // Puntos decorativos en los carriles (simulando grava o textura)
    glColor3f(0.5f, 0.5f, 0.5f); // Gris claro
    glPointSize(2.0f);

    for (float x = 10; x < 800; x += 35)
    {
        for (float y = 210; y < 390; y += 25)
        {
            // Agregar algo de variación aleatoria simulada
            float offsetX = ((int)(x + y) % 7) - 3; // Pseudo-random -3 a 3
            float offsetY = ((int)(x * y) % 5) - 2; // Pseudo-random -2 a 2
            glVertex2f(x + pointOffset + offsetX, y + offsetY);
        }
    }

    glEnd();
}

void drawSign()
{
    // Cartel "LOS CHORROS" que se mueve de derecha a izquierda
    int cycleTime = frameCount % 900; // Ciclo más largo para movimiento completo

    if (cycleTime < 600) // El cartel está visible durante más tiempo
    {
        // Posición del cartel (se mueve de derecha a izquierda)
        float signX = 900 - (cycleTime * 1.5f); // Velocidad de movimiento

        // Solo dibujar si está en pantalla
        if (signX > -200 && signX < 900)
        {
            // Poste del cartel
            glLineWidth(8.0f);
            glBegin(GL_LINES);
            glColor3f(0.4f, 0.2f, 0.1f); // Café
            glVertex2f(signX + 50, 480);
            glVertex2f(signX + 50, 530);
            glEnd();

            // Fondo del cartel (más grande para letras separadas)
            glBegin(GL_QUADS);
            glColor3f(0.9f, 0.9f, 0.7f); // Beige claro
            glVertex2f(signX, 520);
            glVertex2f(signX + 200, 520);
            glVertex2f(signX + 200, 545);
            glVertex2f(signX, 545);
            glEnd();

            // Borde del cartel
            glLineWidth(3.0f);
            glBegin(GL_LINE_LOOP);
            glColor3f(0.3f, 0.2f, 0.1f); // Café oscuro
            glVertex2f(signX, 520);
            glVertex2f(signX + 200, 520);
            glVertex2f(signX + 200, 545);
            glVertex2f(signX, 545);
            glEnd();

            // Texto "LOS CHORROS" usando puntos pequeños con mejor separación
            glPointSize(2.5f);
            glBegin(GL_POINTS);
            glColor3f(0.8f, 0.0f, 0.0f); // Rojo

            float textX = signX + 15; // Posición inicial del texto

            // L
            for (int i = 0; i < 12; i++)
                glVertex2f(textX, 525 + i);
            for (int i = 0; i < 10; i++)
                glVertex2f(textX + i, 525);
            textX += 18; // Espacio entre letras

            // O
            for (int i = 0; i < 10; i++)
            {
                glVertex2f(textX, 526 + i);
                glVertex2f(textX + 8, 526 + i);
            }
            for (int i = 0; i < 8; i++)
            {
                glVertex2f(textX + 1 + i, 526);
                glVertex2f(textX + 1 + i, 535);
            }
            textX += 18;

            // S
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 535);
            for (int i = 0; i < 5; i++)
                glVertex2f(textX, 531 + i);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 530);
            for (int i = 0; i < 5; i++)
                glVertex2f(textX + 7, 526 + i);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 526);
            textX += 25; // Espacio más grande entre palabras

            // C
            for (int i = 0; i < 10; i++)
                glVertex2f(textX, 526 + i);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 526);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 535);
            textX += 18;

            // H
            for (int i = 0; i < 12; i++)
                glVertex2f(textX, 525 + i);
            for (int i = 0; i < 12; i++)
                glVertex2f(textX + 8, 525 + i);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 530);
            textX += 18;

            // O
            for (int i = 0; i < 10; i++)
            {
                glVertex2f(textX, 526 + i);
                glVertex2f(textX + 8, 526 + i);
            }
            for (int i = 0; i < 8; i++)
            {
                glVertex2f(textX + 1 + i, 526);
                glVertex2f(textX + 1 + i, 535);
            }
            textX += 18;

            // R
            for (int i = 0; i < 12; i++)
                glVertex2f(textX, 525 + i);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 535);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 530);
            for (int i = 0; i < 5; i++)
                glVertex2f(textX + 7, 531 + i);
            for (int i = 0; i < 5; i++)
                glVertex2f(textX + 3 + i, 526 + i);
            textX += 18;

            // R
            for (int i = 0; i < 12; i++)
                glVertex2f(textX, 525 + i);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 535);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 530);
            for (int i = 0; i < 5; i++)
                glVertex2f(textX + 7, 531 + i);
            for (int i = 0; i < 5; i++)
                glVertex2f(textX + 3 + i, 526 + i);
            textX += 18;

            // O
            for (int i = 0; i < 10; i++)
            {
                glVertex2f(textX, 526 + i);
                glVertex2f(textX + 8, 526 + i);
            }
            for (int i = 0; i < 8; i++)
            {
                glVertex2f(textX + 1 + i, 526);
                glVertex2f(textX + 1 + i, 535);
            }
            textX += 18;

            // S
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 535);
            for (int i = 0; i < 5; i++)
                glVertex2f(textX, 531 + i);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 530);
            for (int i = 0; i < 5; i++)
                glVertex2f(textX + 7, 526 + i);
            for (int i = 0; i < 8; i++)
                glVertex2f(textX + i, 526);

            glEnd();
        }
    }
}

void drawRoad()
{
    // Dibuja el paisaje de fondo primero
    drawMountain();
    drawForest();

    // Dibuja el fondo de la carretera (ajustado para 2 carriles)
    glColor3f(0.2f, 0.2f, 0.2f); // Gris oscuro
    glBegin(GL_QUADS);
    glVertex2f(0, 200);   // Inicio Y ajustado
    glVertex2f(800, 200); // Ancho completo
    glVertex2f(800, 400); // Final Y ajustado
    glVertex2f(0, 400);   // Cierre del rectángulo
    glEnd();

    // Dibuja los puntos decorativos y reflectores
    drawRoadPoints();

    // Dibuja línea central entre los 2 carriles
    drawLaneLines(300); // Línea central entre carril 1 y 2

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