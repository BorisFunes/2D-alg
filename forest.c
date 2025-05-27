#include <GL/glut.h>
#include <math.h>
#include "header.h"

void drawFilledCircle(float cx, float cy, float radius, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Centro del círculo

    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = cx + radius * cos(angle);
        float y = cy + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawFilledEllipse(float cx, float cy, float radiusX, float radiusY, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Centro de la elipse

    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = cx + radiusX * cos(angle);
        float y = cy + radiusY * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    glBegin(GL_TRIANGLES);
    // Primer triángulo
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);

    // Segundo triángulo
    glVertex2f(x1, y1);
    glVertex2f(x3, y3);
    glVertex2f(x4, y4);
    glEnd();
}

// ========== FUNCIONES DE PAISAJE REFACTORIZADAS ==========

void drawForest()
{
    // ========== TRONCOS DE ÁRBOLES ==========
    // Convertir líneas gruesas a rectángulos usando triángulos
    glColor3f(0.4f, 0.2f, 0.1f); // Café oscuro para troncos

    for (float x = 50; x < 1000; x += FOREST_DENSITY)
    {
        float trunkX = x + landscapeOffset;
        float trunkWidth = 8.0f; // Equivalente al grosor de línea original

        // Dibujar tronco como rectángulo usando dos triángulos
        glBegin(GL_TRIANGLES);
        // Primer triángulo del tronco
        glVertex2f(trunkX - trunkWidth / 2, 50);
        glVertex2f(trunkX + trunkWidth / 2, 50);
        glVertex2f(trunkX + trunkWidth / 2, 120);

        // Segundo triángulo del tronco
        glVertex2f(trunkX - trunkWidth / 2, 50);
        glVertex2f(trunkX + trunkWidth / 2, 120);
        glVertex2f(trunkX - trunkWidth / 2, 120);
        glEnd();
    }

    // ========== COPAS DE ÁRBOLES ==========
    // Convertir puntos grandes a círculos pequeños usando triangle fans
    glColor3f(0.1f, 0.6f, 0.1f); // Verde oscuro

    for (float x = 50; x < 1000; x += FOREST_DENSITY)
    {
        float treeX = x + landscapeOffset;

        // Copa principal - usar múltiples círculos para simular los puntos originales
        for (int i = -20; i <= 20; i += TREE_DETAIL_LEVEL)
        {
            for (int j = 0; j <= 30; j += TREE_DETAIL_LEVEL)
            {
                if (abs(i) + j < 35) // Mantener forma circular aproximada
                {
                    float leafX = treeX + i;
                    float leafY = 120 + j;
                    drawFilledCircle(leafX, leafY, 6.0f, 8); // Radio equivalente al punto size 12
                }
            }
        }
    }

    // ========== ARBUSTOS ==========
    // Convertir puntos medianos a círculos más pequeños
    glColor3f(0.2f, 0.7f, 0.2f); // Verde claro

    for (float x = 20; x < 1000; x += 45)
    {
        float bushX = x + landscapeOffset;

        for (int i = -10; i <= 10; i += 6)
        {
            float leafX = bushX + i;
            float leafY = 35 + (abs(i) % 10);
            drawFilledCircle(leafX, leafY, 3.0f, 6); // Radio equivalente al punto size 6
        }
    }

    // ========== FLORES ==========
    // Convertir puntos pequeños de colores a círculos diminutos
    for (float x = 30; x < 1000; x += FLOWER_SPACING)
    {
        float flowerBaseX = x + landscapeOffset;

        // Flores rojas - usar elipses pequeñas para más variedad visual
        glColor3f(1.0f, 0.2f, 0.2f);
        drawFilledEllipse(flowerBaseX, 25, 2.5f, 2.0f, 6);

        // Flores amarillas
        glColor3f(1.0f, 1.0f, 0.0f);
        drawFilledCircle(flowerBaseX + 20, 30, 2.0f, 6);

        // Flores azules
        glColor3f(0.2f, 0.2f, 1.0f);
        drawFilledCircle(flowerBaseX + 40, 28, 2.0f, 6);

        // Agregar centros de flores para más detalle
        glColor3f(1.0f, 1.0f, 1.0f); // Centros blancos
        drawFilledCircle(flowerBaseX, 25, 0.8f, 4);
        drawFilledCircle(flowerBaseX + 20, 30, 0.8f, 4);
        drawFilledCircle(flowerBaseX + 40, 28, 0.8f, 4);
    }
}

