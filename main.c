#include <GL/glut.h>
#include "header.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Variables globales
int selectedVehicle = 0;
bool enMenu = true;
bool gameInitialized = false;
bool gamePaused = false; // Nueva variable para controlar la pausa
bool gameOver = false;

// Variables para animación y efectos
float offset = 0.0f;
float pointOffset = 0.0f;
float landscapeOffset = 0.0f;
int frameCount = 0;

// Función para inicializar el juego con vehículos
void initGame()
{
    if (!gameInitialized)
    {
        // Inicializar generador de números aleatorios
        srand(time(NULL));
        
        // Limpiar vehículos existentes
        clearVehicles();
        clearObstacles(); // Asegurarse de limpiar obstáculos previos

        // Inicializar SOLO el vehículo seleccionado por el jugador
        switch (selectedVehicle)
        {
        case 1:                         // Delivery
            initVehicle(0, 50, 250, 0); // Vehículo Delivery en la izquierda
            break;
        case 2:                         // Car
            initVehicle(0, 50, 250, 1); // Carro en la izquierda
            break;
        case 3:                         // Coaster
            initVehicle(0, 50, 300, 2); // Coaster en la izquierda
            break;
        case 4:                         // Bus
            initVehicle(0, 50, 300, 3); // Bus en la izquierda
            break;
        default:
            // Si no hay selección válida, crear un vehículo delivery por defecto en la izquierda
            initVehicle(0, 50, 250, 0);
            selectedVehicle = 1;
            break;
        }

        // Inicializar sistema de obstáculos
        initObstacles();
        
        gameInitialized = true;
        printf("Juego inicializado con vehiculo tipo %d en posicion izquierda (x=50)\n", selectedVehicle);
    }
}

void drawGameOverMessage()
{
    // Dibujar un rectángulo semi-transparente de fondo
    glEnable(GL_BLEND);
    glColor4f(0.3f, 0.0f, 0.0f, 0.7f); // Rojo oscuro semi-transparente
    drawRect(0, 0, 800, 600);

    // Dibujar el texto de Game Over
    glColor3f(1.0f, 1.0f, 1.0f); // Blanco para el texto

    // Posicionar el texto en el centro de la pantalla
    glRasterPos2f(350, 320);
    char gameOverText[] = "GAME OVER";
    for (int i = 0; gameOverText[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gameOverText[i]);
    }

    glRasterPos2f(300, 280);
    char instructionText[] = "Presiona 'R' para reiniciar";
    for (int i = 0; instructionText[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, instructionText[i]);
    }

    glDisable(GL_BLEND);
}

// Función principal de display
void display()
{
    // Limpiar buffer primero
    glClear(GL_COLOR_BUFFER_BIT);

    // Configurar matriz de proyección
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (enMenu)
    {
        drawMenu();
    }
    else
    {
        if (!gameInitialized)
        {
            initGame();
        }

        drawGame();

        // Solo actualizar si no está pausado ni en game over
        if (!gamePaused && !gameOver)
        {
            updateVehicles();
            updateObstacles();
            
            Vehicle* playerVehicle = getVehicle(0);
            if (playerVehicle != NULL)
            {
                float vehicleWidth, vehicleHeight;
                
                switch (playerVehicle->vehicleType)
                {
                    case 0: vehicleWidth = 44; vehicleHeight = 32; break;
                    case 1: vehicleWidth = 70; vehicleHeight = 50; break;
                    case 2: vehicleWidth = 100; vehicleHeight = 85; break;
                    case 3: vehicleWidth = 140; vehicleHeight = 70; break;
                    default: vehicleWidth = 50; vehicleHeight = 40;
                }
                
                if (checkObstacleCollision(playerVehicle->x, playerVehicle->y, 
                                        vehicleWidth, vehicleHeight))
                {
                    printf("¡GAME OVER! Colisión detectada\n");
                    gameOver = true;
                    gamePaused = true;
                }
            }
        }

        renderVehicles();
        renderObstacles();
        drawGameInfo();

        if (gamePaused)
        {
            if (gameOver)
            {
                drawGameOverMessage();
            }
            else
            {
                drawPauseMessage();
            }
        }
    }

    glutSwapBuffers();
}
// Función de redimensionamiento
void reshape(int w, int h)
{
    printf("Ventana redimensionada a: %dx%d\n", w, h);

    // Evitar división por cero
    if (h == 0)
        h = 1;

    // Configurar viewport
    glViewport(0, 0, w, h);

    // Configurar matrices de proyección
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Mantener proporción de aspecto pero usar coordenadas fijas
    float aspect = (float)w / (float)h;
    if (aspect >= (800.0f / 600.0f))
    {
        // Ventana más ancha - ajustar horizontalmente
        float newWidth = 600.0f * aspect;
        gluOrtho2D(-(newWidth - 800.0f) / 2.0f, 800.0f + (newWidth - 800.0f) / 2.0f, 0, 600);
    }
    else
    {
        // Ventana más alta - ajustar verticalmente
        float newHeight = 800.0f / aspect;
        gluOrtho2D(0, 800, -(newHeight - 600.0f) / 2.0f, 600.0f + (newHeight - 600.0f) / 2.0f);
    }

    glMatrixMode(GL_MODELVIEW);

    // Forzar redibujado
    glutPostRedisplay();
}

