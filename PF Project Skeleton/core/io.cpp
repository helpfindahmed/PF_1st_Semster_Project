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




// bool loadLevelFile(const char* filename) {
//     initializeSimulationState(); 
//     ifstream file (filename);
//     if (!file.is_open()){
//         cout << "Error: could not load the file " << filename << endl; 
//         return false;
//     }else{
//         string line;
            
//         while (getline(file, line)) {
//             if (line == "ROWS:")
//                 break;
//         }
//         file >> rows;
                
//         while (getline(file, line)) {
//             if (line == "COLS:")
//                 break;
//         }
//         file >> cols;
        
//         while (getline(file, line)) {
//             if (line == "SEED:")
//                 break;
//         }
//         file >> seed;
        
//         while (getline(file, line)) {
//             if (line == "WEATHER:")
//                 break;
//         }
//         file >> line;
        
//         grid = new char[rows * cols];    
//         for (int k = 0; k < rows * cols; k++) {
//             grid[k] = ' ';
//         }

//         while (getline(file, line)) {
//             if (line == "MAP:")
//                 break;
//         }
     
//         int current_row = 2;
//         int current_index = 2 * cols; 
        
//         int destCount = 0;
//         int tempDestX[10] = {};
//         int tempDestY[10] = {};
//         while (current_row < rows && getline(file, line)) {
            
//             if (line == "SWITCHES:") {
//                 break; 
//             }
            
//             if (line.empty()) {
//                 continue; 
//             }
//             for (int k = 0; k < cols; k++) {
//                     if (line[k]>='A' && line[k]<='Z'){
//                         int idx = getSwitchIndex(line[k]);
//                         switchX[idx] = current_row; 
//                         switchY[idx] = k;
                       
//                     }
//                     if (line[k] == 'D' && destCount < 10) {
//                         tempDestX[destCount] = current_row;
//                         tempDestY[destCount] = k;
//                         destCount++;
//                     }
//                     grid[current_index++] = line[k];
//                 }
//             current_row++;
//         }
        
//         while (getline(file,line)){ //A PER_DIR 0 2 2 2 2 STRAIGHT TURN
//             if (line == "TRAINS:") break;
//             if (!line.empty()){
//                 int index = getSwitchIndex(line[0]);
//                 switchIndex[index] = index;
//                 if (line[2] == 'P')
//                     {switchMode[index] = 2;
//                     switchState[index] = (int)line[10]-48;
//                     switchK[index][0] = (int)line[12]-48;
//                     switchK[index][1] = (int)line[14]-48;
//                     switchK[index][2] = (int)line[16]-48;
//                     switchK[index][3] = (int)line[18]-48;
//                     switchState0[index] = 0;
//                     switchState1[index] = 1;
//                     }
//                 else    
//                     {switchMode[index] = 0;
//                     switchState[index] = (int)line[9]-48;
//                     switchK[index][0] = (int)line[11]-48;
//                     switchK[index][1] = (int)line[13]-48;
//                     switchK[index][2] = (int)line[15]-48;
//                     switchK[index][3] = (int)line[17]-48;
//                     switchState0[index] = 0;
//                     switchState1[index] = 1;
//                     }
//             }else{
//                 getline(file,line);
//                 break;
//             }
//         }   
        
//         int i = 0;
//         while (getline(file,line))
//         {
//             if (line.empty())
//                 break; 
//             else{             
//                     int j = 0;
                    
//                     while(line[j]!=' ')
//                     {
//                             trainSpawnTick[i] = trainSpawnTick[i]*10+(int)line[j]-48;
//                             j++;
//                     } 
//                     j++;
//                         while(line[j]!=' ')
//                     {
//                             trainX[i] = trainX[i] *10+(int)line[j]-48;
//                             j++;

//                     } 
//                     j++;
//                     while(line[j]!=' ')
//                     {
//                             trainY[i] = trainY[i]*10+(int)line[j]-48;
//                             j++;
//                     } 
//                     j++;
//                     while(line[j]!=' ')
//                     {
//                             trainDirection[i] = trainDirection[i]*10+(int)line[j]-48;
//                             j++;
//                     } 
//                     j++;
//                     while(line[j]!=' ')
//                     {
//                             trainColor[i]= trainColor[i]*10+(int)line[j]-48;
//                             j++;
//                     }

//                     spawnX[i] = trainX[i];
//                     spawnY[i] = trainY[i];
//                     if (destCount > 0) {
//                         int destIndex = i % destCount;  // Round-robin assignment
//                         destX[i] = tempDestX[destIndex];
//                         destY[i] = tempDestY[destIndex];
//                     }
                    
//                     i++;
//                 }
//             }
//             maxTrains = i;
        
//     cout << "Level '" << filename << "' loaded successfully. Grid size: " << rows << "x" << cols << endl;
//     return true;
//     }
// }



