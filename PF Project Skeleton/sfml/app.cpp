#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/grid.h"
#include "../core/switches.h"
#include "../core/io.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>
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

static Texture trackTexture;      // 5.png - tracks
static Texture tileTexture;        // 3.png - switches
static Texture trainTexture;       // NEW: For trains

void printGrid(RenderWindow& window){
    bool hasTrackSprites = (trackTexture.getSize().x > 0);
    bool hasTileSprites = (tileTexture.getSize().x > 0);
    bool hasTrainSprites = (trainTexture.getSize().x > 0);
    
    // First, draw all the track tiles (your existing code)
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            float x = g_gridOffsetX + (j * g_cellSize);
            float y = g_gridOffsetY + (i * g_cellSize);
            char tile = grid[i * cols + j];
            
            // ... all your existing tile drawing code ...
            
            Sprite sprite;
            bool drewSprite = false;
            float spriteWidth = 0;
            float spriteHeight = 0;
            
            // [Keep all your existing track and tile drawing code here]
            if (hasTrackSprites) {
                sprite.setTexture(trackTexture);
                
                if (tile == '=') {
                    sprite.setTextureRect(IntRect(44, 63, 340, 142));
                    spriteWidth = 340;
                    spriteHeight = 142;
                    drewSprite = true;
                } 
                else if (tile == '|') {
                    sprite.setTextureRect(IntRect(450, 62, 138, 239));
                    spriteWidth = 138;
                    spriteHeight = 239;
                    drewSprite = true;
                } 
                else if (tile == '/') {
                    sprite.setTextureRect(IntRect(46, 295, 315, 322));
                    spriteWidth = 315;
                    spriteHeight = 322;
                    drewSprite = true;
                } 
                else if (tile == '\\') {
                    sprite.setTextureRect(IntRect(46, 295, 315, 322));
                    spriteWidth = 315;
                    spriteHeight = 322;
                    drewSprite = true;
                } 
                else if (tile == '+') {
                    sprite.setTextureRect(IntRect(362, 666, 295, 314));
                    spriteWidth = 295;
                    spriteHeight = 314;
                    drewSprite = true;
                }
                
                if (drewSprite) {
                    sprite.setPosition(x, y);
                    float scaleX = g_cellSize / spriteWidth;
                    float scaleY = g_cellSize / spriteHeight;
                    
                    if (tile == '\\') {
                        sprite.setScale(-scaleX, scaleY);
                        sprite.setPosition(x + g_cellSize, y);
                    } else {
                        sprite.setScale(scaleX, scaleY);
                    }
                    window.draw(sprite);
                }
            }
            
            if (!drewSprite && hasTileSprites) {
                sprite.setTexture(tileTexture);
                
                switch(tile) {
                    case 'S':
                        sprite.setTextureRect(IntRect(62, 99, 171, 174));
                        spriteWidth = 171;
                        spriteHeight = 174;
                        drewSprite = true;
                        break;
                    case 'D':
                        sprite.setTextureRect(IntRect(567, 99, 170, 170));
                        spriteWidth = 170;
                        spriteHeight = 170;
                        drewSprite = true;
                        break;
                    default:
                        if (tile >= 'A' && tile <= 'Z') {
                            sprite.setTextureRect(IntRect(66, 597, 165, 174));
                            spriteWidth = 165;
                            spriteHeight = 174;
                            drewSprite = true;
                        }
                        break;
                }
                
                if (drewSprite) {
                    sprite.setPosition(x, y);
                    float scaleX = g_cellSize / spriteWidth;
                    float scaleY = g_cellSize / spriteHeight;
                    sprite.setScale(scaleX, scaleY);
                    window.draw(sprite);
                }
            }
            
            if (!drewSprite) {
                RectangleShape Rectangle(Vector2f(g_cellSize, g_cellSize));
                Rectangle.setPosition(x, y);
                
                if (tile == 'S' || tile == 'D') Rectangle.setFillColor(Color(100, 0, 100));
                else if (tile >= 'A' && tile <= 'Z') Rectangle.setFillColor(Color(255, 180, 50));
                else if (tile == '=') Rectangle.setFillColor(Color(100, 100, 255)); 
                else if (tile == '+' || tile == '|' || tile == '=') Rectangle.setFillColor(Color(250, 0, 250)); 
                else Rectangle.setFillColor(Color(50, 50, 50)); 
                
                Rectangle.setOutlineThickness(2.0f);
                Rectangle.setOutlineColor(Color(20, 20, 20));
                window.draw(Rectangle);
            }
        }
    }
    
    // NOW draw trains ON TOP of the tracks
    for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            int trainCol = trainX[i];  // X = column
            int trainRow = trainY[i];  // Y = row
            
            float x = g_gridOffsetX + (trainCol * g_cellSize);
            float y = g_gridOffsetY + (trainRow * g_cellSize);
            
            if (hasTrainSprites) {
                // Use train sprite from 1.png
                Sprite trainSprite;
                trainSprite.setTexture(trainTexture);
                
                // Train sprite coordinates (adjust these based on your sprite sheet)
                trainSprite.setTextureRect(IntRect(0, 0, 64, 64));  // Adjust based on actual sprite
                
                // Scale to fit cell
                float scale = g_cellSize / 64.0f;
                trainSprite.setScale(scale, scale);
                trainSprite.setPosition(x, y);
                
                // Color the train based on trainColor
                Color trainColors[] = {
                    Color::Red, Color::Blue, Color::Green, Color::Yellow, Color::Magenta,
                    Color::Cyan, Color(255, 128, 0), Color(128, 0, 255), Color(255, 0, 128), Color(0, 255, 128)
                };
                trainSprite.setColor(trainColors[trainColor[i] % 10]);
                
                window.draw(trainSprite);
            } else {
                // Fallback: colored circles
                CircleShape trainCircle(g_cellSize * 0.3f);
                trainCircle.setPosition(x + g_cellSize * 0.2f, y + g_cellSize * 0.2f);
                
                Color trainColors[] = {
                    Color::Red, Color::Blue, Color::Green, Color::Yellow, Color::Magenta,
                    Color::Cyan, Color(255, 128, 0), Color(128, 0, 255), Color(255, 0, 128), Color(0, 255, 128)
                };
                trainCircle.setFillColor(trainColors[trainColor[i] % 10]);
                trainCircle.setOutlineThickness(2.0f);
                trainCircle.setOutlineColor(Color::White);
                
                window.draw(trainCircle);
            }
            
            // Optional: Draw train ID number
            // (You'd need to load a font for this)
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
    unsigned int windowWidth = cols * (int)g_cellSize + 100;
    unsigned int windowHeight = rows * (int)g_cellSize + 100;
    g_window = new RenderWindow(VideoMode(windowWidth, windowHeight), "Switchback Rails");
    if (!g_window) {
        return false;
    }
    g_window->setFramerateLimit(10);
    
    printf("Loading sprites...\n");
    
    if (!trackTexture.loadFromFile("Sprites/5.png")) {
        printf("Warning: Could not load 5.png - using colored rectangles\n");
    } else {
        printf("SUCCESS: 5.png loaded!\n");
    }
    
    if (!tileTexture.loadFromFile("Sprites/3.png")) {
        printf("Warning: Could not load 3.png\n");
    } else {
        printf("SUCCESS: 3.png loaded!\n");
    }
    
    // Load train sprites
    if (!trainTexture.loadFromFile("Sprites/1.png")) {
        printf("Warning: Could not load 1.png - using colored circles\n");
    } else {
        printf("SUCCESS: 1.png loaded!\n");
    }
    
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
    Clock clock;  // Add this - SFML clock for timing
    float tickInterval = 0.5f;  // 0.5 seconds per tick (2 ticks per second)
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