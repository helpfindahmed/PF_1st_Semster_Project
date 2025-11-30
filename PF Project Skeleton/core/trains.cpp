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



// void spawnTrainsForTick() {
//     for (int i = 0 ; i < maxTrains ; i++)
//     {
//         if(trainActive[i]==0)
//         {
//             if(trainSpawnTick[i]== tick )
//             {
//                 trainActive[i]=1;
//                 trainprevX[i] = trainX[i];
//                 trainprevY[i] = trainY[i];
//             }
//         }
//     }

    
// }


void spawnTrainsForTick() {
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 0) {  // Train not yet active
            if (trainSpawnTick[i] == tick) {
                trainActive[i] = 1;
                // Initialize previous position to current position
                trainprevX[i] = trainX[i];
                trainprevY[i] = trainY[i];
                
                // ADD DEBUG:
                std::cout << "*** SPAWNING Train " << i << " at tick " << tick << std::endl;
            }
        }
    }
}
// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------




// bool determineNextPosition(int trainIndex) {
//     int currentX = trainX[trainIndex];
//     int currentY = trainY[trainIndex];
//     int currentDir = trainDirection[trainIndex];
    
//     std::cout << "DEBUG determineNextPosition: Train " << trainIndex 
//               << " at (" << currentX << "," << currentY << ")" << std::endl;
    
//     // Calculate next position based on current direction
//     int nextX = currentX;
//     int nextY = currentY;
    
//     if (currentDir == Dirnorth) nextX--;
//     else if (currentDir == Dirsouth) nextX++;
//     else if (currentDir == Dirwest) nextY--;
//     else if (currentDir == Direast) nextY++;
    
//     std::cout << "DEBUG: Next position would be (" << nextX << "," << nextY << ")" << std::endl;
    
    // // Check if next position is a valid track tile using the grid function
    // if (!isTrackTile(nextX, nextY)) {
    //     std::cout << "DEBUG: Invalid track at (" << nextX << "," << nextY << ")" << std::endl;
    //     // Invalid tile or out of bounds - train crashes
    //     trainActive[trainIndex] = 0;
    //     numCrashed++;
    //     std::cout << "Train " << trainIndex << " crashed at (" << nextX << "," << nextY << ")" << std::endl;
    //     return false;
    // }
    // // Get the tile at next position
    // char nextTile = grid[nextX * cols + nextY];
    // int newDir = currentDir;
    // if (nextTile == '+') {
    //     newDir = getSmartDirectionAtCrossing(trainIndex);
    // } 
    
    // else if (nextTile == '/' || nextTile == '\\') {
    //     newDir = getNextDirection(nextX, nextY, currentDir);
    // }
    
    // else if (nextTile >= 'A' && nextTile <= 'Z') {
    //     newDir = getNextDirection(nextX, nextY, currentDir);
    // }
    
    // else if (nextTile == '-' || nextTile == '|' || nextTile == '=') {
    //     newDir = getNextDirection(nextX, nextY, currentDir);
    // }

    // else if (nextTile == 'S' || nextTile == 'D') {
    //     newDir = currentDir;
    // }
    // // Store planned position

    // plannedX[trainIndex] = nextX;
    // plannedY[trainIndex] = nextY;
    // plannedDirection[trainIndex] = newDir;
    
    // return true;
  
// }


