#ifndef HEADER_H
#define HEADER_H
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Variables globales compartidas
extern float offset;
extern float pointOffset;
extern float landscapeOffset;
extern int frameCount;
extern int selectedVehicle;
extern bool enMenu;
extern bool gamePaused; // Nueva variable para control de pausa

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
bool checkCollision(int vehicleIndex, float newX, float newY);
float getLaneCenterY(int lane);

// Funciones de información
Vehicle *getVehicle(int index);
int getVehicleCount();

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

#endif