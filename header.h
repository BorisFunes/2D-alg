#ifndef HEADER_H
#define HEADER_H
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definiciones matemáticas
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Variables globales compartidas
extern float offset;
extern float pointOffset;
extern float landscapeOffset;
extern int frameCount;
extern int selectedVehicle;
extern bool enMenu;
extern bool gamePaused; // Nueva variable para control de pausa
extern bool gameOver;
extern int playerLives;
extern bool isInvulnerable;
extern clock_t invulnerabilityTimer;

// Estructura del vehículo
typedef struct
{
    float x, y;          // Posición actual
    float targetY;       // Posición Y objetivo (para cambios de carril)
    int vehicleType;     // 0=delivery, 1=car, 2=coaster, 3=bus
    bool isChangingLane; // Si está cambiando de carril
    int currentLane;     // Carril actual (0=inferior, 1=superior)
    float speed;         // Velocidad individual del vehículo
    bool isJumping;      // NUEVO: Para verificar si está saltando
    float jumpHeight;    // NUEVO: Altura actual del salto
    float jumpSpeed;     // NUEVO: Velocidad del salto
} Vehicle;

// ========== ESTRUCTURA DE OBSTÁCULOS ==========
typedef struct
{
    float x, y;  // Posición del obstáculo
    int size;    // Tamaño del obstáculo
    int type;    // Tipo de piedra (0, 1, 2)
    int lane;    // Carril donde está (0=inferior, 1=superior)
    bool active; // Si el obstáculo está activo
} Obstacle;

// Funciones de menú y juego
void drawMenu();
void drawGame();
void drawText(float x, float y, const char *text);
void drawGameInfo();     // Nueva función para mostrar info del juego
void drawPauseMessage(); // Nueva función para mostrar mensaje de pausa

// Funciones de carretera
void drawRoad();
void drawRoadPoints();
void drawLaneLines(float y);
void drawSign();

// Funciones de paisaje
void drawForest();
void drawMountain();

// NUEVAS FUNCIONES AUXILIARES PARA PRIMITIVAS OPTIMIZADAS
void drawTriangleStrip(float *vertices, int count);
void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void drawFilledCircle(float cx, float cy, float radius, int segments);
void drawFilledEllipse(float cx, float cy, float radiusX, float radiusY, int segments);
void drawThickLine(int x0, int y0, int x1, int y1, int thickness);
void drawFilledPolygon(float *vertices, int vertexCount);

// Funciones de vehículos (dibujo)
void drawBus(int x, int y);
void drawCar(int x, int y);
void drawDelivery(int x, int y);
void drawCoaster(int x, int y);

// Funciones auxiliares para dibujo
void drawCircle(float cx, float cy, float r, int segments);
void drawRect(float x1, float y1, float x2, float y2);
void drawRectOutline(float x1, float y1, float x2, float y2);
void drawWheel(float x, float y, float radius, float innerRadius);
void drawWindows(float startX, float y, float width, float height, int count, float spacing, float r, float g, float b);
void drawWindowsOutline(float startX, float y, float width, float height, int count, float spacing);
void drawLight(float x, float y, float width, float height, float r, float g, float b);
void drawHorizontalLines(float x, float y, float width, int count, float spacing);

// ========== FUNCIONES DE MOVIMIENTO (move.c) ==========
// Funciones de inicialización
void initVehicle(int index, float x, float y, int type);
void clearVehicles();

// Funciones de movimiento básico
void moveVehicleForward(int index);  // Mantener para compatibilidad
void moveVehicleBackward(int index); // Mantener para compatibilidad
void moveVehicleLeft(int index);     // NUEVA: Movimiento manual izquierda
void moveVehicleRight(int index);    // NUEVA: Movimiento manual derecha
void moveVehicleUp(int index);
void moveVehicleDown(int index);

// Funciones de salto
void jumpVehicle(int index);

// Funciones de control
void controlVehicle(int index, unsigned char key);
void changeVehicleLane(int index);

// Funciones de actualización y renderizado
void updateVehicles();
void renderVehicles();

// Funciones de utilidad
bool isWithinRoadBounds(float x, float y);
bool checkCollision(int index, float newX, float newY);
float getLaneCenterY(int lane);

// Funciones de información
Vehicle *getVehicle(int index);
int getVehicleCount();

