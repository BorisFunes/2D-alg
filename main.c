#include <GL/glut.h>
#include "header.h"

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawRoad(); // Llama la función que dibuja las calles y paisaje
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h); // Vista ortogonal 2D
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
    glutCreateWindow("Juego 2D - Los Chorros");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glClearColor(1.0, 1.0, 1.0, 1.0); // Fondo blanco
    glutMainLoop();
    return 0;
}