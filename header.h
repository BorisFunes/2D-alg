#ifndef ROAD_H
#define ROAD_H

// Variables globales compartidas
extern float offset;
extern float pointOffset;
extern float landscapeOffset;
extern int frameCount;

// Funciones de carretera
void drawRoad();
void drawRoadPoints();
void drawLaneLines(float y);
void drawSign();

// Funciones de paisaje
void drawForest();
void drawMountain();

#endif