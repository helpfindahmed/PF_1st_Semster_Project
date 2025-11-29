#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/grid.h"
#include "../core/switches.h"
#include "../core/io.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>
#include <string>
#include <iostream>

using namespace sf;
// ============================================================================
// APP.CPP - Implementation of SFML application (NO CLASSES)
// ============================================================================

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES FOR APP STATE
// ----------------------------------------------------------------------------
static RenderWindow* g_window = nullptr;
static Font g_font;

// View for camera (panning/zoom)
static View g_camera;

// Simulation state
static bool g_isPaused = false;
static bool g_isStepMode = false;

// Mouse state
static bool g_isDragging = false;
static int g_lastMouseX = 0;
static int g_lastMouseY = 0;

// Grid rendering parameters
static float g_cellSize = 32.0f;
static float g_gridOffsetX = 50.0f;
static float g_gridOffsetY = 50.0f;

void printGrid(RenderWindow& window){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            int screenX = g_gridOffsetX + (j * g_cellSize);
            int screenY = g_gridOffsetY + (i * g_cellSize);
            
            int gridIndex = i * cols + j;
            
            // Bounds check before accessing grid
            if (gridIndex < 0 || gridIndex >= rows * cols) {
                continue;
            }
            
            char tile = grid[gridIndex];
            
            RectangleShape Rectangle(Vector2f(g_cellSize, g_cellSize));
            Rectangle.setPosition(screenX, screenY);
            
            if (tile == 'S' || tile == 'D') Rectangle.setFillColor(Color(100, 255, 100));
            else if (tile >= 'A' && tile <= 'Z') Rectangle.setFillColor(Color(255, 180, 50));
            else if (tile == '=') Rectangle.setFillColor(Color(100, 100, 255)); 
            else if (tile == '+' || tile == '|' || tile == '-' || tile == '/' || tile == '\\') Rectangle.setFillColor(Color(150, 150, 150)); 
            else Rectangle.setFillColor(Color(50, 50, 50)); 
            
            Rectangle.setOutlineThickness(1.0f);
            Rectangle.setOutlineColor(Color(20, 20, 20));
            window.draw(Rectangle);
        }
    }
}

// ----------------------------------------------------------------------------
// INITIALIZATION// Draw the simulation elements

// ----------------------------------------------------------------------------
// This function will initialize the SFML application window and resources.
// It creates a render window with a specified size and title, sets the// Draw the simulation elements

// framerate limit, attempts to load a font file for text rendering, and
// initializes the camera view. Returns true on success, false on failure.
// This should be called once at the start of the application before entering
// the main loop.
// --------------------------------or--------------------------------------------
bool initializeApp() {
    unsigned int windowWidth =  cols * (int)g_cellSize + 100;
    unsigned int windowHeight = rows * (int)g_cellSize + 100;
    g_window = new RenderWindow(VideoMode(windowWidth,windowHeight), "Switchback Rails");
    if (!g_window) {
        return false;
    }
    g_window->setFramerateLimit(60);
    return true;
}

// ----------------------------------------------------------------------------
// MAIN RUN LOOP
// ----------------------------------------------------------------------------
// This function will run the main application loop. It handles event processing,
// simulation updates, and rendering. The loop continues while the window is open.
// It processes SFML events (window close, keyboard input, mouse input), updates
// the simulation at a fixed interval (2 ticks per second) when not paused,
// checks if the simulation is complete, and renders the current frame. Keyboard
// controls: SPACE to pause/resume, PERIOD to step one tick, ESC to exit. The
// loop exits when the window is closed or ESC is pressed.
// ----------------------------------------------------------------------------



void runApp() {
    Clock clock;  
    float tickInterval = 0.1f;  // 0.5 seconds per tick (2 ticks per second)
    float timeSinceLastTick = 0.0f;
    
    while (g_window->isOpen()) {
        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();
        timeSinceLastTick += deltaTime;
        
        // Handle events
        Event event;
        while (g_window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                g_window->close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    g_window->close();
                }
            }
        }
        
        // SIMULATION UPDATE - runs based on time, not events
        if (!isSimulationComplete() && timeSinceLastTick >= tickInterval) {
            simulateOneTick();
            timeSinceLastTick = 0.0f;  // Reset timer
            
            // Optional: print to console for debugging
            std::cout << "Tick: " << tick << " | Arrived: " << numArrived 
                      << " | Crashed: " << numCrashed << std::endl;
        }
        
        // If simulation is complete, close after a short delay
        if (isSimulationComplete()) {
            std::cout << "Simulation complete!" << std::endl;
            g_window->close();
        }
        
        // RENDERING
        g_window->clear(Color(50, 50, 50));
        printGrid(*g_window);
        g_window->display();
    }
}



// void runApp() {
//     Clock clock;
//     float tickInterval = 0.1f;  // Faster for debugging
//     float timeSinceLastTick = 0.0f;
    
//     int maxTicks = 200;  // Safety limit
    
//     while (g_window->isOpen()) {
//         float deltaTime = clock.restart().asSeconds();
//         timeSinceLastTick += deltaTime;
        
//         Event event;
//         while (g_window->pollEvent(event)) {
//             if (event.type == Event::Closed) {
//                 g_window->close();
//             }
//             if (event.type == Event::KeyPressed) {
//                 if (event.key.code == Keyboard::Escape) {
//                     g_window->close();
//                 }
//             }
//         }
        
//         if (!isSimulationComplete() && timeSinceLastTick >= tickInterval && tick < maxTicks) {
//             simulateOneTick();
//             timeSinceLastTick = 0.0f;
            
//             // Print every 10 ticks
//             if (tick % 10 == 0 || tick < 5) {
//                 std::cout << "Tick: " << tick << " | Arrived: " << numArrived 
//                           << " | Crashed: " << numCrashed << " | Active: ";
//                 for (int i = 0; i < maxTrains; i++) {
//                     if (trainActive[i]) {
//                         std::cout << i << "(" << trainX[i] << "," << trainY[i] << ") ";
//                     }
//                 }
//                 std::cout << std::endl;
//             }
//         }
        
//         if (isSimulationComplete() || tick >= maxTicks) {
//             std::cout << "Simulation complete! Tick=" << tick << std::endl;
//             g_window->close();
//         }
        
//         g_window->clear(Color(50, 50, 50));
//         printGrid(*g_window);
//         g_window->display();
//     }
// }

// ----------------------------------------------------------------------------
// CLEANUP
// ----------------------------------------------------------------------------
// This function will clean up all resources and close the application window.
// It deletes the render window object and sets the pointer to nullptr. This
// should be called once at the end of the application before exiting to ensure
// proper resource cleanup.
// ----------------------------------------------------------------------------
void cleanupApp() {
     if (g_window) { //if g_window pointer has a value meaning it isnt 0 or NULLPTR
        delete g_window; //delete the heap in DMA
        g_window = nullptr; //make sure it isn't a dangling pointer now
    }
}