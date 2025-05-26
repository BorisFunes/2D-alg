#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

// Array de obstáculos
static Obstacle obstacles[MAX_OBSTACLES];
static int obstacleCount = 0;
static int lastSpawnX = 0; // Para controlar el espaciado
static int spawnTimer = 0; // Timer para controlar la frecuencia de spawn

// Inicializar sistema de obstáculos
void initObstacles()
{
    obstacleCount = 0;
    lastSpawnX = 0;
    spawnTimer = 0;
    
    // Limpiar array de obstáculos
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        obstacles[i].active = false;
        obstacles[i].x = 0;
        obstacles[i].y = 0;
        obstacles[i].size = 0;
        obstacles[i].type = 0;
        obstacles[i].lane = 0;
    }
    
    printf("Sistema de obstáculos inicializado\n");
}

// Crear un nuevo obstáculo
void spawnObstacle(float x, int lane, int type)
{
    if (obstacleCount >= MAX_OBSTACLES) return;
    
    // Buscar slot libre
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (!obstacles[i].active)
        {
            obstacles[i].active = true;
            obstacles[i].x = x;
            obstacles[i].lane = lane;
            obstacles[i].type = type;
            obstacles[i].size = 15 + (rand() % 20); // Tamaño variable entre 15-35
            
            // Determinar Y basado en el carril (corregido)
            if (lane == 0) // Carril inferior
            {
                obstacles[i].y = 250; // Centro del carril inferior
            }
            else // Carril superior
            {
                obstacles[i].y = 350; // Centro del carril superior
            }
            
            obstacleCount++;
            printf("Obstáculo creado en X: %.0f, Carril: %d, Tipo: %d, Y: %.0f\n", 
                   obstacles[i].x, obstacles[i].lane, obstacles[i].type, obstacles[i].y);
            break;
        }
    }
}

void updateObstacles()
{
    // Mover obstáculos existentes
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (obstacles[i].active)
        {
            obstacles[i].x -= OBSTACLE_SPEED;
            
            // Eliminar obstáculos que salieron de pantalla
            if (obstacles[i].x < -50)
            {
                obstacles[i].active = false;
                obstacleCount--;
                printf("Obstáculo eliminado (fuera de pantalla)\n");
            }
        }
    }
    
    // Generar nuevos obstáculos
    spawnTimer++;
    
    if (spawnTimer >= MIN_SPAWN_INTERVAL)
    {
        if ((rand() % 100) < OBSTACLE_SPAWN_CHANCE)
        {
            // Elegir carril aleatorio (0 o 1)
            int lane = rand() % 2;
            
            // Elegir tipo de obstáculo aleatorio (0, 1 o 2)
            int type = rand() % 3;
            
            // Spawnear en el lado derecho de la pantalla
            spawnObstacle(850, lane, type);
            
            spawnTimer = 0;
        }
        else
        {
            spawnTimer = 0;
        }
    }
}

void drawBitcoinLife(float x, float y, float size) {
    // Círculo exterior (oro Bitcoin)
    glColor3f(0.96f, 0.65f, 0.07f);
    drawCircle(x, y, size, 32);
    
    // Letra "B" estilo Bitcoin
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.5f);
    
    // Primera barra vertical (izquierda)
    glBegin(GL_LINES);
    glVertex2f(x - size/3, y - size/2);
    glVertex2f(x - size/3, y + size/2);
    glEnd();
    
    // Segunda barra vertical (derecha)
    glBegin(GL_LINES);
    glVertex2f(x + size/6, y - size/2);
    glVertex2f(x + size/6, y + size/2);
    glEnd();
    
    // Curva superior
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - size/3, y - size/2);
    glVertex2f(x - size/6, y - size/2);
    glVertex2f(x + size/6, y - size/4);
    glVertex2f(x - size/6, y);
    glVertex2f(x + size/6, y + size/4);
    glVertex2f(x - size/6, y + size/2);
    glVertex2f(x - size/3, y + size/2);
    glEnd();
    
    // Curva central
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - size/6, y);
    glVertex2f(x + size/6, y - size/4);
    glEnd();
    
    glLineWidth(1.0f);
}

// Dibujar una piedra tipo 1 (redonda)
void drawRockType1(float x, float y, float size)
{
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    
    // Color base gris
    glColor3f(0.5f, 0.4f, 0.3f);
    
    // Forma circular aproximada usando puntos
    int radius = (int)(size / 2);
    for (int i = -radius; i <= radius; i += 2)
    {
        for (int j = -radius; j <= radius; j += 2)
        {
            // Crear forma circular
            if ((i * i + j * j) <= (radius * radius))
            {
                glVertex2f(x + i, y + j);
            }
        }
    }
    
    // Puntos más oscuros para sombras
    glColor3f(0.3f, 0.2f, 0.1f);
    for (int i = -radius/2; i <= radius/2; i += 3)
    {
        for (int j = -radius/2; j <= radius/2; j += 3)
        {
            if ((i * i + j * j) <= ((radius/2) * (radius/2)))
            {
                glVertex2f(x + i + 2, y + j - 2); // Offset para sombra
            }
        }
    }
    
    // Puntos claros para brillo
    glColor3f(0.7f, 0.6f, 0.5f);
    for (int i = -radius/3; i <= radius/3; i += 4)
    {
        for (int j = -radius/3; j <= radius/3; j += 4)
        {
            glVertex2f(x + i - 1, y + j + 1); // Offset para brillo
        }
    }
    
    glEnd();
}

