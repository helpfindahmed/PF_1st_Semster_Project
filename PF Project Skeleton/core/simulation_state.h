#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H


// ============================================================================
// SIMULATION_STATE.H - Global constants and state
// ============================================================================
// Global constants and arrays used by the game.
// ============================================================================

// ----------------------------------------------------------------------------
// GRID CONSTANTS
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// TRAIN CONSTANTS
// ----------------------------------------------------------------------------
extern int maxTrains; 

// ----------------------------------------------------------------------------
// SWITCH CONSTANTS
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------
extern int weatherCondition; // 0=Normal, 1=Rain, 2=Fog

// ----------------------------------------------------------------------------
// SIGNAL CONSTANTS
// ----------------------------------------------------------------------------




// ----------------------------------------------------------------------------
// GLOBAL STATE: GRID
// ----------------------------------------------------------------------------
extern int rows; 
extern int cols; 
extern char* grid;

// ----------------------------------------------------------------------------
// GLOBAL STATE: TRAINS
// ----------------------------------------------------------------------------

extern int trainSpawnTick[10];
extern int trainX[10];
extern int trainY[10];
extern int trainDirection[10]; 
extern int trainActive[10];
extern int trainColor[10];
extern int trainprevX[10];
extern int trainprevY[10];
extern int trainDelay[10];
extern int totalActiveTrains;
// ----------------------------------------------------------------------------
// GLOBAL STATE: SWITCHES (A-Z mapped to 0-25)
// ----------------------------------------------------------------------------

extern const int maxSwitches; 
extern int switchIndex[26]; 
extern int switchMode[26];
extern int switchState[26];
extern int switchEntryCount [26][4];
extern int signalColor[26];

// extern int switchUp[20];
// extern int switchRight[20];
// extern int switchDown[20];
// extern int switchLeft[20];
extern int switchK[26][4];
extern int switchFlipQueued[26];
extern int switchX[26];
extern int switchY[26];

extern int switchState0[26];
extern int switchState1[26];

// ----------------------------------------------------------------------------
// GLOBAL STATE: SPAWN POINTS
// ----------------------------------------------------------------------------
extern int spawnX[10];
extern int spawnY[10];

// ----------------------------------------------------------------------------
// GLOBAL STATE: DESTINATION POINTS
// ----------------------------------------------------------------------------
extern int destX[10];
extern int destY[10];

// ----------------------------------------------------------------------------
// GLOBAL STATE: SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
extern int tick;

// ----------------------------------------------------------------------------
// GLOBAL STATE: METRICS
// ----------------------------------------------------------------------------
extern int numArrived;
extern int numCrashed;

// ----------------------------------------------------------------------------
// GLOBAL STATE: EMERGENCY HALT
// ----------------------------------------------------------------------------
extern bool isEmergencyHalt;

// ----------------------------------------------------------------------------
// INITIALIZATION FUNCTION
// ----------------------------------------------------------------------------
// Resets all state before loading a new level.
void initializeSimulationState();

#endif
