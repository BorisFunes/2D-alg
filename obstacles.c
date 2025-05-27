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
    // Incrementar contador de tiempo de juego para dificultad progresiva
    gameTimeCounter++;

    // Aumentar dificultad cada 1800 frames (30 segundos a 60fps)
    if (PROGRESSIVE_DIFFICULTY && gameTimeCounter % 1800 == 0 && difficultyLevel < 5)
    {
        difficultyLevel++;
        printf("¡Dificultad aumentada a nivel %d!\n", difficultyLevel);
    }

    // Mover obstáculos existentes
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (obstacles[i].active)
        {
            // Velocidad ajustada por dificultad
            float currentSpeed = OBSTACLE_SPEED + (difficultyLevel * 0.5f);
            obstacles[i].x -= currentSpeed;

            // Eliminar obstáculos que salieron de pantalla
            if (obstacles[i].x < -50)
            {
                obstacles[i].active = false;
                obstacleCount--;
            }
        }
    }

    // Sistema de spawn mejorado con múltiples modos
    spawnTimer++;

    // Calcular intervalo dinámico basado en dificultad
    int dynamicInterval = MIN_SPAWN_INTERVAL - (difficultyLevel * 3);
    if (dynamicInterval < 10)
        dynamicInterval = 10; // Mínimo absoluto

    // Calcular probabilidad dinámica
    int dynamicChance = OBSTACLE_SPAWN_CHANCE + (difficultyLevel * 5);
    if (dynamicChance > 60)
        dynamicChance = 60; // Máximo 60%

    if (spawnTimer >= dynamicInterval)
    {
        int spawnRoll = rand() % 100;

        // MODO 1: Ráfaga de obstáculos (más común a mayor dificultad)
        if (spawnRoll < (BURST_SPAWN_CHANCE + difficultyLevel * 2) && !inBurstMode)
        {
            printf("¡INICIANDO RÁFAGA DE OBSTÁCULOS!\n");
            inBurstMode = true;
            burstCount = 2 + (rand() % (MAX_BURST_OBSTACLES - 1)); // 2-4 obstáculos
            burstTimer = 0;
            spawnTimer = 0;
        }
        // MODO 2: Spawn simultáneo en ambos carriles
        else if (spawnRoll < (DUAL_SPAWN_CHANCE + difficultyLevel * 3))
        {
            printf("¡SPAWN DUAL EN AMBOS CARRILES!\n");

            // Spawn en carril inferior
            int type1 = rand() % 3;
            spawnObstacle(850, 0, type1);

            // Spawn en carril superior (con pequeño offset)
            int type2 = rand() % 3;
            spawnObstacle(870 + (rand() % 40), 1, type2);

            spawnTimer = 0;
        }
        // MODO 3: Triple spawn en un solo carril
        else if (spawnRoll < (TRIPLE_SPAWN_CHANCE + difficultyLevel * 2))
        {
            printf("¡TRIPLE SPAWN EN UN CARRIL!\n");

            int lane = rand() % 2;

            // Crear 3 obstáculos en el mismo carril con diferentes posiciones X
            for (int j = 0; j < 3; j++)
            {
                int type = rand() % 3;
                float xPos = 850 + (j * (MIN_OBSTACLE_DISTANCE + 20));
                spawnObstacle(xPos, lane, type);
            }

            spawnTimer = 0;
        }
        // MODO 4: Spawn normal (más frecuente)
        else if (spawnRoll < dynamicChance)
        {
            int lane = rand() % 2;
            int type = rand() % 3;
            spawnObstacle(850, lane, type);
            spawnTimer = 0;
        }
        else
        {
            spawnTimer = 0;
        }
    }

    // Manejar modo ráfaga
    if (inBurstMode)
    {
        burstTimer++;

        // Spawn cada 8 frames durante la ráfaga
        if (burstTimer >= 8 && burstCount > 0)
        {
            int lane = rand() % 2;
            int type = rand() % 3;

            // Posiciones variadas para la ráfaga
            float xPos = 850 + (rand() % 100);
            spawnObstacle(xPos, lane, type);

            burstCount--;
            burstTimer = 0;

            printf("Obstáculo de ráfaga spawneado. Restantes: %d\n", burstCount);
        }

        // Terminar ráfaga
        if (burstCount <= 0)
        {
            inBurstMode = false;
            burstTimer = 0;
            printf("¡RÁFAGA TERMINADA!\n");
        }
    }

    // Sistema adicional: Spawn de obstáculos extra cada cierto tiempo
    static int extraSpawnTimer = 0;
    extraSpawnTimer++;

    // Cada 120 frames (2 segundos), posibilidad de spawn extra
    if (extraSpawnTimer >= 120)
    {
        if ((rand() % 100) < (20 + difficultyLevel * 5)) // 20-45% de probabilidad
        {
            printf("¡SPAWN EXTRA ACTIVADO!\n");

            // Determinar tipo de spawn extra
            int extraType = rand() % 3;

            switch (extraType)
            {
            case 0: // Obstáculos alternados
                spawnObstacle(850, 0, rand() % 3);
                spawnObstacle(920, 1, rand() % 3);
                break;

            case 1: // Barrera completa (obstáculos en ambos carriles muy juntos)
                spawnObstacle(850, 0, rand() % 3);
                spawnObstacle(855, 1, rand() % 3);
                printf("¡BARRERA COMPLETA CREADA!\n");
                break;

            case 2: // Cadena de obstáculos
                for (int k = 0; k < 2 + difficultyLevel; k++)
                {
                    int chainLane = rand() % 2;
                    spawnObstacle(850 + (k * 60), chainLane, rand() % 3);
                }
                printf("¡CADENA DE OBSTÁCULOS CREADA!\n");
                break;
            }
        }
        extraSpawnTimer = 0;
    }

    // Debug info cada 300 frames
    static int debugTimer = 0;
    debugTimer++;
    if (debugTimer >= 300)
    {
        printf("=== ESTADO OBSTÁCULOS ===\n");
        printf("Activos: %d/%d | Dificultad: %d | Ráfaga: %s\n",
               obstacleCount, MAX_OBSTACLES, difficultyLevel,
               inBurstMode ? "SÍ" : "NO");
        debugTimer = 0;
    }
}
void drawBitcoinLife(float x, float y, float size)
{
    // Círculo exterior (oro Bitcoin)
    glColor3f(0.96f, 0.65f, 0.07f);
    drawCircle(x, y, size, 32);

    // Letra "B" estilo Bitcoin
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.5f);

    // Primera barra vertical (izquierda)
    glBegin(GL_LINES);
    glVertex2f(x - size / 3, y - size / 2);
    glVertex2f(x - size / 3, y + size / 2);
    glEnd();

    // Segunda barra vertical (derecha)
    glBegin(GL_LINES);
    glVertex2f(x + size / 6, y - size / 2);
    glVertex2f(x + size / 6, y + size / 2);
    glEnd();

    // Curva superior
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - size / 3, y - size / 2);
    glVertex2f(x - size / 6, y - size / 2);
    glVertex2f(x + size / 6, y - size / 4);
    glVertex2f(x - size / 6, y);
    glVertex2f(x + size / 6, y + size / 4);
    glVertex2f(x - size / 6, y + size / 2);
    glVertex2f(x - size / 3, y + size / 2);
    glEnd();

    // Curva central
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - size / 6, y);
    glVertex2f(x + size / 6, y - size / 4);
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
    for (int i = -radius / 2; i <= radius / 2; i += 3)
    {
        for (int j = -radius / 2; j <= radius / 2; j += 3)
        {
            if ((i * i + j * j) <= ((radius / 2) * (radius / 2)))
            {
                glVertex2f(x + i + 2, y + j - 2); // Offset para sombra
            }
        }
    }

    // Puntos claros para brillo
    glColor3f(0.7f, 0.6f, 0.5f);
    for (int i = -radius / 3; i <= radius / 3; i += 4)
    {
        for (int j = -radius / 3; j <= radius / 3; j += 4)
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
    for (int i = -halfSize / 2; i <= halfSize / 2; i += 5)
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