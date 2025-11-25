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
    return x >= 0 && x < cols && y >= 0 && y < rows;
}
// ----------------------------------------------------------------------------
// Check if a tile is a track tile.
// ----------------------------------------------------------------------------
// Returns true if the tile can be traversed by trains.
// ----------------------------------------------------------------------------
bool isTrackTile(int x, int y ) {
    if (!isInBounds(x, y)) {
        return false;
    }

    
    char tile = grid[(y * cols) + x];

    return tile == '-' || tile == '|' || tile == '+';
    
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

    
    char tile = grid[(y * cols) + x];

   
    return tile >= 'A' && tile <= 'Z';
}

// ----------------------------------------------------------------------------
// Get switch index from character.
// ----------------------------------------------------------------------------
// Maps 'A'..'Z' to 0..25, else -1.
// ----------------------------------------------------------------------------
int getSwitchIndex(char switchChar) {

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

        for (int i = 0; i < MAX_TRAINS; ++i) {
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
        for (int i = 0; i < MAX_TRAINS; ++i) {
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

    
    char& tile = grid[(y * cols) + x]; 

    if (tile == ' ') {
        
        tile = 'S'; 
        return true;
    } else if (tile == 'S') {
        
        tile = ' ';
        return true;
    }
    
    return false;
}
