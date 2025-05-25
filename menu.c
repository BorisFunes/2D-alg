#include <GL/glut.h>
#include "header.h"
#include <stdio.h>

// Función para dibujar texto simple usando puntos (bitmap text no disponible en todas las configuraciones)
// Función para mostrar texto
void drawText(float x, float y, const char *text)
{
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}
// Función para dibujar el menú principal
// Función para dibujar el menú principal
void drawMenu()
{
    // Fondo del menú
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Marco decorativo del menú
    glColor3f(0.8f, 0.8f, 0.9f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(150, 200);
    glVertex2f(650, 200);
    glVertex2f(650, 500);
    glVertex2f(150, 500);
    glEnd();
    glLineWidth(1.0f);

    // Título principal
    glColor3f(1.0f, 0.9f, 0.7f);
    drawText(220, 460, "JUEGO DE VEHICULOS 2D");

    glColor3f(0.9f, 0.9f, 1.0f);
    drawText(250, 430, "SELECCIONA TU VEHICULO");

    // Opciones del menú
    glColor3f(0.9f, 0.9f, 1.0f);
    drawText(200, 380, "1. Moto de Delivery");
    drawText(200, 350, "2. Kia Soul (Carro)");
    drawText(200, 320, "3. Microbus Coaster");
    drawText(200, 290, "4. Bus Urbano");

    // Opción de salir en color diferente
    glColor3f(1.0f, 0.7f, 0.7f);
    drawText(200, 250, "5. Salir del juego");

    // Instrucciones
    glColor3f(0.8f, 0.8f, 0.8f);
    drawText(220, 210, "Presiona 1, 2, 3, 4 o 5");

    // No necesitas glutSwapBuffers() aquí porque se maneja en display()
}

// Función para dibujar el juego completo
void drawGame()
{
    // NO limpiar aquí - ya se limpia en display()

    // Dibujar carretera (que incluye paisaje de fondo)
    drawRoad();

    // NO llamar glutSwapBuffers() aquí - se hace en display()
}

// Función para mostrar información del juego
void drawGameInfo()
{
    Vehicle *playerVehicle = getVehicle(0);
    if (playerVehicle != NULL)
    {
        // Información del vehículo del jugador
        glColor3f(1.0f, 1.0f, 1.0f);

        char info[100];
        sprintf(info, "Velocidad: %.1f", playerVehicle->speed);
        drawText(10, 580, info);

        sprintf(info, "Carril: %d", playerVehicle->currentLane + 1);
        drawText(10, 560, info);

        sprintf(info, "Pos: (%.0f, %.0f)", playerVehicle->x, playerVehicle->y);
        drawText(10, 540, info);

        // Controles
        glColor3f(0.8f, 0.8f, 0.8f);
        drawText(10, 50, "WASD o Flechas: Mover");
        drawText(10, 30, "M: Menu  R: Reiniciar");
        drawText(10, 10, "P: Pausa");
        drawText(90, 10, "ESPACIO: Saltar");
    }
}