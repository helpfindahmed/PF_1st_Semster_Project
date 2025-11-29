#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/grid.h"
#include "../core/switches.h"
#include "../core/io.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>

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
static Texture tileTexture;     // 3.png - switches



void printGrid(RenderWindow& window){
    bool hasTrackSprites = (trackTexture.getSize().x > 0);
    bool hasTileSprites = (tileTexture.getSize().x > 0);
    
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            float x = g_gridOffsetX + (j * g_cellSize);
            float y = g_gridOffsetY + (i * g_cellSize);
            char tile = grid[i * cols + j];
            
            Sprite sprite;
            bool drewSprite = false;
            float spriteWidth = 0;
            float spriteHeight = 0;
            
            // Try to draw from track texture first (5.png)
            if (hasTrackSprites) {
    sprite.setTexture(trackTexture);

    // Start of the converted if-else if logic
    
    // Case '=' (Safety buffer / Horizontal track)
    if (tile == '=') {
        // Horizontal track: (44, 63) size 340x142
        sprite.setTextureRect(IntRect(44, 63, 340, 142));
        spriteWidth = 340;
        spriteHeight = 142;
        drewSprite = true;
    } 
    
    // Case '|' (Vertical track)
    // NOTE: I corrected the previous 'case true' error and replaced it with '|'.
    else if (tile == '|') {
        // Vertical track: (450, 62) size 138x239
        sprite.setTextureRect(IntRect(450, 62, 138, 239));
        spriteWidth = 138;
        spriteHeight = 239;
        drewSprite = true;
    } 
    
    // Case '/' (Curve slash)
    else if (tile == '/') {
        // Curve slash: (46, 295) size 315x322
        sprite.setTextureRect(IntRect(46, 295, 315, 322));
        spriteWidth = 315;
        spriteHeight = 322;
        drewSprite = true;
    } 
    
    // Case '\' (Curve backslash)
    else if (tile == '\\') {
        // Curve backslash (flip / horizontally): (46, 295) size 315x322
        sprite.setTextureRect(IntRect(46, 295, 315, 322));
        spriteWidth = 315;
        spriteHeight = 322;
        drewSprite = true;
    } 
    
    // Case '+' (Crossing)
    else if (tile == '+') {
        // Crossing: (362, 666) size 295x314
        sprite.setTextureRect(IntRect(362, 666, 295, 314));
        spriteWidth = 295;
        spriteHeight = 314;
        drewSprite = true;
    }
    
    // End of the converted logic
    
    // The drawing/scaling logic that followed your original switch statement 
    // needs to be moved out here, after the final 'else if'.
    
    if (drewSprite) {
        sprite.setPosition(x, y);
        
        // Calculate scale to fit cell size
        float scaleX = g_cellSize / spriteWidth;
        float scaleY = g_cellSize / spriteHeight;
        
        // Special handling for backslash - flip horizontally
        if (tile == '\\') {
            sprite.setScale(-scaleX, scaleY);  // Negative X flips
            sprite.setPosition(x + g_cellSize, y);  // Adjust for flip
        } else {
            sprite.setScale(scaleX, scaleY);
        }
        
        window.draw(sprite);
    }
}
            
            
            
            // Try to draw from tile texture (3.png) for S, D, and switches
            if (!drewSprite && hasTileSprites) {
                sprite.setTexture(tileTexture);
                
                switch(tile) {
                    case 'S':  // Source/Spawn: (62, 99) size 171x174
                        sprite.setTextureRect(IntRect(62, 99, 171, 174));
                        spriteWidth = 171;
                        spriteHeight = 174;
                        drewSprite = true;
                        break;
                        
                    case 'D':  // Destination: (567, 99) size 170x170
                        sprite.setTextureRect(IntRect(567, 99, 170, 170));
                        spriteWidth = 170;
                        spriteHeight = 170;
                        drewSprite = true;
                        break;
                        
                    default:  // Switches A-Z: (66, 597) size 165x174
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
                    
                    // Calculate scale to fit cell size
                    float scaleX = g_cellSize / spriteWidth;
                    float scaleY = g_cellSize / spriteHeight;
                    sprite.setScale(scaleX, scaleY);
                    
                    window.draw(sprite);
                }
            }
            
            // If sprite not drawn, use colored rectangles (fallback)
            if (!drewSprite) {
                RectangleShape Rectangle(Vector2f(g_cellSize, g_cellSize));
                Rectangle.setPosition(x, y);
                
                if (tile == 'S' || tile == 'D') Rectangle.setFillColor(Color(100, 0, 100));
                else if (tile >= 'A' && tile <= 'Z') Rectangle.setFillColor(Color(255, 180, 50));
                else if (tile == '=') Rectangle.setFillColor(Color(100, 100, 255)); 
                else if (tile == '+' || tile == '|' || tile == '=' ) Rectangle.setFillColor(Color(250, 0, 250)); 
                else Rectangle.setFillColor(Color(180, 252, 20)); 
                
                Rectangle.setOutlineThickness(2.0f);
                Rectangle.setOutlineColor(Color(20, 20, 20));
                
                window.draw(Rectangle);
            }
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
    g_window = new RenderWindow(VideoMode(windowWidth, windowHeight), "Switchback Rails");
    if (!g_window) {
        return false;
    }
    g_window->setFramerateLimit(10);  // Slower so you can see simulation
    
    // Load track sprites (5.png)
    printf("Loading sprites...\n");
    if (!trackTexture.loadFromFile("Sprites/5.png")) {
        printf("Warning: Could not load 5.png - using colored rectangles\n");
    } else {
        printf("SUCCESS: 5.png loaded! Image size: %dx%d\n", 
               trackTexture.getSize().x, trackTexture.getSize().y);
    }
    
    // Load switch sprites (3.png) - we'll add this later
    if (!tileTexture.loadFromFile("Sprites/3.png")) {
        printf("Warning: Could not load 3.png\n");
    } else {
        printf("SUCCESS: 3.png loaded!\n");
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
    while (g_window->isOpen()) {
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
            if (!isSimulationComplete()){ //simulate a tick until all trains reach their destination or collide
                simulateOneTick();       //and simulate it as long as the window is open the moment it closes this also stops 
            }
        }
        
        g_window->clear(Color(50,50,50));
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