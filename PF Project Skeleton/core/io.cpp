#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cmath>
using namespace std;
int seed;

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
        string line;
            
        while (getline(file, line)) {
            if (line == "ROWS:")
                break;
        }
        file >> rows;
                
        while (getline(file, line)) {
            if (line == "COLS:")
                break;
        }
        file >> cols;
        
        while (getline(file, line)) {
            if (line == "SEED:")
                break;
        }
        file >> seed;
        
        while (getline(file, line)) {
            if (line == "WEATHER:")
                break;
        }
        file >> line;
        
        grid = new char[rows * cols];    
        for (int k = 0; k < 2 * cols; ++k) {
            grid[k] = ' ';
        }

        while (getline(file, line)) {
            if (line == "MAP:")
                break;
        }
     
        int current_row = 2;
        int current_index = 2 * cols; 
        
        while (current_row < rows && getline(file, line)) {
            
            if (line == "SWITCHES:") {
                break; 
            }
            
            if (line.empty()) {
                continue; 
            }
            for (int k = 0; k < cols; k++) {
                    if (line[k]>='A' && line[k]<='Z'){
                        int idx = getSwitchIndex(line[k]);
                        switchY[idx] = current_row;
                        switchX[idx] = k;
                    }
                    grid[current_index++] = line[k];
                }
            current_row++;
        }
        
        while (getline(file,line)){ //A PER_DIR 0 2 2 2 2 STRAIGHT TURN
            if (line == "TRAINS:") break;
            if (!line.empty()){
                int index = getSwitchIndex(line[0]);
                switchIndex[index] = index;
                if (line[2] == 'P')
                    {switchMode[index] = 2;
                    switchState[index] = (int)line[10]-48;
                    switchK[index][0] = (int)line[12]-48;
                    switchK[index][1] = (int)line[14]-48;
                    switchK[index][2] = (int)line[16]-48;
                    switchK[index][3] = (int)line[18]-48;
                    switchState0[index] = 0;
                    switchState1[index] = 1;
                    }
                else    
                    {switchMode[index] = 0;
                    switchState[index] = (int)line[9]-48;
                    switchK[index][0] = (int)line[11]-48;
                    switchK[index][1] = (int)line[13]-48;
                    switchK[index][2] = (int)line[15]-48;
                    switchK[index][3] = (int)line[17]-48;
                    switchState0[index] = 0;
                    switchState1[index] = 1;
                    }
            }else{
                getline(file,line);
                break;
            }
        }   
        
        int i = 0;
        while (getline(file,line))
        {
            if (line.empty())
                break; 
            else{             
                    int j = 0;
                    
                    while(line[j]!=' ')
                    {
                            trainSpawnTick[i] = trainSpawnTick[i]*(pow(10,1))+(int)line[j]-48;
                            j++;
                    } 
                    j++;
                        while(line[j]!=' ')
                    {
                            trainX[i] = trainX[i] *(pow(10,1))+(int)line[j]-48;
                            j++;

                    } 
                    j++;
                    while(line[j]!=' ')
                    {
                            trainY[i] = trainY[i]*(pow(10,1))+(int)line[j]-48;
                            j++;
                    } 
                    j++;
                    while(line[j]!=' ')
                    {
                            trainDirection[i] = trainDirection[i]*(pow(10,1))+(int)line[j]-48;
                            j++;
                    } 
                    j++;
                    while(line[j]!=' ')
                    {
                            trainColor[i]= trainColor[i]*(pow(10,1))+(int)line[j]-48;
                            j++;
                    }
                    i++;
                }
            }
    cout << "Level '" << filename << "' loaded successfully. Grid size: " << rows << "x" << cols << endl;
    return true;
    }
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
        signals_log << "TICK,SWITCH_ID,colsOR\n";
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
// Append tick, switch id, signal colsor to signals.csv.
// ----------------------------------------------------------------------------
void logSignalState(int switchIndex , char signalcolsor) {

    ofstream signals_log("signals.csv", ios::app); 
    // 3. WRITING: Write the data if the file is open
    if (!signals_log.is_open()) {
        cout << "Error:File not opened successfully!" << endl;
        return;
    }else {
        signals_log << tick << "," << switchIndex << "," << signalcolsor << "\n";
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