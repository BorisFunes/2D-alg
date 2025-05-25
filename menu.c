#include <GL/glut.h>
#include "header.h"

// funcion para mostrar texto
void drawText(float x, float y, const char *text)
{
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

// menu
void drawMenu()
{
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // marco
    glColor3f(0.8f, 0.8f, 0.9f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(180, 250);
    glVertex2f(620, 250);
    glVertex2f(620, 480);
    glVertex2f(180, 480);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(1.0f, 0.9f, 0.7f);
    drawText(240, 450, "SELECCIONA TU VEHICULO");

    glColor3f(0.9f, 0.9f, 1.0f);
    drawText(200, 410, "1. Moto de Delivery");
    drawText(200, 380, "2. Kia Soul");
    drawText(200, 350, "3. Microbus Coaster");
    drawText(200, 320, "4. Bus");
    glColor3f(1.0f, 0.7f, 0.7f);
    drawText(200, 290, "5. Salir del juego");
    glColor3f(0.8f, 0.8f, 0.8f);
    drawText(220, 260, "Presiona 1, 2, 3, 4 o 5");

    glutSwapBuffers();
}

void drawGame()
{

    glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    drawRoad(); // Llama la función que dibuja las calles
    // dibuja segun la seleccion
    switch (selectedVehicle)
    {
    case 1:
        drawDelivery(680, 250);
        break;
    case 2:
        drawCar(680, 250);
        break;
    case 3:
        drawCoaster(680, 250);
        break;
    case 4:
        drawBus(600, 250);
        break;
    }

    // muestra el vehiculo seleccionado
    glColor3f(1.0f, 1.0f, 1.0f);
    switch (selectedVehicle)
    {
    case 1:
        drawText(20, 570, "Vehiculo: Moto de Delivery");
        break;
    case 2:
        drawText(20, 570, "Vehiculo: Kia Soul");
        break;
    case 3:
        drawText(20, 570, "Vehiculo: Microbus Coaster");
        break;
    case 4:
        drawText(20, 570, "Vehiculo: Bus");
        break;
    }
    glColor3f(0.2f, 0.2f, 0.2f);
    drawText(20, 5, "Presiona 'M' para volver al menu");

    glutSwapBuffers();
}
