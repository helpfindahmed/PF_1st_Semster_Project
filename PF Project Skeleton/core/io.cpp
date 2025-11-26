#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

// ============================================================================
// IO.CPP - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LOAD LEVEL FILE
// ----------------------------------------------------------------------------
// Load a .lvl file into global state.
// ----------------------------------------------------------------------------
bool loadLevelFile(const char* filename) {
    initializeSimulationState(); 
    ifstream file (filename);
    if (!file.is_open()){
        cout << "Error: could not load the file " << filename << endl; 
        return false;
    }else{
        //read all data from file and put into variables.
    }
    
    cout << "Level '" << filename << "' loaded successfully. Grid size: " << cols << "x" << rows << endl;
    return true;
}

// ----------------------------------------------------------------------------
// INITIALIZE LOG FILES
// ----------------------------------------------------------------------------
// Create/clear CSV logs with headers.
// ----------------------------------------------------------------------------
void initializeLogFiles() {

    ofstream trace_log("trace.csv");
    if (trace_log.is_open()) {
        trace_log << "TICK,TRAIN_ID,X,Y,DIRECTION,STATE\n";
        trace_log.close();
    }

    
    ofstream switches_log("switches.csv");
    if (switches_log.is_open()) {
        switches_log << "TICK,SWITCH_ID,X,Y,STATE\n";
        switches_log.close();
    }
    
    
    ofstream signals_log("signals.csv");
    if (signals_log.is_open()) {
        signals_log << "TICK,SWITCH_ID,COLOR\n";
        signals_log.close();
    }
}
// ----------------------------------------------------------------------------
// LOG TRAIN TRACE
// ----------------------------------------------------------------------------
// Append tick, train id, position, direction, state to trace.csv.
// ----------------------------------------------------------------------------
void logTrainTrace(int trainId) {
    ofstream trace_log("trace.csv", ios::app); 
    if (!trace_log.is_open()) {
        cout << "Error:File not opened successfully!" << endl;
        return;
    }else{
         trace_log << tick << ","
                  << trainId<< ","
                  << trainX[trainId] << ","
                  << trainY[trainId] << ","
                  << trainDirection[trainId] << ","
                  << trainActive[trainId] << "\n";
        trace_log.close();
    }
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchState(int switchIndex) {
    ofstream switches_log("switches.csv", ios::app); 
    if (!switches_log.is_open()) {
        cout << "Error:File not opened successfully!" << endl;
        return;
    }else{
         switches_log << tick << ","
                     << switchIndex << ","
                     << switchX[switchIndex] << ","
                     << switchY[switchIndex] << ","
                     << switchState[switchIndex] << "\n";
        switches_log.close();
    }
}

// ----------------------------------------------------------------------------
// LOG SIGNAL STATE
// ----------------------------------------------------------------------------
// Append tick, switch id, signal color to signals.csv.
// ----------------------------------------------------------------------------
void logSignalState(int switchIndex , char signalColor) {

    ofstream signals_log("signals.csv", ios::app); 
    // 3. WRITING: Write the data if the file is open
    if (!signals_log.is_open()) {
        cout << "Error:File not opened successfully!" << endl;
        return;
    }else {
        signals_log << tick << "," << switchIndex << "," << signalColor << "\n";
        signals_log.close();
    }
    
}

// ----------------------------------------------------------------------------
// WRITE FINAL METRICS
// ----------------------------------------------------------------------------
// Write summary metrics to metrics.txt.
// ----------------------------------------------------------------------------
void writeMetrics() {
    ofstream metrics_file("metrics.txt");
    if (!metrics_file.is_open()) {
        cout << "Error:File not opened successfully." << endl;
        return;
    }else {
        metrics_file << "000000-Simulation Metrics-000000\n";
        metrics_file << "Total Ticks: " << tick << "\n";
        metrics_file << "Trains Arrived: " << numArrived << "\n";
        metrics_file << "Trains Crashed: " << numCrashed << "\n";
        metrics_file.close();
    }
}
