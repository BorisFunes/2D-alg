#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

// Definir M_PI si no está definido
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Array de obstáculos
static Obstacle obstacles[MAX_OBSTACLES];
static int obstacleCount = 0;
static int lastSpawnX = 0; // Para controlar el espaciado
static int spawnTimer = 0; // Timer para controlar la frecuencia de spawn
static int burstTimer = 0;
static bool inBurstMode = false;
static int burstCount = 0;
static int difficultyLevel = 1;
static int gameTimeCounter = 0;
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
    if (obstacleCount >= MAX_OBSTACLES)
        return;

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
                obstacles[i].y = LANE_0_CENTER_Y; // 250
            }
            else // Carril superior
            {
                obstacles[i].y = LANE_1_CENTER_Y; // 350
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
            // Mover hacia la izquierda (efecto de avance del vehículo)
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
    
    // Solo spawnnear si ha pasado suficiente tiempo y hay espacio
    if (spawnTimer >= MIN_SPAWN_INTERVAL)
    {
        // Probabilidad de spawn (ajustable para dificultad)
        if ((rand() % 100) < OBSTACLE_SPAWN_CHANCE)
        {
            // Elegir carril aleatorio
            int lane = rand() % 2; // 0 o 1
            
            // Elegir tipo de piedra aleatorio
            int type = rand() % 3; // 0, 1 o 2 (diferentes tipos de piedra)
            
            // Spawnnear en el lado derecho de la pantalla
            spawnObstacle(850, lane, type);
            
            spawnTimer = 0; // Resetear timer
        }
        else
        {
            spawnTimer = 0; // Resetear timer aunque no se genere obstáculo
        }
    }
}

void drawBitcoinLife(float x, float y, float size)
{
    // Círculo exterior (oro Bitcoin)
    glColor3f(0.96f, 0.65f, 0.07f);
    drawCircle(x, y, size, 64); 

    // B negra
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.5f);

    // Hacemos la B un poco más grande
    float w = size * 0.5f;  // Antes 0.4f
    float h = size * 0.75f; // Antes 0.6f

    // Parte izquierda del tallo
    glBegin(GL_LINES);
    glVertex2f(x - w / 2, y - h / 2);
    glVertex2f(x - w / 2, y + h / 2);
    glEnd();

    // Parte superior del bucle superior
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - w / 2, y + h / 2);
    glVertex2f(x + w / 4, y + h / 2);
    glVertex2f(x + w / 2, y + h / 4);
    glVertex2f(x + w / 4, y);
    glVertex2f(x - w / 2, y);
    glEnd();

    // Parte inferior del bucle inferior
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - w / 2, y);
    glVertex2f(x + w / 4, y);
    glVertex2f(x + w / 2, y - h / 4);
    glVertex2f(x + w / 4, y - h / 2);
    glVertex2f(x - w / 2, y - h / 2);
    glEnd();

    // Líneas verticales que atraviesan la "B"
    glLineWidth(1.5f);
    float spacing = size * 0.1f; 

    glBegin(GL_LINES);
    // Línea izquierda
    glVertex2f(x - spacing, y - size / 1.2f);
    glVertex2f(x - spacing, y + size / 1.2f);

    // Línea derecha
    glVertex2f(x + spacing, y - size / 1.2f);
    glVertex2f(x + spacing, y + size / 1.2f);
    glEnd();

    glLineWidth(1.0f);
}


// Función para dibujar burbuja de invulnerabilidad
void drawInvulnerabilityBubble(float x, float y, float width, float height)
{
    // Habilitar transparencia
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Color azul transparente para la burbuja
    glColor4f(0.3f, 0.6f, 1.0f, 0.3f);

    // Calcular el radio basado en las dimensiones del vehículo
    float radiusX = (width / 2) + 15; // Agregar margen de 15 píxeles
    float radiusY = (height / 2) + 15;

    // Dibujar elipse para la burbuja
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + width / 2, y + height / 2); // Centro

    for (int i = 0; i <= 360; i += 10)
    {
        float angle = i * M_PI / 180.0f;
        float px = (x + width / 2) + radiusX * cos(angle);
        float py = (y + height / 2) + radiusY * sin(angle);
        glVertex2f(px, py);
    }
    glEnd();

    // Dibujar borde de la burbuja (más visible)
    glColor4f(0.3f, 0.6f, 1.0f, 0.6f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i <= 360; i += 10)
    {
        float angle = i * M_PI / 180.0f;
        float px = (x + width / 2) + radiusX * cos(angle);
        float py = (y + height / 2) + radiusY * sin(angle);
        glVertex2f(px, py);
    }
    glEnd();

    glLineWidth(1.0f);
    glDisable(GL_BLEND);
}