// Dibujar una piedra tipo 2 (angular)
void drawRockType2(float x, float y, float size)
{
    glPointSize(3.5f);
    glBegin(GL_POINTS);
    
    // Color base café
    glColor3f(0.4f, 0.3f, 0.2f);
    
    // Forma angular usando puntos
    int halfSize = (int)(size / 2);
    
    // Crear forma angular irregular
    for (int i = -halfSize; i <= halfSize; i += 2)
    {
        for (int j = -halfSize; j <= halfSize; j += 2)
        {
            // Forma más angular que circular
            if (abs(i) + abs(j) <= halfSize + 5)
            {
                // Agregar irregularidad
                int noise = (i + j + (int)x + (int)y) % 3 - 1;
                glVertex2f(x + i + noise, y + j + noise);
            }
        }
    }
    
    // Detalles de grietas
    glColor3f(0.1f, 0.1f, 0.05f);
    for (int i = -halfSize/2; i <= halfSize/2; i += 5)
    {
        glVertex2f(x + i, y);
        glVertex2f(x, y + i);
    }
    
    glEnd();
}

// Dibujar una piedra tipo 3 (alargada)
void drawRockType3(float x, float y, float size)
{
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    
    // Color base gris verdoso
    glColor3f(0.4f, 0.5f, 0.3f);
    
    // Forma alargada
    int width = (int)(size * 1.2f);
    int height = (int)(size * 0.7f);
    
    for (int i = -width/2; i <= width/2; i += 2)
    {
        for (int j = -height/2; j <= height/2; j += 3)
        {
            // Forma elíptica
            if (((i * i) / (float)((width/2) * (width/2))) + 
                ((j * j) / (float)((height/2) * (height/2))) <= 1.0f)
            {
                glVertex2f(x + i, y + j);
            }
        }
    }
    
    // Musgo o liquen (puntos verdes)
    glColor3f(0.2f, 0.6f, 0.2f);
    for (int i = -width/3; i <= width/3; i += 6)
    {
        for (int j = -height/3; j <= height/3; j += 6)
        {
            if (rand() % 3 == 0) // Solo algunos puntos
            {
                glVertex2f(x + i, y + j);
            }
        }
    }
    
    glEnd();
}

// Renderizar todos los obstáculos
void renderObstacles()
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (obstacles[i].active)
        {
            switch (obstacles[i].type)
            {
                case 0:
                    drawRockType1(obstacles[i].x, obstacles[i].y, obstacles[i].size);
                    break;
                case 1:
                    drawRockType2(obstacles[i].x, obstacles[i].y, obstacles[i].size);
                    break;
                case 2:
                    drawRockType3(obstacles[i].x, obstacles[i].y, obstacles[i].size);
                    break;
                default:
                    drawRockType1(obstacles[i].x, obstacles[i].y, obstacles[i].size);
                    break;
            }
        }
    }
}

// Verificar colisión entre vehículo y obstáculos
bool checkObstacleCollision(float vehicleX, float vehicleY, float vehicleWidth, float vehicleHeight)
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (obstacles[i].active)
        {
            // Calcular distancia entre vehículo y obstáculo
            float obstacleLeft = obstacles[i].x - obstacles[i].size/2;
            float obstacleRight = obstacles[i].x + obstacles[i].size/2;
            float obstacleTop = obstacles[i].y + obstacles[i].size/2;
            float obstacleBottom = obstacles[i].y - obstacles[i].size/2;
            
            float vehicleLeft = vehicleX;
            float vehicleRight = vehicleX + vehicleWidth;
            float vehicleTop = vehicleY + vehicleHeight;
            float vehicleVehicleBottom = vehicleY;
            
            // Verificar solapamiento en ambos ejes
            bool overlapX = (vehicleLeft < obstacleRight) && (vehicleRight > obstacleLeft);
            bool overlapY = (vehicleVehicleBottom < obstacleTop) && (vehicleTop > obstacleBottom);
            
            if (overlapX && overlapY)
            {
                printf("¡COLISIÓN DETECTADA! Vehículo: (%.0f,%.0f) vs Obstáculo: (%.0f,%.0f)\n", 
                       vehicleX, vehicleY, obstacles[i].x, obstacles[i].y);
                return true;
            }
        }
    }
    return false;
}

// Limpiar todos los obstáculos
void clearObstacles()
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        obstacles[i].active = false;
    }
    obstacleCount = 0;
    lastSpawnX = 0;
    spawnTimer = 0;
    printf("Todos los obstáculos eliminados\n");
}

// Obtener información de obstáculos para debug
int getActiveObstacleCount()
{
    return obstacleCount;
}

// Obtener obstáculo específico (para debug o AI)
Obstacle* getObstacle(int index)
{
    if (index >= 0 && index < MAX_OBSTACLES && obstacles[index].active)
    {
        return &obstacles[index];
    }
    return NULL;
}

// Función para ajustar dificultad (llamar desde el juego principal)
void setObstacleDifficulty(int difficulty)
{
    // difficulty: 1 = fácil, 2 = medio, 3 = difícil
    switch (difficulty)
    {
        case 1: // Fácil
            // Se configuran las constantes en header.h
            break;
        case 2: // Medio
            // Aumentar frecuencia de spawn
            break;
        case 3: // Difícil
            // Máxima frecuencia
            break;
        default:
            break;
    }
}