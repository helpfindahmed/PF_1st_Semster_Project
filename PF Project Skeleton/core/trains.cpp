#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>

using namespace std;

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

    for (int i = 0 ; i < maxTrains ; i ++)
    {
        if(trainActive[i]==0)
        {
            if(trainSpawnTick[i]== tick )
            {
                trainActive[i]=1;
            }
        }
    }

    
}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition(int trainIndex) {
  int currentX = trainX[trainIndex];
    int currentY = trainY[trainIndex];
    int currentDir = trainDirection[trainIndex];
    
    // Calculate next position based on current direction
    int nextX = currentX;
    int nextY = currentY;
    
    if (currentDir == Dirnorth) nextX--;
    else if (currentDir == Dirsouth) nextX++;
    else if (currentDir == Dirwest) nextY--;
    else if (currentDir == Direast) nextY++;
    
    // Check if next position is out of bounds   train will crash
    if (!isInBounds(nextX, nextY)) {
        trainActive[trainIndex] = 0;
        numCrashed++;
        return false;
    }
    
    // Get the tile at next position
    char nextTile = grid[nextX * cols + nextY];
    
   
    int newDir = currentDir;
    
   
    if (nextTile == '+') {
        newDir = getSmartDirectionAtCrossing(trainIndex);
    } 
    
    else if (nextTile == '/' || nextTile == '\\') {
        newDir = getNextDirection(nextX, nextY, currentDir);
    }
    
    else if (nextTile >= 'A' && nextTile <= 'Z') {
        newDir = getNextDirection(nextX, nextY, currentDir);
    }
    // For straight tracks (-, |, =) and spawn/destination (S, D) maintain direction
    else if (nextTile == '-' || nextTile == '|' || nextTile == '=') {
        newDir = getNextDirection(nextX, nextY, currentDir);
    }
    // Invalid tile - train crashes
    else {
        trainActive[trainIndex] = 0;
        numCrashed++;
        return false;
    }
    
    // Store planned position

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
    
    char tile = grid[x * cols + y];
    
    // Straight tracks maintain direction
    if (tile == '=' && (currentDir == Dirwest || currentDir == Direast)) {
        return currentDir;
    }
    if (tile == '|' && (currentDir == Dirsouth|| currentDir == Dirnorth)) {
        return currentDir;
    }
    
    // Curves change direction
    if (tile == '/') {
        if (currentDir == Direast) return Dirsouth;
        if (currentDir == Dirwest) return Dirnorth;
        if (currentDir == Dirnorth) return Dirwest;
        if (currentDir == Dirsouth) return Direast;
    }
    if (tile == '\\') {
        if (currentDir == Direast) return Dirsouth;
        if (currentDir == Dirwest) return Dirnorth;
        if (currentDir == Dirnorth) return Dirwest;
        if (currentDir == Dirsouth) return Direast;
    }
    

    
    // Crossing maintains direction   smart routing handling
    if (tile == '+') {
        return currentDir;
    }
    
    
    if (tile >= 'A' && tile <= 'Z') {
        int switchIndex = getSwitchIndex(tile);
        if (switchIndex >= 0 && switchIndex < maxSwitches) {
            int state = switchState[switchIndex];
           
            return state;
        }
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
        if (differx > 0) return Dirsouth;  // Move down (increasing x)
        else return Dirnorth;           // Move up (decreasing x)
    } else {
        if (differy > 0) return Direast; // Move right (increasing y)
        else return Dirwest;         // Move left (decreasing y)
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
            determineNextPosition(i);
        }
    }
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
     for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            trainprevX[i] = trainX[i];
            trainprevY[i] = trainY[i];
        }
    }
    
    // Move all trains to their planned positions
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            trainX[i] = plannedX[i];
            trainY[i] = plannedY[i];
            trainDirection[i] = plannedDirection[i];
        }
    }
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
}

// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
      for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            // Check if train reached its destination
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