// Manejador de teclas normales - CORREGIDO
void keyHandler(unsigned char key, int x, int y)
{
    if (enMenu)
    {
        if (key >= '1' && key <= '4')
        {
            // Seleccionar vehículo
            selectedVehicle = key - '0';
            enMenu = false;
            gameInitialized = false; // Reinicializar juego con nuevo vehículo
            printf("Vehiculo seleccionado: %d\n", selectedVehicle);
        }
        else if (key == '5')
        {
            // Salir del juego
            printf("Saliendo del juego...\n");
            exit(0);
        }
    }
    else
    {
        // En el juego
        if (key == 'm' || key == 'M')
        {
            // Volver al menú
            enMenu = true;
            selectedVehicle = 0;
            gameInitialized = false;
            clearVehicles();
            printf("Volviendo al menu...\n");
        }
        else if (key == 'r' || key == 'R')
        {
            // Reiniciar posiciones de vehículos
            gamePaused = false; // Asegurar que no esté pausado al reiniciar
            gameInitialized = false;
            gameOver = false;
            printf("Reiniciando juego...\n");
        }
        else if (key == 'p' || key == 'P' && !gameOver)
        {
            // Pausar/reanudar juego
            gamePaused = !gamePaused;
            if (gamePaused)
            {
                printf("Juego PAUSADO\n");
            }
            else
            {
                printf("Juego REANUDADO\n");
            }
        }
        else if (key == ' ') // Control de salto con barra espaciadora
        {
            // Solo permitir saltar si no está pausado
            if (!gamePaused)
            {
                // Saltar con el vehículo principal (índice 0)
                if (getVehicleCount() > 0)
                {
                    jumpVehicle(0);
                    printf("¡Salto activado para vehiculo 0!\n");
                }
                else
                {
                    printf("No hay vehiculos para saltar\n");
                }
            }
        }
        else
        {
            // Solo permitir controlar el vehículo si no está pausado
            if (!gamePaused && !gameOver)
            {
                // Controlar el vehículo principal (índice 0)
                controlVehicle(0, key);
            }
        }
    }
    glutPostRedisplay();
}

// Función para manejar teclas especiales (flechas)
void specialKeyHandler(int key, int x, int y)
{
    // Solo permitir movimiento si no está en menú, hay vehículos y no está pausado
    if (!enMenu && getVehicleCount() > 0 && !gamePaused)
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            moveVehicleUp(0); // Cambiar al carril superior
            break;
        case GLUT_KEY_DOWN:
            moveVehicleDown(0); // Cambiar al carril inferior
            break;
        case GLUT_KEY_LEFT:
            moveVehicleLeft(0); // Mover hacia la izquierda
            break;
        case GLUT_KEY_RIGHT:
            moveVehicleRight(0); // Mover hacia la derecha
            break;
        }
    }
    glutPostRedisplay();
}

// Función idle para redibujado continuo
void idle()
{
    if (!enMenu)
    {
        glutPostRedisplay(); // Redibuja continuamente solo en el juego
    }
}

// Función para manejar el timer (controlar FPS)
void timer(int value)
{
    // Solo redibujar si no está en menú (el juego siempre debe redibujarse, pausado o no)
    if (!enMenu)
    {
        glutPostRedisplay();
    }
    glutTimerFunc(16, timer, 0); // ~60 FPS (16ms)
}

// Función para dibujar el mensaje de pausa
void drawPauseMessage()
{
    // Dibujar un rectángulo semi-transparente de fondo
    glEnable(GL_BLEND);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Negro semi-transparente
    drawRect(0, 0, 800, 600);

    // Dibujar el texto de pausa (necesitarás implementar drawText o usar GLUT)
    glColor3f(1.0f, 1.0f, 1.0f); // Blanco para el texto

    // Posicionar el texto en el centro de la pantalla
    glRasterPos2f(350, 320);

    // Usar GLUT para renderizar texto
    char pauseText[] = "JUEGO PAUSADO";
    for (int i = 0; pauseText[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, pauseText[i]);
    }

    glRasterPos2f(300, 280);
    char instructionText[] = "Presiona 'P' para continuar";
    for (int i = 0; instructionText[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, instructionText[i]);
    }

    glDisable(GL_BLEND);
}

// Función principal
int main(int argc, char **argv)
{
    printf("Iniciando juego de vehiculos 2D...\n");

    // Inicializar GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Juego 2D");

    // Configurar OpenGL
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Fondo azul cielo
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Deshabilitar depth test para 2D
    glDisable(GL_DEPTH_TEST);

    // Configurar callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyHandler);
    glutSpecialFunc(specialKeyHandler); // Para teclas de flecha
    glutIdleFunc(idle);
    glutTimerFunc(16, timer, 0); // Timer para controlar FPS

    printf("Configuracion completada. Entrando al bucle principal...\n");
    printf("Controles en el menu: 1-4 para seleccionar vehiculo, 5 para salir\n");
    printf("Controles en el juego: WASD o flechas para mover, ESPACIO para saltar, P para pausar, M para menu, R para reiniciar\n");

    // Iniciar el bucle principal
    glutMainLoop();
    return 0;
}