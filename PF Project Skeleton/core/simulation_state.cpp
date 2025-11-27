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
char trainColor[10] = {};
int trainprevX[10] = {};
int trainprevY[10] = {};
// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------
int maxSwitches = 0; 
int switchIndex[26] = {}; //get from the getSwitchIndex()
int switchMode[20] = {}; // 1 = PER_DIR, 2 = GLOBAL
int switchState[20] = {}; //1 = UP, 2 = DOWN, 3 = LEFT, 4 = RIGHT


int switchUp[20] = {};
int switchRight[20] = {};
int switchDown[20] = {};
int switchLeft[20] = {};


int switchX[20] = {};
int switchY[20] = {};


int switchState0[20] = {}; //1 = UP, 2 = DOWN, 3 = LEFT, 4 = RIGHT
int switchState1[20] = {}; //1 = UP, 2 = DOWN, 3 = LEFT, 4 = RIGHT


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

void initializeSimulationState() {
    rows = -1;
    cols = -1;
    tick = 0;
    numArrived = 0;
    numCrashed = 0;
    isEmergencyHalt = false;

    for (int i = 0; i < maxTrains; ++i) {
        trainSpawnTick[i] = -1;
        trainX[i] = -1;
        trainY[i] = -1;
        trainprevX[i] = -1;
        trainprevY[i] = -1;
        trainDirection[i] = -1;
        trainActive[i] = -1;
        spawnX[i] = -1;
        spawnY[i] = -1;
        destX[i] = -1;
        destY[i] = -1;
    }

    for (int i = 0; i < maxSwitches; ++i) {
        switchIndex[i] = -1;
        switchMode[i] = -1;
        switchState[i] = -1;
        switchUp[i] = -1;
        switchRight[i] = -1;
        switchDown[i] = -1;
        switchLeft[i] = -1;
        switchX[i] = -1;
        switchY[i] = -1;
        switchState0[i] = -1;
        switchState1[i] = -1;
    }

    if (grid) { //since grid was DMA we need to delete to ensure no memory leakage so deleting process here
        delete[] grid;
        grid = nullptr; //make sure grid doesnt become a dangling pointer
    }
    // rows and cols will be set when you load the level file
}
