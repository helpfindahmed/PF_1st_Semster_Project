#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include <cstdlib>
#include <ctime>

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================

// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------

void initializeSimulation(char* filename) {
    loadLevelFile(filename); //a pointer pointing to the first element's first character of C-string from command line
    initializeLogFiles();
}

// ----------------------------------------------------------------------------
// SIMULATE ONE TICK
// ----------------------------------------------------------------------------

void simulateOneTick() {
      // Phase 1: 
    spawnTrainsForTick();
    // Phase 2: 
    determineAllRoutes();
    // Phase 3: 
    updateSwitchCounters();  
    // Phase 4: 
    queueSwitchFlips();      
    // Phase 5: 
    detectCollisions();
    moveAllTrains();
    applyDeferredFlips();      
    // Phase 6: 
    checkArrivals();
    // Phase 7:        
    
    // Update logs
    for(int i = 0; i < 10; i++){
        if (trainActive[i])
            logTrainTrace(trainColor[i]);   
    }
    for (int i = 0; i < 26; i++){
        if (switchMode[i] != -1)
            logSwitchState(i);  
    }
    tick++;
}

// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------

bool isSimulationComplete() { //defined when all trains reach destination or collide
    if (numArrived + numCrashed == maxTrains)
        return true;
    else
        return false;


}