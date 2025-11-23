#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/io.h"
#include <iostream>
#include <string>
using namespace std;

// ============================================================================
// MAIN.CPP - Entry point of the application (NO CLASSES)
// ============================================================================

// ----------------------------------------------------------------------------
// MAIN ENTRY POINT
// ----------------------------------------------------------------------------
// This function is the main entry point of the application. It handles command
// line arguments to specify the level file to load, loads the level file using
// loadLevelFile, initializes the simulation system, initializes the SFML
// application window, prints control instructions to the console, runs the
// main application loop, cleans up resources, and prints final simulation
// statistics. Returns 0 on success, 1 on error (e.g., failed to load level
// file or initialize application).
// ----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    if (argc < 2){ //making sure user typed in at least 1 argument (excluding the program name)
        cout << "Error: missing filename \n";
        cout << "Usage: " << argv[0] << " <filename.lvl> \n";
        return 1;
    }
    initializeSimulation(argv[1]);
    initializeApp();
    
    runApp();
    
    cleanupApp();
    cout << "Application closed" << endl;

    return 0;
}
