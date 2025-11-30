#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>
#include <iostream>


// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================
const int Dirnorth = 0;
const int Direast = 1;
const int Dirsouth = 2;
const int Dirwest = 3;
// Storage for planned moves (for collisions).
int plannedX[10] = {};
int plannedY[10] = {};
int plannedDirection[10] = {};

// Previous positions (to detect switch entry).

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() {
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 0) {  // Train not yet active
            if (trainSpawnTick[i] == tick) {
                trainActive[i] = 1;
                // Initialize previous position to current position
                trainprevX[i] = trainX[i];
                trainprevY[i] = trainY[i];
            }
        }
    }

}
// ---------------------toggle-------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition(int trainIndex) {
    int currentX = trainX[trainIndex];
    int currentY = trainY[trainIndex];
    int currentDir = trainDirection[trainIndex];
    
    int newDir = currentDir;
    int nextX = currentX;
    int nextY = currentY;
    char currentTile = grid[currentY * cols + currentX];

    //Check if current tile is a crossing and update direction
    if (currentTile == '+') {
        newDir = getSmartDirectionAtCrossing(trainIndex);
    }
    
    if (currentTile == '/' || currentTile == '\\') {
        newDir = getNextDirection(nextX, nextY, currentDir);
    }
    
    else if (currentTile >= 'A' && currentTile <= 'Z' && currentTile != 'S' && currentTile != 'D') {
        newDir = getNextDirection(nextX, nextY, currentDir);
    }
    
    else if (currentTile == '-' || currentTile == '|' || currentTile == '=') {
        newDir = getNextDirection(nextX, nextY, currentDir);
    }

    else if (currentTile == 'S' || currentTile == 'D') {
        newDir = currentDir;
    }
    // Store planned position

    if (newDir == Dirnorth) nextY--;
    else if (newDir == Dirsouth) nextY++;
    else if (newDir == Dirwest) nextX--;
    else if (newDir == Direast) nextX++;

    if (!isTrackTile(nextX, nextY)) {
        trainActive[trainIndex] = 0;
        numCrashed++;
    }

    plannedX[trainIndex] = nextX;
    plannedY[trainIndex] = nextY;
    plannedDirection[trainIndex] = newDir;
    
    return true;
  
    
}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int x , int y , int currentDir) {
    //   if (!isInBounds(x, y)) return currentDir;
    
    char tile = grid[y * cols + x];
    
    // Straight tracks maintain direction
    if (tile == '=' && (currentDir == Dirwest || currentDir == Direast)) {
        return currentDir;
    }
    if (tile == '|' && (currentDir == Dirsouth|| currentDir == Dirnorth)) {
        return currentDir;
    }
    
    // Curves change direction
    if (tile == '/') {
        if (currentDir == Direast) return Dirnorth;
        if (currentDir == Dirwest) return Dirsouth;         
        if (currentDir == Dirnorth) return Direast;         
        if (currentDir == Dirsouth) return Dirwest;      
    }
    if (tile == '\\') {
        if (currentDir == Direast) return Dirsouth;
        if (currentDir == Dirwest) return Dirnorth;
        if (currentDir == Dirnorth) return Dirwest;
        if (currentDir == Dirsouth) return Direast;
    }
    
    if (tile >= 'A' && tile <= 'Z') {
    int switchIdx = getSwitchIndex(tile);
    return getSwitchStateForDirection(switchIdx);
}
    
    return currentDir;
}

// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing(int trainIndex) {

    int currentX = trainX[trainIndex];
    int currentY = trainY[trainIndex];
    int targetX = destX[trainIndex];
    int targetY = destY[trainIndex];
    
    // Calculate differences
    int differx = targetX - currentX;
    int differy = targetY - currentY;
    
    // Make distances positive for comparison
    int distX = differx;
    if (distX < 0) distX = -distX;
    
    int distY = differy;
    if (distY < 0) distY = -distY;
    
    // Prefer horizontal movement if horizontal distance is greater
    if (distX > distY) {
        if (differx > 0) return Direast;  // Move down (increasing x)
        else return Dirwest;           // Move up (decreasing x)
    } else {
        if (differy > 0) return Dirsouth; // Move right (increasing y)
        else return Dirnorth;         // Move left (decreasing y)
    }
}

// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            if (trainDelay[i] > 0){
                plannedX[i] = trainX[i];
                plannedY[i] = trainY[i];
                plannedDirection[i] = trainDirection[i];
                continue; 
            }
            determineNextPosition(i);
        }
    }
}

void moveAllTrains() {
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            if (trainDelay[i] > 0)
                trainDelay[i]--;
            trainprevX[i] = trainX[i];
            trainprevY[i] = trainY[i];

            trainX[i] = plannedX[i];
            trainY[i] = plannedY[i];
            trainDirection[i] = plannedDirection[i];


            if (weatherCondition == 1) { 
                if ((rand() % 100) < 10) {
                    trainDelay[i] = 1;
                }
            }

            if (grid[trainY[i]*cols + trainX[i]] == '-'){
                trainDelay[i] = 1;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
    bool crashed[10] = {};
    bool waiting[10] = {};
    
    
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 0 || crashed[i]) 
            continue;
        
        for (int j = i + 1; j < maxTrains; j++) {
            if (trainActive[j] == 0 || crashed[j]) 
                continue;
            
            if (plannedX[i] == plannedX[j] && plannedY[i] == plannedY[j]) {
                // Calculate Manhattan distance for train i
                int dxi = destX[i] - trainX[i];
                int dyi = destY[i] - trainY[i];
                if (dxi < 0) dxi = -dxi;
                if (dyi < 0) dyi = -dyi;
                int distI = dxi + dyi;
                
                // Calculate Manhattan distance for train j
                int dxj = destX[j] - trainX[j];
                int dyj = destY[j] - trainY[j];
                if (dxj < 0) dxj = -dxj;
                if (dyj < 0) dyj = -dyj;
                int distJ = dxj + dyj;
                
                if (distI > distJ) {
                    
                    waiting[j] = true;
                } else if (distJ > distI) {
                    
                    waiting[i] = true;
                } else { 
                    crashed[i] = true;
                    crashed[j] = true;
                }
            }
            if (plannedX[i] == trainX[j] && plannedY[i] == trainY[j] &&
                plannedX[j] == trainX[i] && plannedY[j] == trainY[i]) {
                // Calculate Manhattan distance for train i
                int dxi = destX[i] - trainX[i];
                int dyi = destY[i] - trainY[i];
                if (dxi < 0) dxi = -dxi;
                if (dyi < 0) dyi = -dyi;
                int distI = dxi + dyi;
                
                // Calculate Manhattan distance for train j
                int dxj = destX[j] - trainX[j];
                int dyj = destY[j] - trainY[j];
                if (dxj < 0) dxj = -dxj;
                if (dyj < 0) dyj = -dyj;
                int distJ = dxj + dyj;
                
                if (distI > distJ) {
                    
                    waiting[j] = true;
                } else if (distJ > distI) {
                    
                    waiting[i] = true;
                } else {

                    crashed[i] = true;
                    crashed[j] = true;
                }
            }
        }
    }
    
    
    for (int i = 0; i < maxTrains; i++) {
        if (waiting[i] && !crashed[i]) {
            plannedX[i] = trainX[i];
            plannedY[i] = trainY[i];
            plannedDirection[i] = trainDirection[i];
        }
    }
    
    
    for (int i = 0; i < maxTrains; i++) {
        if (crashed[i]) {
            trainActive[i] = 0;
            numCrashed++;
        }
    }
}
// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            // Check if train reached its SPECIFIC destination
            if (trainX[i] == destX[i] && trainY[i] == destY[i]) {
                trainActive[i] = 0;
                numArrived++;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply halt to trains in the active zone.
// ----------------------------------------------------------------------------
void applyEmergencyHalt() {
}

// ----------------------------------------------------------------------------
// UPDATE EMERGENCY HALT
// ----------------------------------------------------------------------------
// Decrement timer and disable when done.
// ----------------------------------------------------------------------------
void updateEmergencyHalt() {
}
