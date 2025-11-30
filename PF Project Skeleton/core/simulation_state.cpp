#include "simulation_state.h"
#include <cstring>
using namespace std;

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ----------------------------------------------------------------------------
// GRID
// ----------------------------------------------------------------------------
int rows = 0; //will also be later defined in some other file
int cols = 0; //will also be later defined in some other file
char* grid = nullptr; //will be later defined in some other file and will become 2d array since char* grid = char[rows*cols]
// ----------------------------------------------------------------------------
// TRAINS
// ----------------------------------------------------------------------------
int maxTrains = 0; 
int trainSpawnTick[10] = {};
int trainX[10] = {};
int trainY[10] = {};
int trainDirection[10] = {}; 
int trainActive[10] = {}; //can only be 0 or 1
int trainColor[10] = {};
int trainprevX[10] = {};
int trainprevY[10] = {};
int trainDelay[10] = {};
int totalActiveTrains = 0;
// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------
const int maxSwitches = 26; 
int switchIndex[26] = {}; //get from the getSwitchIndex()
int switchMode[26] = {}; //  0 = GLOBAL, 2 = PER_DIR
int switchState[26] = {}; 
int switchEntryCount [26][4];
int signalColor[26] = {}; //0 = GREEN, 1 = YELLOW, 2 = RED
// int switchUp[26] = {};
// int switchRight[26] = {};
// int switchDown[26] = {};
// int switchLeft[26] = {};
int switchK[26][4]; //north, east, south, west
int switchFlipQueued[26];

int switchX[26] = {};
int switchY[26] = {};


int switchState0[26] = {}; //1 = north, 2 = east, 3 = south, 4 = west
int switchState1[26] = {}; 


// ----------------------------------------------------------------------------
// SPAWN AND DESTINATION POINTS
// ----------------------------------------------------------------------------
int spawnX[10] = {};
int spawnY[10] = {};
int destX[10] = {};
int destY[10] = {};
// ----------------------------------------------------------------------------
// SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
int tick = 0;
// ----------------------------------------------------------------------------
// METRICS
// ----------------------------------------------------------------------------
int numArrived = 0;
int numCrashed = 0;
// ----------------------------------------------------------------------------
// EMERGENCY HALT
// ----------------------------------------------------------------------------
bool isEmergencyHalt = false;

// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
// ----------------------------------------------------------------------------
// Resets all global simulation state.
// ----------------------------------------------------------------------------
// Called before loading a new level.
// ----------------------------------------------------------------------------
int weatherCondition = 0;
void initializeSimulationState() {
    rows = -1;
    cols = -1;
    tick = 0;
    numArrived = 0;
    numCrashed = 0;
    isEmergencyHalt = false;
    weatherCondition = 0;
    totalActiveTrains = 0;
    for (int i = 0; i < 10; ++i) {
        trainSpawnTick[i] = -1;
        trainX[i] = -1;
        trainY[i] = -1;
        trainprevX[i] = -1;
        trainprevY[i] = -1;
        trainDirection[i] = -1;
        trainActive[i] = 0;
        trainDelay[i] = 0;
        spawnX[i] = -1;
        spawnY[i] = -1;
        destX[i] = -1;
        destY[i] = -1;
    }

    for (int i = 0; i < 26; ++i) {
        switchIndex[i] = -1;
        switchMode[i] = -1;
        switchState[i] = -1;
        // switchUp[i] = -1;
        // switchRight[i] = -1;
        // switchDown[i] = -1;
        // switchLeft[i] = -1;
        switchX[i] = -1;
        switchY[i] = -1;
        switchState0[i] = -1;
        switchState1[i] = -1;
        switchFlipQueued[i] = -1;
        signalColor[i] = -1;
       
    }

    for (int i = 0; i < 26; i++){
        for (int j = 0; j < 4; j++){
            switchK[i][j] = -1;
            switchEntryCount[i][j] = -1;
        }
    }

    if (grid) { //since grid was DMA we need to delete to ensure no memory leakage so deleting process here
        delete[] grid;
        grid = nullptr; //make sure grid doesnt become a dangling pointer
    }
    // rows and cols will be set when you load the level file
}
