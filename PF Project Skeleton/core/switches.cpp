#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"
#include <cstdlib>
#include <cmath>

// ============================================================================
// SWITCHES.CPP - Switch management
// ============================================================================
const int Dirnorth = 0;
const int Direast = 1;
const int Dirsouth = 2;
const int Dirwest = 3;

// ----------------------------------------------------------------------------
// UPDATE SWITCH COUNTERS
// ----------------------------------------------------------------------------
// Increment counters for trains entering switches.
// ----------------------------------------------------------------------------
void updateSwitchCounters() {
    for (int i = 0; i < maxTrains; i++){
        if (trainActive[i] == 0)
            continue;
            
        // Check if train actually moved this tick (or just spawned)
        // We only want to count the entry ONCE.
        bool moved = (trainX[i] != trainprevX[i]) || (trainY[i] != trainprevY[i]) || (trainSpawnTick[i] == tick);

        if (moved && isSwitchTile(trainX[i], trainY[i])){
            int conversion = trainY[i] * cols + trainX[i];
            int switchnum = getSwitchIndex(grid[conversion]);
            
            if (switchnum == -1)
                continue;

            // Determine direction of travel
            int entryDir = -1; 
            // If just spawned, use current facing direction
            if (trainSpawnTick[i] == tick) {
                entryDir = trainDirection[i];
            } else {
                if (trainX[i] > trainprevX[i]) entryDir = Direast;
                else if (trainX[i] < trainprevX[i]) entryDir = Dirwest;
                else if (trainY[i] > trainprevY[i]) entryDir = Dirsouth;
                else if (trainY[i] < trainprevY[i]) entryDir = Dirnorth;
                else entryDir = trainDirection[i]; 
            }

            // Update appropriate counter
            if (switchMode[switchnum] == 0 ){// global switch 
                switchEntryCount[switchnum][0]++; 
            } else if(switchMode[switchnum] == 2 ){// Per Dir switch.
                switchEntryCount[switchnum][entryDir]++;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// QUEUE SWITCH FLIPS
// ----------------------------------------------------------------------------
// Queue flips when counters hit K.
// ----------------------------------------------------------------------------
void queueSwitchFlips() {
    // Loop through all defined switches
    for (int i = 0; i < maxSwitches; i++) {
        bool needsFlip = false;
        
        // 1. Check GLOBAL Mode (Mode 0)
        if (switchMode[i] == 0) { 
            if (switchEntryCount[i][0] >= switchK[i][0]) { 
                needsFlip = true;
            }
        } 
        // 2. Check PER DIR Mode (Mode 2)
        else if (switchMode[i] == 2) { 
            // Check all four direction counters against their specific K-limits
            for (int j = 0; j < 4; j++) {
                if (switchK[i][j] > 0 && switchEntryCount[i][j] >= switchK[i][j]) {
                    needsFlip = true;
                    break; 
                }
            }
        }

        if (needsFlip) {
            switchFlipQueued[i] = true;
            
            // Reset ALL counters for this switch immediately upon queuing
            for (int j = 0; j < 4; ++j) {
                switchEntryCount[i][j] = 0;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
     for (int i = 0; i < maxSwitches; i++) {
        if (switchFlipQueued[i]) {
            
            bool isOccupied = false;
            for (int t = 0; t < maxTrains; t++) {
                if (trainActive[t] && trainX[t] == switchX[i] && trainY[t] == switchY[i]) {
                    isOccupied = true;
                    break;
                }
            }

            if (!isOccupied) {
                switchState[i] = 1 - switchState[i];
                switchFlipQueued[i] = false;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights() {
    for (int i = 0; i < maxSwitches; i++) {
        
        // Default to Green (0)
        signalColor[i] = 0; 

        if (switchX[i] == -1) //switch DNE
            continue;

        int minDist = 1000;

        // Find the nearest ACTIVE train
        for (int a = 0; a < maxTrains; a++) {
            if (trainActive[a]) {
                int dist = abs(trainX[a] - switchX[i]) + abs(trainY[a] - switchY[i]);
                if (dist < minDist) {
                    minDist = dist;
                }
            }
        }
        
        // Set color based on distance
        if (minDist == 0) {      // On top 
            signalColor[i] = 0;  // Green
        } 
        else if (minDist == 1) { // 1 tile away
            signalColor[i] = 2;  // Red
        } 
        else if (minDist == 2) { // 2 tiles away
            signalColor[i] = 1;  // Yellow
        }  
        else {
            signalColor[i] = 0;  // Green
        }
    }
}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState(int x , int y) {
    if (x < 0 || x >= cols || y < 0 || y >= rows) return;
    int switchitr = getSwitchIndex(grid[(y * cols) + x]); 
    if (switchitr != -1 && switchitr < maxSwitches) {
        switchState[switchitr] = 1 - switchState[switchitr];
        for (int j = 0; j < 4; ++j) {
            switchEntryCount[switchitr][j] = 0;
        }
    }
}

// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// ----------------------------------------------------------------------------
int getSwitchStateForDirection(int switchIndex) {
    if (switchIndex >= 0 && switchIndex < maxSwitches) {
        // STANDARD LOGIC:
        // switchState 0 = switchState0 (East/Straight)
        // switchState 1 = switchState1 (West/Turn)
        if (switchState[switchIndex] == 0) {
            return switchState1[switchIndex]; 
        } else {
            return switchState0[switchIndex]; 
        }
    }
    return 0; 
}