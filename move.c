#include <GL/glut.h>
#include <stdbool.h>
#include "header.h"
#include <time.h>

// Definiciones de límites de la carretera
#define ROAD_BOTTOM 200
#define ROAD_TOP 400
#define ROAD_CENTER 300
#define ROAD_WIDTH 800
#define LANE_HEIGHT 100 // Altura de cada carril
#define VEHICLE_SPEED 2.0f
#define LANE_CHANGE_SPEED 1.5f

// Array de vehículos (máximo 10 vehículos)
static Vehicle vehicles[10];
static int vehicleCount = 0;

// VARIABLES DE SALTO CORREGIDAS
static bool vehicleJumping[10] = {false}; // Array para cada vehículo
static clock_t jumpStartTime[10] = {0};   // Tiempo de inicio del salto usando clock_t
static float originalY[10] = {0.0f};      // Posición Y original

// Función para inicializar un vehículo
void initVehicle(int index, float x, float y, int type)
{
    if (index >= 0 && index < 10)
    {
        vehicles[index].x = x;
        vehicles[index].y = y;
        vehicles[index].targetY = y;
        vehicles[index].vehicleType = type;
        vehicles[index].isChangingLane = false;
        vehicles[index].speed = VEHICLE_SPEED; // Velocidad fija, no aleatoria

        // Determinar carril inicial
        if (y < ROAD_CENTER)
        {
            vehicles[index].currentLane = 0; // Carril inferior
        }
        else
        {
            vehicles[index].currentLane = 1; // Carril superior
        }

        // IMPORTANTE: Inicializar variables de salto
        vehicleJumping[index] = false;
        jumpStartTime[index] = 0;
        originalY[index] = y;

        if (index >= vehicleCount)
        {
            vehicleCount = index + 1;
        }
    }
}

// Función para verificar si una posición está dentro de los límites de la carretera
bool isWithinRoadBounds(float x, float y)
{
    return (x >= 0 && x <= ROAD_WIDTH &&
            y >= ROAD_BOTTOM + 20 && y <= ROAD_TOP - 20);
}

// Función para obtener la posición Y del centro de un carril
float getLaneCenterY(int lane)
{
    if (lane == 0)
    {
        return ROAD_BOTTOM + LANE_HEIGHT / 2; // Centro del carril inferior (250)
    }
    else
    {
        return ROAD_CENTER + LANE_HEIGHT / 2; // Centro del carril superior (350)
    }
}

// Función para detectar colisiones entre vehículos
bool checkCollision(int vehicleIndex, float newX, float newY)
{
    for (int i = 0; i < vehicleCount; i++)
    {
        if (i != vehicleIndex)
        {
            float dx = newX - vehicles[i].x;
            float dy = newY - vehicles[i].y;
            float distance = sqrt(dx * dx + dy * dy);

            // Distancia mínima basada en el tipo de vehículo
            float minDistance = 60.0f; // Distancia base
            if (vehicles[i].vehicleType == 3 || vehicles[vehicleIndex].vehicleType == 3)
            {
                minDistance = 80.0f; // Mayor distancia para buses
            }

            if (distance < minDistance)
            {
                return true; // Hay colisión
            }
        }
    }
    return false; // No hay colisión
}

// Función para mover vehículo hacia la izquierda
void moveVehicleLeft(int index)
{
    if (index >= 0 && index < vehicleCount)
    {
        float newX = vehicles[index].x - vehicles[index].speed;

        // Mantener el vehículo dentro de los límites de la pantalla
        if (newX >= 0 && !checkCollision(index, newX, vehicles[index].y))
        {
            vehicles[index].x = newX;
        }
    }
}

// Función para mover vehículo hacia la derecha
void moveVehicleRight(int index)
{
    if (index >= 0 && index < vehicleCount)
    {
        float newX = vehicles[index].x + vehicles[index].speed;

        // Mantener el vehículo dentro de los límites de la pantalla
        // Ajustar según el ancho del vehículo
        float vehicleWidth = 80; // Ancho aproximado del vehículo más grande (bus)
        if (vehicles[index].vehicleType == 0)
            vehicleWidth = 50; // Delivery
        else if (vehicles[index].vehicleType == 1)
            vehicleWidth = 80; // Car
        else if (vehicles[index].vehicleType == 2)
            vehicleWidth = 100; // Coaster
        else if (vehicles[index].vehicleType == 3)
            vehicleWidth = 140; // Bus

        if (newX + vehicleWidth <= ROAD_WIDTH && !checkCollision(index, newX, vehicles[index].y))
        {
            vehicles[index].x = newX;
        }
    }
}

// Función para cambiar de carril
void changeVehicleLane(int index)
{
    if (index >= 0 && index < vehicleCount && !vehicles[index].isChangingLane)
    {
        int newLane = (vehicles[index].currentLane == 0) ? 1 : 0;
        float newTargetY = getLaneCenterY(newLane);

        // Verificar si el cambio de carril es seguro
        if (!checkCollision(index, vehicles[index].x, newTargetY))
        {
            vehicles[index].targetY = newTargetY;
            vehicles[index].currentLane = newLane;
            vehicles[index].isChangingLane = true;
        }
    }
}

// Función para mover vehículo arriba (cambio de carril hacia arriba)
void moveVehicleUp(int index)
{
    if (index >= 0 && index < vehicleCount && vehicles[index].currentLane == 0)
    {
        changeVehicleLane(index);
    }
}

// Función para mover vehículo abajo (cambio de carril hacia abajo)
void moveVehicleDown(int index)
{
    if (index >= 0 && index < vehicleCount && vehicles[index].currentLane == 1)
    {
        changeVehicleLane(index);
    }
}

