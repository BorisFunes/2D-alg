#include <GL/glut.h>
#include "header.h"
#include <stdbool.h>

int selectedVehicle = 0;
bool enMenu = true;

// func principal
void display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (enMenu)
    {
        drawMenu();
    }
    else
    {
        drawGame();
    }
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h); // Vista ortogonal 2D
}

void keyHandler(unsigned char key, int x, int y)
{
    if (enMenu)
    {
        if (key >= '1' && key <= '4') // seleccionar vehículo
        {
            selectedVehicle = key - '0';
            enMenu = false;
        }

        else if (key == '5') // salir del juego
        {
            exit(0);
        }
    }
    else
    {
        // Volver al menú con 'M'
        if (key == 'm' || key == 'M')
        {
            enMenu = true;
            selectedVehicle = 0;
        }
    }
    glutPostRedisplay();
}

void idle()
{
    glutPostRedisplay(); // Redibuja continuamente
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Juego 2D - Vehiculos");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyHandler);
    glutIdleFunc(idle);

    glClearColor(1.0, 1.0, 1.0, 1.0); // Fondo blanco

    glutMainLoop();
    return 0;
}