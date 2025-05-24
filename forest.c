#include <GL/glut.h>
#include <math.h>
#include "header.h"

void drawForest()
{
    // Bosque en la parte inferior (estilo Stardew Valley)

    // Troncos de árboles usando líneas
    glLineWidth(8.0f);
    for (float x = 50; x < 1000; x += 80)
    {
        glBegin(GL_LINES);
        glColor3f(0.4f, 0.2f, 0.1f); // Café oscuro para troncos
        glVertex2f(x + landscapeOffset, 50);
        glVertex2f(x + landscapeOffset, 120);
        glEnd();
    }

    // Copa de árboles usando puntos grandes
    glPointSize(12.0f);
    glBegin(GL_POINTS);
    glColor3f(0.1f, 0.6f, 0.1f); // Verde oscuro

    for (float x = 50; x < 1000; x += 80)
    {
        // Copa principal
        for (int i = -20; i <= 20; i += 8)
        {
            for (int j = 0; j <= 30; j += 8)
            {
                if (abs(i) + j < 35) // Forma circular aproximada
                {
                    glVertex2f(x + landscapeOffset + i, 120 + j);
                }
            }
        }
    }
    glEnd();

    // Arbustos usando puntos medianos
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    glColor3f(0.2f, 0.7f, 0.2f); // Verde claro

    for (float x = 20; x < 1000; x += 45)
    {
        for (int i = -10; i <= 10; i += 6)
        {
            glVertex2f(x + landscapeOffset + i, 35 + (abs(i) % 10));
        }
    }
    glEnd();

    // Flores usando puntos pequeños de colores
    glPointSize(4.0f);
    glBegin(GL_POINTS);

    for (float x = 30; x < 1000; x += 120)
    {
        // Flores rojas
        glColor3f(1.0f, 0.2f, 0.2f);
        glVertex2f(x + landscapeOffset, 25);

        // Flores amarillas
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(x + landscapeOffset + 20, 30);

        // Flores azules
        glColor3f(0.2f, 0.2f, 1.0f);
        glVertex2f(x + landscapeOffset + 40, 28);
    }
    glEnd();
}

void drawMountain()
{
    // Montaña continua usando puntos para crear silueta natural como en la imagen

    // Creamos múltiples capas de montañas con diferentes tonos de café/verde

    // Capa trasera (más oscura)
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glColor3f(0.3f, 0.5f, 0.2f); // Verde oscuro café

    for (float x = 0; x < 1200; x += 4)
    {
        // Crear silueta ondulada usando función seno para naturalidad
        float height1 = 500 + 40 * sin((x + landscapeOffset) * 0.02f) +
                        20 * sin((x + landscapeOffset) * 0.05f) +
                        15 * cos((x + landscapeOffset) * 0.08f);

        // Rellenar desde la altura hasta arriba
        for (float y = height1; y < 600; y += 6)
        {
            glVertex2f(x + landscapeOffset, y);
        }
    }
    glEnd();

    // Capa media
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    glColor3f(0.4f, 0.6f, 0.3f); // Verde medio

    for (float x = 0; x < 1200; x += 4)
    {
        float height2 = 480 + 35 * sin((x + landscapeOffset + 50) * 0.025f) +
                        25 * cos((x + landscapeOffset + 30) * 0.04f) +
                        10 * sin((x + landscapeOffset) * 0.1f);

        for (float y = height2; y < 580; y += 5)
        {
            glVertex2f(x + landscapeOffset, y);
        }
    }
    glEnd();

    // Capa frontal (más clara)
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glColor3f(0.5f, 0.7f, 0.4f); // Verde claro

    for (float x = 0; x < 1200; x += 3)
    {
        float height3 = 460 + 30 * sin((x + landscapeOffset + 100) * 0.03f) +
                        20 * sin((x + landscapeOffset + 80) * 0.06f) +
                        12 * cos((x + landscapeOffset + 20) * 0.09f);

        for (float y = height3; y < 560; y += 4)
        {
            glVertex2f(x + landscapeOffset, y);
        }
    }
    glEnd();

    // Detalles de vegetación en las montañas
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glColor3f(0.2f, 0.4f, 0.1f); // Verde muy oscuro para sombras

    for (float x = 20; x < 1200; x += 25)
    {
        float baseHeight = 470 + 25 * sin((x + landscapeOffset) * 0.03f);
        for (int i = 0; i < 8; i++)
        {
            glVertex2f(x + landscapeOffset + (i % 3) * 3, baseHeight + i * 2);
        }
    }
    glEnd();

    // Rocas cayendo usando puntos
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glColor3f(0.4f, 0.3f, 0.2f); // Café para rocas

    for (float x = 20; x < 1000; x += 45)
    {
        float rockY = 450 + ((int)(x + frameCount) % 50); // Rocas cayendo
        glVertex2f(x + landscapeOffset, rockY);
        glVertex2f(x + landscapeOffset + 8, rockY - 3);
        glVertex2f(x + landscapeOffset + 4, rockY + 2);
    }
    glEnd();
}