// FUNCIÓN DE SALTO CORREGIDA
void jumpVehicle(int index)
{
    if (index >= 0 && index < vehicleCount && !vehicleJumping[index])
    {
        vehicleJumping[index] = true;
        jumpStartTime[index] = clock(); // Usar clock() directamente
        originalY[index] = vehicles[index].y;
        printf("Vehiculo %d iniciando salto desde Y=%.2f\n", index, originalY[index]);
    }
}

// FUNCIÓN DE ACTUALIZACIÓN CORREGIDA
void updateVehicles()
{
    // Actualizar saltos primero
    clock_t currentTime = clock();

    for (int i = 0; i < vehicleCount; i++)
    {
        if (vehicleJumping[i])
        {
            // Calcular tiempo transcurrido en segundos
            double elapsedTime = (double)(currentTime - jumpStartTime[i]) / CLOCKS_PER_SEC;

            if (elapsedTime >= 0.3) // 0.3 segundos de salto (muy rápido)
            {
                // Terminar salto - volver a posición original
                vehicleJumping[i] = false;
                vehicles[i].y = originalY[i];
                printf("Vehiculo %d termino salto, Y=%.2f\n", i, vehicles[i].y);
            }
            else
            {
                // Calcular altura del salto usando parábola
                float t = (float)(elapsedTime / 0.3); // Normalizar de 0 a 1 en 0.3 segundos
                float jumpHeight = 100.0f;            // Mantener la misma altura

                // Fórmula parabólica: máximo en t=0.5, cero en t=0 y t=1
                float jumpOffset = jumpHeight * (4.0f * t * (1.0f - t));
                vehicles[i].y = originalY[i] + jumpOffset;

                // Debug: mostrar posición cada cierto tiempo
                static int debugCounter = 0;
                if (debugCounter % 10 == 0) // Más frecuente debido a la mayor velocidad
                {
                    printf("Vehiculo %d saltando: t=%.2f, Y=%.2f (original=%.2f, offset=%.2f)\n",
                           i, t, vehicles[i].y, originalY[i], jumpOffset);
                }
                debugCounter++;
            }
        }
    }

    // Manejo del cambio de carril
    for (int i = 0; i < vehicleCount; i++)
    {
        if (vehicles[i].isChangingLane)
        {
            float dy = vehicles[i].targetY - vehicles[i].y;

            if (fabs(dy) > LANE_CHANGE_SPEED)
            {
                // Mover gradualmente hacia el carril objetivo
                if (dy > 0)
                {
                    vehicles[i].y += LANE_CHANGE_SPEED;
                    // Solo actualizar originalY si no está saltando
                    if (!vehicleJumping[i])
                        originalY[i] += LANE_CHANGE_SPEED;
                }
                else
                {
                    vehicles[i].y -= LANE_CHANGE_SPEED;
                    // Solo actualizar originalY si no está saltando
                    if (!vehicleJumping[i])
                        originalY[i] -= LANE_CHANGE_SPEED;
                }
            }
            else
            {
                // Llegó al carril objetivo
                vehicles[i].y = vehicles[i].targetY;
                vehicles[i].isChangingLane = false;
                // Solo actualizar originalY si no está saltando
                if (!vehicleJumping[i])
                    originalY[i] = vehicles[i].targetY;
            }
        }
        // Si no está cambiando de carril y no está saltando, mantener Y original
        else if (!vehicleJumping[i])
        {
            originalY[i] = vehicles[i].y;
        }
    }
}

// Función para renderizar todos los vehículos
void renderVehicles()
{
    for (int i = 0; i < vehicleCount; i++)
    {
        switch (vehicles[i].vehicleType)
        {
        case 0:
            drawDelivery((int)vehicles[i].x, (int)vehicles[i].y);
            break;
        case 1:
            drawCar((int)vehicles[i].x, (int)vehicles[i].y);
            break;
        case 2:
            drawCoaster((int)vehicles[i].x, (int)vehicles[i].y);
            break;
        case 3:
            drawBus((int)vehicles[i].x, (int)vehicles[i].y);
            break;
        }
    }
}

// Función para controlar un vehículo específico con teclado
void controlVehicle(int index, unsigned char key)
{
    if (index < 0 || index >= vehicleCount)
        return;

    switch (key)
    {
    case 'w':
    case 'W':
        moveVehicleUp(index);
        break;
    case 's':
    case 'S':
        moveVehicleDown(index);
        break;
    case 'a':
    case 'A':
        // Mover hacia la izquierda
        moveVehicleLeft(index);
        break;
    case 'd':
    case 'D':
        // Mover hacia la derecha
        moveVehicleRight(index);
        break;
    }
}

// Función para obtener información de un vehículo
Vehicle *getVehicle(int index)
{
    if (index >= 0 && index < vehicleCount)
    {
        return &vehicles[index];
    }
    return NULL;
}

// Función para obtener el número total de vehículos
int getVehicleCount()
{
    return vehicleCount;
}

// Función para limpiar todos los vehículos
void clearVehicles()
{
    vehicleCount = 0;
    // Limpiar también las variables de salto
    for (int i = 0; i < 10; i++)
    {
        vehicleJumping[i] = false;
        jumpStartTime[i] = 0;
        originalY[i] = 0.0f;
    }
}

// Funciones adicionales para compatibilidad (mantienen las funciones originales)
void moveVehicleForward(int index)
{
    moveVehicleLeft(index); // Forward ahora es hacia la izquierda
}

void moveVehicleBackward(int index)
{
    moveVehicleRight(index); // Backward ahora es hacia la derecha
}