bool loadLevelFile(const char* filename) {
    initializeSimulationState(); 
    ifstream file(filename);
    if (!file.is_open()){
        cout << "Error: could not load the file " << filename << endl; 
        return false;
    }
    
    string line;
    
    // Read ROWS
    while (getline(file, line)) {
        if (line == "ROWS:")
            break;
    }
    file >> rows;
    
    // Read COLS
    while (getline(file, line)) {
        if (line == "COLS:")
            break;
    }
    file >> cols;
    
    // Read SEED
    while (getline(file, line)) {
        if (line == "SEED:")
            break;
    }
    file >> seed;
    
    // Read WEATHER
    while (getline(file, line)) {
        if (line == "WEATHER:")
            break;
    }
    file >> line;
    
    // Allocate grid
    grid = new char[rows * cols];
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            grid[i*cols + j] = ' ';
        }
    }
    // Skip to MAP section
    while (getline(file, line)) {
        if (line == "MAP:")
            break;
    }
    
    // Read the map - fill the ENTIRE grid from row 0
    int destCount = 0;
    int tempDestX[10] = {};
    int tempDestY[10] = {};
    cout << "Starting grid reading" << endl;
    int i = 2;
    while (i < rows && getline(file,line)){
        if (line.empty()){ 
            cout << "Gyattt ts is empty" << endl;
            continue;
        }
        if (line == "SWITCHES:")    
            break;
        
        for (int j = 0; j < cols; j++) {
            char tile = line[j];
            grid[i * cols + j] = tile;
            
            // Store switch positions (exclude S and D)
            if (tile >= 'A' && tile <= 'Z' && tile != 'S' && tile != 'D') {
                int idx = getSwitchIndex(tile);
                if (idx != -1) {
                    switchX[idx] = j;
                    switchY[idx] = i;
                    cout << "Switch " << tile << " at (" << switchX[idx] << "," << switchY[idx] << ")" << endl;
                }
            }

            if (tile == 'D' && destCount < 10) {
                tempDestX[destCount] = j;
                tempDestY[destCount] = i;
                destCount++;
            }
        }
        i++;
    }
    
    cout << "Found " << destCount << " destinations" << endl;
    for (int d = 0; d < destCount; d++) {
        cout << "  Dest " << d << ": column=" << tempDestX[d] << ", row=" << tempDestY[d] << endl;
    }
    
    // Read SWITCHES section
    while (getline(file, line)) {
        if (line == "TRAINS:") break;
        if (line.empty()) continue;
        
        int index = getSwitchIndex(line[0]);
        if (index == -1) continue;
        
        switchIndex[index] = index;
        
        if (line.length() > 2 && line[2] == 'P') {
            switchMode[index] = 2; // PER_DIR
            switchState[index] = (int)line[10] - 48;
            switchK[index][0] = (int)line[12] - 48;
            switchK[index][1] = (int)line[14] - 48;
            switchK[index][2] = (int)line[16] - 48;
            switchK[index][3] = (int)line[18] - 48;
            switchState0[index] = 0;
            switchState1[index] = 1;
        } else if (line.length() > 2 && line[2] == 'G') {
            switchMode[index] = 0; // GLOBAL
            switchState[index] = (int)line[9] - 48;
            switchK[index][0] = (int)line[11] - 48;
            switchK[index][1] = (int)line[13] - 48;
            switchK[index][2] = (int)line[15] - 48;
            switchK[index][3] = (int)line[17] - 48;
            switchState0[index] = 0;
            switchState1[index] = 1;
        }
    }
    
    // Read TRAINS section
    int trainCount = 0;
    while (getline(file, line) && !line.empty() && trainCount < 10) {
        int values[5] = {0, 0, 0, 0, 0};
        int valueIndex = 0;
        int j = 0;
        
        // Parse 5 space-separated integers
        while (valueIndex < 5 && j < (int)line.length()) {
            if (line[j] == ' ') {
                valueIndex++;
                j++;
            } else if (line[j] >= '0' && line[j] <= '9') {
                values[valueIndex] = values[valueIndex] * 10 + (line[j] - '0');
                j++;
            } else {
                j++;
            }
        }
        
        trainSpawnTick[trainCount] = values[0];
        trainX[trainCount] = values[1];
        trainY[trainCount] = values[2];
        trainDirection[trainCount] = values[3];
        trainColor[trainCount] = values[4];
        
        spawnX[trainCount] = trainX[trainCount];
        spawnY[trainCount] = trainY[trainCount];
        
        // Assign destination round-robin
        if (destCount > 0) {
            int destIndex = trainCount % destCount;
            destX[trainCount] = tempDestX[destIndex];
            destY[trainCount] = tempDestY[destIndex];
        }
        
        cout << "Train " << trainCount << ": spawn=(" << trainX[trainCount] << "," 
             << trainY[trainCount] << ") dest=(" << destX[trainCount] << "," 
             << destY[trainCount] << ")" << endl;
        
        trainCount++;
    }
    
    maxTrains = trainCount;
    
    file.close();
    cout << "Level loaded: " << rows << "x" << cols << ", " << maxTrains << " trains" << endl;
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
        trace_log << "Tick,TrainID,X,Y,Direction,State\n";
        trace_log.close();
    }

    
    ofstream switches_log("switches.csv");
    if (switches_log.is_open()) {
        switches_log << "Tick,Switch,Mode,State\n";
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
        string modeStr;
        if (switchMode[switchIndex] == 0) {
            modeStr = "GLOBAL";
        } else if (switchMode[switchIndex] == 2) {
            modeStr = "PER_DIR";
        } else {
            modeStr = "UNKNOWN";
        }
         switches_log << tick << ","
                     << (char) (switchIndex + 'A') << ","
                     << modeStr << ","
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