void drawMountain()
{
    // ========== CAPA TRASERA DE MONTAÑA ==========
    glColor3f(0.3f, 0.5f, 0.2f); // Verde oscuro café

    // Crear silueta de montaña usando triangle strips para mejor rendimiento
    glBegin(GL_TRIANGLE_STRIP);

    for (float x = 0; x < 1200; x += MOUNTAIN_DETAIL_LEVEL)
    {
        float adjustedX = x + landscapeOffset;

        // Calcular altura usando función seno para naturalidad (igual que original)
        float height1 = 500 + 40 * sin(adjustedX * 0.02f) +
                        20 * sin(adjustedX * 0.05f) +
                        15 * cos(adjustedX * 0.08f);

        // Crear triángulos desde la base hasta la silueta
        glVertex2f(adjustedX, height1); // Punto de la silueta
        glVertex2f(adjustedX, 600);     // Punto superior (fuera de pantalla)
    }
    glEnd();

    // ========== CAPA MEDIA DE MONTAÑA ==========
    glColor3f(0.4f, 0.6f, 0.3f); // Verde medio

    glBegin(GL_TRIANGLE_STRIP);
    for (float x = 0; x < 1200; x += MOUNTAIN_DETAIL_LEVEL)
    {
        float adjustedX = x + landscapeOffset;

        float height2 = 480 + 35 * sin((adjustedX + 50) * 0.025f) +
                        25 * cos((adjustedX + 30) * 0.04f) +
                        10 * sin(adjustedX * 0.1f);

        glVertex2f(adjustedX, height2);
        glVertex2f(adjustedX, 580);
    }
    glEnd();

    // ========== CAPA FRONTAL DE MONTAÑA ==========
    glColor3f(0.5f, 0.7f, 0.4f); // Verde claro

    glBegin(GL_TRIANGLE_STRIP);
    for (float x = 0; x < 1200; x += MOUNTAIN_DETAIL_LEVEL)
    {
        float adjustedX = x + landscapeOffset;

        float height3 = 460 + 30 * sin((adjustedX + 100) * 0.03f) +
                        20 * sin((adjustedX + 80) * 0.06f) +
                        12 * cos((adjustedX + 20) * 0.09f);

        glVertex2f(adjustedX, height3);
        glVertex2f(adjustedX, 560);
    }
    glEnd();

    // ========== DETALLES DE VEGETACIÓN EN MONTAÑAS ==========
    glColor3f(0.2f, 0.4f, 0.1f); // Verde muy oscuro para sombras

    for (float x = 20; x < 1200; x += 25)
    {
        float vegX = x + landscapeOffset;
        float baseHeight = 470 + 25 * sin(vegX * 0.03f);

        // Crear pequeños grupos de vegetación usando triángulos
        for (int i = 0; i < 8; i++)
        {
            float leafX = vegX + (i % 3) * 3;
            float leafY = baseHeight + i * 2;

            // Dibujar pequeño triángulo para simular vegetación
            glBegin(GL_TRIANGLES);
            glVertex2f(leafX - 1.5f, leafY);
            glVertex2f(leafX + 1.5f, leafY);
            glVertex2f(leafX, leafY + 3);
            glEnd();
        }
    }

    // ========== ROCAS CAYENDO ==========
    glColor3f(0.4f, 0.3f, 0.2f); // Café para rocas

    for (float x = 20; x < 1000; x += 45)
    {
        float rockX = x + landscapeOffset;
        float rockY = 450 + ((int)(x + frameCount) % 50); // Rocas cayendo (animación)

        // Dibujar rocas como pequeños polígonos irregulares
        glBegin(GL_TRIANGLES);

        // Roca principal - triángulo irregular
        glVertex2f(rockX - 2, rockY - 2);
        glVertex2f(rockX + 3, rockY - 1);
        glVertex2f(rockX + 1, rockY + 3);

        // Fragmentos adicionales
        glVertex2f(rockX + 6, rockY - 4);
        glVertex2f(rockX + 10, rockY - 2);
        glVertex2f(rockX + 8, rockY + 1);

        glVertex2f(rockX + 2, rockY + 1);
        glVertex2f(rockX + 6, rockY + 3);
        glVertex2f(rockX + 4, rockY + 4);

        glEnd();
    }
}