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
const int Global =0;

// ----------------------------------------------------------------------------
// UPDATE SWITCH COUNTERS
// ----------------------------------------------------------------------------
// Increment counters for trains entering switches.
// ----------------------------------------------------------------------------
void updateSwitchCounters(int currentx, int currenty , int prevx, int prevy) {

    // if train is active i dont know if you have already made active function or not 
    if(isSwitchTile(currentx , currenty)&&isSwitchTile(prevx , prevy)  )
    {
        int conversion = currenty * cols + currentx;
        int switchnum= getSwitchIndex(grid[conversion]);
        if (switchnum == -1) return;

          int entryDir = -1; 
        if (currentx > prevx) entryDir = Direast;
        else if (currentx < prevx) entryDir = Dirwest;
        else if (currenty > prevy) entryDir = Dirsouth;
        else if (currenty < prevy) entryDir = Dirnorth;

        if (switchMode[switchnum] == 2  )// global switch .
            {
                switchEntryCount[switchnum][Global]++;
            }
            else if(switchMode[switchnum] == 0  )// Per Dir switch .
            {
                switchEntryCount[switchnum][entryDir]++;
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
    for (int i = 0; i < maxSwitches; ++i) {
        bool needsFlip = false;
        
        // 1. Check GLOBAL Mode (Mode 2)
        if (switchMode[i] == 2) { 
          
            if (switchEntryCount[i][Global] >= switchK[i][Global]) {
                needsFlip = true;
            }
            
            if (needsFlip) {
                // Reset the master counter to 0
                switchEntryCount[i][Global] = 0;
            }
            
        } 
        // 2. Check PER DIR Mode (Mode 1)
        else if (switchMode[i] == 1) { 
            // Check all four direction counters against their specific K-limits
            for (int j = 0; j < 4; ++j) {
                if (switchEntryCount[i][j] >= switchK[i][j]) {
                    needsFlip = true;
                    break; // One time check because needs only one counter to trigger the flip
                }
            }

            if (needsFlip) {
                // Rule (PDF): Counters reset on flip -> reset all 4 counters
                for (int j = 0; j < 4; ++j) {
                    switchEntryCount[i][j] = 0;
                }
            }
        }
        
        
        if (needsFlip) {
            switchFlipQueued[i] = true;// need to be declared in the simulation sate file
        }
    }
}

// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
     for (int i = 0; i < maxSwitches; ++i) {
        if (switchFlipQueued[i]) {
           
            switchState[i] = 1 - switchState[i]; 
            
            
            switchFlipQueued[i] = false;

            
            logSwitchState(i); 
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

       int switchitr = getSwitchIndex(grid[(y * cols) + x]); 
    
    if (switchitr != -1 && switchitr < maxSwitches) {
        // Toggle the state: 0 <-> 1
        switchState[switchitr] = 1 - switchState[switchitr];
        
        // Reset counters (since manual input usually overrides automation)
        // This resets all 4 counters for consistency.
        for (int j = 0; j < 4; ++j) {
            switchEntryCount[switchitr][j] = 0;
        }
        
        // Log the manual state change
        logSwitchState(switchitr); 
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
