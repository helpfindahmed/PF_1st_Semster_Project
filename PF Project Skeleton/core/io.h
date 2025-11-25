#ifndef IO_H
#define IO_H

// ============================================================================
// IO.H - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LEVEL LOADING
// ----------------------------------------------------------------------------
// Load a .lvl file.
bool loadLevelFile(const char* filename);

// ----------------------------------------------------------------------------
// LOGGING
// ----------------------------------------------------------------------------
// Create/clear log files.
void initializeLogFiles();

// Append train movement to trace.csv.
void logTrainTrace(int trainId);

// Append switch state to switches.csv.
void logSwitchState(int switchIndex);

// Append signal state to signals.csv.
void logSignalState(int switchIndex, char signalColor);

// Write final metrics to metrics.txt.
void writeMetrics();

#endif
