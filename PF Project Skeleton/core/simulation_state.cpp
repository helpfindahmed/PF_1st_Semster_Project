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
int trains[10][6] = {}; //spawn tick, trainX, trainY, trainDirection, trainActive, color/trainId 
// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------
int maxSwitches = 0; 
int switches[20][11] = {}; //getSwitchIndex(), mode if 1 = PER_DIR 2 = GLOBAL, init, k_up, right, down, left, state0, state1
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

    //initialize everything to -1
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 6; j++){
        trains[i][j] = -1;
        }
        spawnX[i] = -1;
        spawnY[i] = -1;
        destX[i] = -1;
        destY[i] = -1;
    }

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 11; j++){
            switches[i][j] = -1; 
        }
    }

    if (grid) { //since grid was DMA we need to delete to ensure no memory leakage so deleting process here
        delete[] grid;
        grid = nullptr; //make sure grid doesnt become a dangling pointer
    }
    // rows and cols will be set when you load the level file
}