// ========== FUNCIONES DE OBSTÁCULOS (obstacles.c) ==========
// Funciones de inicialización
void initObstacles();
void clearObstacles();

// Funciones de spawn y manejo
void spawnObstacle(float x, int lane, int type);
void updateObstacles();

// Funciones de renderizado
void renderObstacles();
void drawRockType1(float x, float y, float size);
void drawRockType2(float x, float y, float size);
void drawRockType3(float x, float y, float size);
void drawBitcoinLife(float x, float y, float size);

// Funciones de colisión
bool checkObstacleCollision(float vehicleX, float vehicleY, float vehicleWidth, float vehicleHeight);
void renderInvulnerabilityBubble(float vehicleX, float vehicleY, float vehicleWidth, float vehicleHeight);

// Funciones de información y configuración
int getActiveObstacleCount();
Obstacle *getObstacle(int index);
void setObstacleDifficulty(int difficulty);

// Constantes para el sistema de movimiento
#define ROAD_BOTTOM 200
#define ROAD_TOP 400
#define ROAD_CENTER 300
#define ROAD_WIDTH 800
#define LANE_HEIGHT 100
#define VEHICLE_SPEED 2.0f
#define LANE_CHANGE_SPEED 1.5f

// Constantes para spawn de vehículos
#define SPAWN_LEFT_X 50   // Posición X para spawn en la izquierda
#define SPAWN_RIGHT_X 700 // Posición X para spawn en la derecha (si se necesita)

// ========== CONSTANTES CORREGIDAS PARA CARRILES ==========
// Estas coordenadas deben coincidir con las del juego
#define LANE_0_CENTER_Y 250 // Centro del carril inferior
#define LANE_1_CENTER_Y 350 // Centro del carril superior
#define LANE_BOUNDARY_Y 300 // Límite entre carriles (Y < 300 = carril 0, Y >= 300 = carril 1)

// ========== CONSTANTES PARA OBSTÁCULOS - INCREMENTO MASIVO ==========
#define MAX_OBSTACLES 50             // Máximo número de obstáculos simultáneos - DUPLICADO
#define OBSTACLE_SPEED 4.0f          // Velocidad de movimiento de obstáculos - AUMENTADO
#define MIN_SPAWN_INTERVAL 25        // Frames mínimos entre spawn - MUY REDUCIDO (menos de medio segundo)
#define OBSTACLE_SPAWN_CHANCE 35     // Porcentaje de probabilidad de spawn por frame - MUY AUMENTADO
#define DUAL_SPAWN_CHANCE 25         // NUEVO: Probabilidad de spawn en ambos carriles simultáneamente
#define TRIPLE_SPAWN_CHANCE 15       // NUEVO: Probabilidad de spawn múltiple en un carril
#define MIN_OBSTACLE_DISTANCE 80     // Distancia mínima entre obstáculos - MUY REDUCIDO
#define OBSTACLE_COLLISION_MARGIN 10 // Margen para detección de colisiones
#define MAX_LIVES 3
#define INVULNERABILITY_TIME 2.0f // Tiempo de invulnerabilidad en segundos

// NUEVAS CONSTANTES PARA SALTO
#define JUMP_HEIGHT 50.0f // Altura máxima del salto
#define JUMP_SPEED 8.0f   // Velocidad inicial del salto
#define GRAVITY 0.5f      // Gravedad que afecta el salto

// NUEVAS CONSTANTES PARA SPAWN AVANZADO
#define BURST_SPAWN_CHANCE 10    // NUEVO: Probabilidad de ráfaga de obstáculos
#define MAX_BURST_OBSTACLES 4    // NUEVO: Máximo obstáculos en una ráfaga
#define PROGRESSIVE_DIFFICULTY 1 // NUEVO: Habilitar dificultad progresiva

// CONSTANTES PARA OPTIMIZACIÓN DE PRIMITIVAS
#define CIRCLE_SEGMENTS 16      // Segmentos para círculos (balance calidad/rendimiento)
#define TREE_DETAIL_LEVEL 8     // Nivel de detalle para árboles
#define MOUNTAIN_DETAIL_LEVEL 4 // Nivel de detalle para montañas
#define FOREST_DENSITY 80       // Espaciado entre elementos del bosque
#define FLOWER_SPACING 120      // Espaciado entre flores

#endif