bool determineNextPosition(int trainIndex) {
    int currentX = trainX[trainIndex];
    int currentY = trainY[trainIndex];
    int currentDir = trainDirection[trainIndex];
    
int newDir = currentDir;
    // FIRST: Check if current tile is a crossing and update direction
    char currentTile = grid[currentY * cols + currentX];
    if (currentTile == '+') {
        currentDir = getSmartDirectionAtCrossing(trainIndex);
        // Debug
        if (tick < 30) {
            std::cout << "Train " << trainIndex << " at crossing (" << currentX << "," << currentY 
                     << ") chose direction " << currentDir << std::endl;
        }
    }


    // Debug for first train in first few ticks
    if (trainIndex == 0 && tick < 5) {
        std::cout << "  Train 0: current=(" << currentX << "," << currentY 
                  << ") dir=" << currentDir;
    }
    
    // Calculate next position based on current direction
    int nextX = currentX;
    int nextY = currentY;
    
    if (currentDir == Dirnorth) nextY--;
    else if (currentDir == Dirsouth) nextY++;
    else if (currentDir == Dirwest) nextX--;
    else if (currentDir == Direast) nextX++;
    
    if (trainIndex == 0 && tick < 5) {
        std::cout << " -> next=(" << nextX << "," << nextY << ")" << std::endl;
    }
    
        // Check if next position is a valid track tile using the grid function
    if (!isTrackTile(nextX, nextY)) {
        std::cout << "DEBUG: Invalid track at (" << nextX << "," << nextY << ")" << std::endl;
        // Invalid tile or out of bounds - train crashes
        trainActive[trainIndex] = 0;
        numCrashed++;
        std::cout << "Train " << trainIndex << " crashed at (" << nextX << "," << nextY << ")" << std::endl;
        return false;
    }
    // Get the tile at next position
    char nextTile = grid[nextY * cols + nextX];

    
   
    
    if (nextTile == '/' || nextTile == '\\') {
        newDir = getNextDirection(nextX, nextY, currentDir);
    }
    
    else if (nextTile >= 'A' && nextTile <= 'Z') {
        newDir = getNextDirection(nextX, nextY, currentDir);
    }
    
    else if (nextTile == '-' || nextTile == '|' || nextTile == '=') {
        newDir = getNextDirection(nextX, nextY, currentDir);
    }

    else if (nextTile == 'S' || nextTile == 'D') {
        newDir = currentDir;
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



// void determineAllRoutes() {
//     for (int i = 0; i < maxTrains; i++) {
//         if (trainActive[i] == 1) {
//             determineNextPosition(i);
//         }
//     }
// }

// // ----------------------------------------------------------------------------
// // MOVE ALL TRAINS (PHASE 5)
// // ----------------------------------------------------------------------------
// // Move trains; resolve collisions and apply effects.
// // ----------------------------------------------------------------------------
// void moveAllTrains() {
//      for (int i = 0; i < maxTrains; i++) {
//         if (trainActive[i] == 1) {
//             trainprevX[i] = trainX[i];
//             trainprevY[i] = trainY[i];
//         }
//     }
    
//     // Move all trains to their planned positions
//     for (int i = 0; i < maxTrains; i++) {
//         if (trainActive[i] == 1) {
//             trainX[i] = plannedX[i];
//             trainY[i] = plannedY[i];
//             trainDirection[i] = plannedDirection[i];
//         }
//     }
// }


void determineAllRoutes() {
    if (tick < 3) {
        std::cout << "  determineAllRoutes() called" << std::endl;
    }
    
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            bool success = determineNextPosition(i);
            if (tick < 3) {
                std::cout << "    Train " << i << " route: " << (success ? "OK" : "FAILED") 
                         << " planned=(" << plannedX[i] << "," << plannedY[i] << ")" << std::endl;
            }
        }
    }
}

void moveAllTrains() {
    if (tick < 3) {
        std::cout << "  moveAllTrains() called" << std::endl;
    }
    
    // Store previous positions
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            trainprevX[i] = trainX[i];
            trainprevY[i] = trainY[i];
            if (tick < 3) {
                std::cout << "    Train " << i << " prev saved: (" << trainprevX[i] << "," << trainprevY[i] << ")" << std::endl;
            }
        }
    }
    
    // Move all trains to their planned positions
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            if (tick < 3) {
                std::cout << "    Train " << i << " moving from (" << trainX[i] << "," << trainY[i] 
                         << ") to (" << plannedX[i] << "," << plannedY[i] << ")" << std::endl;
            }
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
    bool crashed[10] = {};
    bool waiting[10] = {};
    
    
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 0 || crashed[i]) continue;
        
        for (int j = i + 1; j < maxTrains; j++) {
            if (trainActive[j] == 0 || crashed[j]) continue;
            
            
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



// void checkArrivals() {
//     for (int i = 0; i < maxTrains; i++) {
//         if (trainActive[i] == 1) {
//             // Check if train reached its SPECIFIC destination
//             if (trainX[i] == destX[i] && trainY[i] == destY[i]) {
//                 trainActive[i] = 0;
//                 numArrived++;
//             }
//         }
//     }
// }


void checkArrivals() {
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            // Check if train reached its SPECIFIC destination
            if (trainX[i] == destX[i] && trainY[i] == destY[i]) {
                trainActive[i] = 0;
                numArrived++;
                std::cout << "*** Train " << i << " ARRIVED at destination (" 
                         << destX[i] << "," << destY[i] << ")! ***" << std::endl;
            }
            
            // Debug: Show distance to destination for first few ticks
            if (tick < 5) {
                std::cout << "  Train " << i << " at (" << trainX[i] << "," << trainY[i] 
                         << ") -> dest (" << destX[i] << "," << destY[i] << ")" << std::endl;
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
