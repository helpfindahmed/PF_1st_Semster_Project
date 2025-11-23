#include "simulation_state.h"
#include <cstring>
using namespace std;

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ----------------------------------------------------------------------------
// GRID
// ----------------------------------------------------------------------------
int rows = 0;
int cols = 0;
char* grid = nullptr;
// ----------------------------------------------------------------------------
// TRAINS
// ----------------------------------------------------------------------------
int maxTrains = 10;
int trainX[10] = {0};
int trainY[10] = {0};
int trainDirection[10] = {0};
int trainActive[10] = {0};
// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------
int maxSwitches = 20;
int switchX[20] = {0};
int switchY[20] = {0};
int switchState[20] = {0};
// ----------------------------------------------------------------------------
// SPAWN AND DESTINATION POINTS
// ----------------------------------------------------------------------------
int spawnX[10] = {0};
int spawnY[10] = {0};
int destX[10] = {0};
int destY[10] = {0};
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
    rows = 0;
    cols = 0;
    tick = 0;
    numArrived = 0;
    numCrashed = 0;
    isEmergencyHalt = false;

    for (int i = 0; i < maxTrains; ++i) {
        trainX[i] = 0;
        trainY[i] = 0;
        trainDirection[i] = 0;
        trainActive[i] = 0;
        spawnX[i] = 0;
        spawnY[i] = 0;
        destX[i] = 0;
        destY[i] = 0;
    }

    for (int i = 0; i < maxSwitches; ++i) {
        switchX[i] = 0;
        switchY[i] = 0;
        switchState[i] = 0;
    }

    if (grid) { //since grid was DMA we need to delete to ensure no memory leakage so deleting process here
        delete[] grid;
        grid = nullptr; //make sure grid doesnt become a dangling pointer
    }
    // rows and cols will be set when you load the level file
}
