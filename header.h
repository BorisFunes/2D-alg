#ifndef ROAD_H
#define ROAD_H
#include <stdbool.h>
// Variables globales compartidas
extern float offset;
extern float pointOffset;
extern float landscapeOffset;
extern int frameCount;

extern int selectedVehicle;
extern bool enMenu;

//funciones de menu 
void drawMenu();
void drawGame();
void drawText(float x, float y, const char* text);

// Funciones de carretera
void drawRoad();
void drawRoadPoints();
void drawLaneLines(float y);
void drawSign();

// Funciones de paisaje
void drawForest();
void drawMountain();

// Funciones de vehiculos
void drawBus(int x, int y);
void drawCar(int x, int y);
void drawDelivery(int x, int y);
void drawCoaster(int x, int y); 
#endif
