#ifndef HEADER_H
#define HEADER_H
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
} Vehicle;

// ========== ESTRUCTURA DE OBSTÁCULOS (NUEVO) ==========
typedef struct
{
    float x, y;      // Posición del obstáculo
    int size;        // Tamaño del obstáculo
    int type;        // Tipo de piedra (0, 1, 2)
    int lane;        // Carril donde está (0=inferior, 1=superior)
    bool active;     // Si el obstáculo está activo
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

// ========== FUNCIONES DE OBSTÁCULOS (obstacles.c) - NUEVO ==========
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


// Funciones de información y configuración
int getActiveObstacleCount();
Obstacle* getObstacle(int index);
void setObstacleDifficulty(int difficulty);

// Constantes para el sistema de movimiento
#define ROAD_BOTTOM 200
#define ROAD_TOP 400
#define ROAD_CENTER 300
#define ROAD_WIDTH 800
#define LANE_HEIGHT 100
#define VEHICLE_SPEED 2.0f
#define LANE_CHANGE_SPEED 1.5f

// Nuevas constantes para spawn
#define SPAWN_LEFT_X 50    // Posición X para spawn en la izquierda
#define SPAWN_RIGHT_X 700  // Posición X para spawn en la derecha (si se necesita)
#define SPAWN_LANE_0_Y 250 // Posición Y para carril inferior
#define SPAWN_LANE_1_Y 300 // Posición Y para carril superior

// ========== CONSTANTES PARA OBSTÁCULOS (NUEVO) ==========
#define MAX_OBSTACLES 20              // Máximo número de obstáculos simultáneos
#define OBSTACLE_SPEED 2.0f           // Velocidad de movimiento de obstáculos
#define MIN_SPAWN_INTERVAL 60         // Frames mínimos entre spawn (1 segundo a 60fps)
#define OBSTACLE_SPAWN_CHANCE 15      // Porcentaje de probabilidad de spawn por frame
#define MIN_OBSTACLE_DISTANCE 150     // Distancia mínima entre obstáculos
#define OBSTACLE_COLLISION_MARGIN 5   // Margen para detección de colisiones
#define MAX_LIVES 3

#endif