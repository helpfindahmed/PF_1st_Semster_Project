#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"

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
            return;
        if (isSwitchTile(trainprevX[i], trainprevY[i])){
            int conversion = trainX[i] * cols + trainY[i];
            int switchnum = getSwitchIndex(grid[conversion]);
            if (switchnum == -1)
                return;

            int entryDir = -1; 
            if (trainX[i] > trainprevX[i]) entryDir = Direast;
            else if (trainX[i] < trainprevX[i]) entryDir = Dirwest;
            else if (trainY[i] > trainprevY[i]) entryDir = Dirsouth;
            else if (trainY[i] < trainprevY[i]) entryDir = Dirnorth;

            if (switchMode[switchnum] == 0 ){// global switch 
                switchEntryCount[switchnum][0]++; //because rest are for readibility
            }else if(switchMode[switchnum] == 2  ){// Per Dir switch.
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
    for (int i = 0; i < maxSwitch; i++) {
        bool needsFlip = false;
        
        // 1. Check GLOBAL Mode (Mode 0)
        if (switchMode[i] == 0) { 
            if (switchEntryCount[i][0] == switchK[i][0]) { //only 0th index bcz rest are for readibility 
                needsFlip = true;
            }

            if (needsFlip) {
                // Reset the master counter to 0
                switchEntryCount[i][0] = 0;
            }
            
        } 
        // 2. Check PER DIR Mode (Mode 2)
        else if (switchMode[i] == 2) { 
            // Check all four direction counters against their specific K-limits
            for (int j = 0; j < 4; j++) {
                if (switchEntryCount[i][j] == switchK[i][j]) {
                    needsFlip = true;
                    break; // one time check only because it needs only one counter to trigger the flip
                }
            }

            if (needsFlip) {
                //Counters reset on flip -> reset all 4 counters
                for (int j = 0; j < 4; ++j) {
                    switchEntryCount[i][j] = 0;
                }
            }
        }
        if (needsFlip) {
            switchFlipQueued[i] = true;
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
     for (int i = 0; i < maxSwitch; i++) {
        if (switchFlipQueued[i]) {

            //switchState[i] = 1 - switchState[i];
            if (switchState[i] == 1){
                switchState[i] = 0;
            }else{
                switchState[i] = 1;
            }
            switchFlipQueued[i] = false;
            //logSwitchState(i); no need for this here since we have to log in every single tick not specifically when switch occurs 
        }
    }
}

// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights() {
}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState(int x , int y) {
    int switchitr = getSwitchIndex(grid[(x * cols) + y]); 
    if (switchitr != -1 && switchitr < maxSwitches) {
        // Toggle the state
        if (switchState[switchitr] == 1){
                switchState[switchitr] = 0;
        }else{
                switchState[switchitr] = 1;
        }
    
        // Reset counters 
        // This resets all 4 counters for consistency.
        for (int j = 0; j < 4; ++j) {
            switchEntryCount[switchitr][j] = 0;
        }
    }
}
// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// -----------------------PF_1st_Semster_Project-----------------------------------------------------
int getSwitchStateForDirection(int switchIndex) {
    if (switchIndex >= 0 && switchIndex < maxSwitches) {
        if (switchState[switchIndex] == 0) {
            return switchState0[switchIndex];
        } else {
            return switchState1[switchIndex];
        }
    }
    return 0; 
}