void drawRockType1(float x, float y, float size)
{
    glPointSize(4.0f);
    glBegin(GL_POINTS);

    int radiusX = (int)(size * 0.6f); 
    int radiusY = (int)(size * 0.4f); 

    glColor3f(0.5f, 0.5f, 0.5f);
    for (int i = -radiusX; i <= radiusX; i += 2)
    {
        for (int j = -radiusY; j <= radiusY; j += 2)
        {
            float distortion = sin(i * 0.2f) * 3 + cos(j * 0.3f) * 2;

            if ((i * i) / (float)(radiusX * radiusX) + 
                (j * j) / (float)(radiusY * radiusY) <= 1.0 + distortion * 0.01f)
            {
                glVertex2f(x + i, y + j);
            }
        }
    }

    // Sombra inferior (gris oscuro)
    glColor3f(0.25f, 0.25f, 0.25f);
    for (int i = -radiusX + 2; i <= radiusX - 2; i += 3)
    {
        for (int j = -radiusY / 2; j <= radiusY / 3; j += 3)
        {
            glVertex2f(x + i + 1, y + j - 2);
        }
    }

    // Brillo superior (gris claro)
    glColor3f(0.75f, 0.75f, 0.75f);
    for (int i = -radiusX / 3; i <= radiusX / 4; i += 4)
    {
        for (int j = radiusY / 4; j <= radiusY / 1.5; j += 4)
        {
            glVertex2f(x + i - 1, y + j + 1);
        }
    }

    glEnd();
}
// Dibujar una piedra tipo 2 (angular)
void drawRockType2(float x, float y, float size)
{
    glPointSize(3.5f);
    glBegin(GL_POINTS);

    int halfSize = (int)(size / 2);

    // Base gris oscuro
    glColor3f(0.15f, 0.15f, 0.15f);
    for (int i = -halfSize; i <= halfSize; i += 2)
    {
        for (int j = -halfSize; j <= halfSize; j += 2)
        {
            // Forma tipo diamante (angular)
            if (abs(i) + abs(j) <= halfSize + 4)
            {
                int offset = ((i * j + (int)x + (int)y) % 3) - 1;
                glVertex2f(x + i + offset, y + j + offset);
            }
        }
    }

    // Zonas de sombra más intensa (parte inferior)
    glColor3f(0.05f, 0.05f, 0.05f);
    for (int i = -halfSize + 2; i <= halfSize - 2; i += 3)
    {
        for (int j = 0; j <= halfSize; j += 3)
        {
            if (abs(i) + abs(j) <= halfSize)
                glVertex2f(x + i, y + j);
        }
    }

    // Detalles de grietas o bordes entre bloques
    glColor3f(0.25f, 0.25f, 0.25f);
    for (int i = -halfSize / 2; i <= halfSize / 2; i += 4)
    {
        glVertex2f(x + i, y + i / 2);
        glVertex2f(x - i, y + i / 3);
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

    for (int i = -width / 2; i <= width / 2; i += 2)
    {
        for (int j = -height / 2; j <= height / 2; j += 3)
        {
            // Forma elíptica
            if (((i * i) / (float)((width / 2) * (width / 2))) +
                    ((j * j) / (float)((height / 2) * (height / 2))) <=
                1.0f)
            {
                glVertex2f(x + i, y + j);
            }
        }
    }

    // Musgo o liquen (puntos verdes)
    glColor3f(0.2f, 0.6f, 0.2f);
    for (int i = -width / 3; i <= width / 3; i += 6)
    {
        for (int j = -height / 3; j <= height / 3; j += 6)
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

// Función para determinar el carril del vehículo basado en su posición Y - CORREGIDA
int getVehicleLane(float vehicleY)
{
    // Usar LANE_BOUNDARY_Y definido en header.h (300)
    if (vehicleY < LANE_BOUNDARY_Y)
        return 0; // Carril inferior
    else
        return 1; // Carril superior
}

// Nueva función para verificar si el vehículo está saltando
bool isVehicleJumping(Vehicle *vehicle)
{
    if (vehicle == NULL)
        return false;

    // Si el vehículo está en el aire (Y > posición normal del carril)
    float normalY = (vehicle->currentLane == 0) ? LANE_0_CENTER_Y : LANE_1_CENTER_Y;

    // Consideramos que está saltando si está más de 30 píxeles arriba de su posición normal
    return (vehicle->y > normalY + 30);
}

// Verificar colisión entre vehículo y obstáculos - TOTALMENTE CORREGIDO
bool checkObstacleCollision(float vehicleX, float vehicleY, float vehicleWidth, float vehicleHeight)
{
    // Obtener el vehículo para verificar si está saltando
    Vehicle *playerVehicle = getVehicle(0);
    if (playerVehicle == NULL)
        return false;

    // Determinar en qué carril está el vehículo usando su posición Y actual
    int vehicleLane = getVehicleLane(vehicleY);

    // Verificar si el vehículo está saltando
    bool jumping = isVehicleJumping(playerVehicle);

    printf("DEBUG: Vehículo en Y=%.0f, carril: %d, saltando: %s\n",
           vehicleY, vehicleLane, jumping ? "SÍ" : "NO");

    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (obstacles[i].active)
        {
            // NUEVO: Si el vehículo está saltando, no verificar colisiones
            if (jumping)
            {
                printf("DEBUG: Vehículo saltando - ignorando todas las colisiones\n");
                continue; // Saltar verificación de colisión
            }

            // NUEVO: Solo verificar colisión si están en el mismo carril Y el vehículo NO está saltando
            if (obstacles[i].lane != vehicleLane)
            {
                printf("DEBUG: Obstáculo %d ignorado - carriles diferentes (vehículo: %d, obstáculo: %d)\n",
                       i, vehicleLane, obstacles[i].lane);
                continue; // Saltar este obstáculo, no están en el mismo carril
            }

            // Calcular bounds de colisión con un margen más preciso
            float obstacleLeft = obstacles[i].x - obstacles[i].size / 2;
            float obstacleRight = obstacles[i].x + obstacles[i].size / 2;
            float obstacleTop = obstacles[i].y + obstacles[i].size / 2;
            float obstacleBottom = obstacles[i].y - obstacles[i].size / 2;

            float vehicleLeft = vehicleX;
            float vehicleRight = vehicleX + vehicleWidth;
            float vehicleTop = vehicleY + vehicleHeight;
            float vehicleBottom = vehicleY;

            // Verificar solapamiento en ambos ejes con margen de error reducido
            bool overlapX = (vehicleLeft < obstacleRight - 5) && (vehicleRight > obstacleLeft + 5);
            bool overlapY = (vehicleBottom < obstacleTop - 5) && (vehicleTop > obstacleBottom + 5);

            if (overlapX && overlapY)
            {
                printf("¡COLISIÓN DETECTADA! Vehículo carril %d: (%.0f,%.0f) vs Obstáculo carril %d: (%.0f,%.0f)\n",
                       vehicleLane, vehicleX, vehicleY, obstacles[i].lane, obstacles[i].x, obstacles[i].y);
                return true;
            }
        }
    }
    return false;
}

// Nueva función para renderizar la burbuja de invulnerabilidad del vehículo
void renderInvulnerabilityBubble(float vehicleX, float vehicleY, float vehicleWidth, float vehicleHeight)
{
    if (isInvulnerable)
    {
        drawInvulnerabilityBubble(vehicleX, vehicleY, vehicleWidth, vehicleHeight);
    }
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
Obstacle *getObstacle(int index)
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