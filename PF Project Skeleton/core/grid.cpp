#include "grid.h"
#include "simulation_state.h"

// ============================================================================
// GRID.CPP - Grid utilities
// ============================================================================

// ----------------------------------------------------------------------------
// Check if a position is inside the grid.
// ----------------------------------------------------------------------------
// Returns true if x,y are within bounds.
// ----------------------------------------------------------------------------
bool isInBounds(int x , int y) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}
// ----------------------------------------------------------------------------
// Check if a tile is a track tile.
// ----------------------------------------------------------------------------
// Returns true if the tile can be traversed by trains.
// ----------------------------------------------------------------------------
bool isTrackTile(int x, int y) {
    if (!isInBounds(x, y)) {
        return false;
    }
    char tile = grid[(x * cols) + y];
    return (tile == '-' || tile == '|' || tile == '+' || tile == '/' || tile == '\\' || tile == '+');
}

// ----------------------------------------------------------------------------
// Check if a tile is a switch.
// ----------------------------------------------------------------------------
// Returns true if the tile is 'A'..'Z'.
// ----------------------------------------------------------------------------
bool isSwitchTile(int x , int y) {

     if (!isInBounds(x, y)) {
        return false;
    }
    char tile = grid[(x * cols) + y];
    return tile >= 'A' && tile <= 'Z';
}

// ----------------------------------------------------------------------------
// Get switch index from character.
// ----------------------------------------------------------------------------
// Maps 'A'..'Z' to 0..25, else -1.
// ----------------------------------------------------------------------------
int getSwitchIndex(char switchChar) { //we get switchChar when we read from the file and load the variables with actual data

    if (switchChar >= 'A' && switchChar <= 'Z') {
       
        return switchChar - 'A'; 
    }
    return -1;
}
// ----------------------------------------------------------------------------
// Check if a position is a spawn point.
// ----------------------------------------------------------------------------
// Returns true if x,y is a spawn.
// ----------------------------------------------------------------------------
bool isSpawnPoint(int x , int y) {
    for (int i = 0; i < maxTrains; i++) {
        if (spawnX[i] == x && spawnY[i] == y) {
            return true;
        }
    }
    return false;
}
// ----------------------------------------------------------------------------
// Check if a position is a destination.
// ----------------------------------------------------------------------------
// Returns true if x,y is a destination.
// ----------------------------------------------------------------------------
bool isDestinationPoint(int x , int y) {
    for (int i = 0; i < maxTrains; i++) {
        if (destX[i] == x && destY[i] == y) {
            return true;
        }
    }
    return false;
}
// ----------------------------------------------------------------------------
// Toggle a safety tile.
// ----------------------------------------------------------------------------
// Returns true if toggled successfully.
// ----------------------------------------------------------------------------
bool toggleSafetyTile(int x , int y) {
    if (!isInBounds(x, y)) {
        return false;
    }
    char tile = grid[(x * cols) + y]; 
    if (tile == '-') {
        tile = '='; 
        return true;

    } else if (tile == '=') {
        tile = '-';
        return true;
    }
    return false;
}   