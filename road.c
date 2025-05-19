#include <GL/glut.h>

static float offset = 0.0f;

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

void drawRoad()
{
    // Dibuja el fondo de la carretera
    glColor3f(0.2f, 0.2f, 0.2f); // Gris oscuro
    glBegin(GL_QUADS);
    glVertex2f(0, 150);
    glVertex2f(800, 150);
    glVertex2f(800, 450);
    glVertex2f(0, 450);
    glEnd();

    // Dibuja las líneas horizontales entrecortadas para los carriles
    drawLaneLines(250); // Entre carril 1 y 2
    drawLaneLines(350); // Entre carril 2 y 3

    // Movimiento de las líneas de derecha a izquierda
    offset -= 1.0f;
    if (offset <= -40.0f)
        offset = 0.0f;
}
