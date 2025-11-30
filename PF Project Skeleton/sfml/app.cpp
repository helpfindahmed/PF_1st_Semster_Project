#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/grid.h"
#include "../core/switches.h"
#include "../core/io.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;
// ============================================================================
// APP.CPP - Implementation of SFML application (NO CLASSES)
// ============================================================================

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES FOR APP STATE
// ----------------------------------------------------------------------------
static RenderWindow* g_window = nullptr;
static Font g_font;
static bool fontLoaded = false;

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
static float g_sidebarWidth = 200.0f;

static Texture trackTexture;      // 5.png - tracks
static Texture tileTexture;        // 3.png - switches
static Texture trainTexture;       //For trains

static SoundBuffer trainSoundBuffer;
static Sound trainSound;
static bool soundLoaded = false;

static Texture g_backgroundTexture;
static Sprite g_backgroundSprite;

void printGrid(RenderWindow& window){


    RectangleShape sidebar(Vector2f(g_sidebarWidth, window.getSize().y));
    sidebar.setPosition(1750, 0);
    sidebar.setFillColor(Color(40, 100, 40));
    window.draw(sidebar);



    int  totalActiveTrains = 0 ;

    for (int i = 0; i < maxTrains; i++){
        if (trainActive[i] == 1){
            totalActiveTrains++;
        }
    }

    if (fontLoaded) {
        float yPos = 20;
        
        Text texttrain;
        texttrain.setFont(g_font);
        texttrain.setString("Total Trains : " + to_string(totalActiveTrains));
        texttrain.setCharacterSize(20);
        texttrain.setFillColor(Color::White);
        texttrain.setPosition(1765, yPos);
        window.draw(texttrain);

        yPos +=40;

        Text text1;
        text1.setFont(g_font);
        text1.setString("Tick: " + to_string(tick));
        text1.setCharacterSize(20);
        text1.setFillColor(Color::White);
        text1.setPosition(1800, yPos);
        window.draw(text1);
        
        yPos += 40;
        
        Text text2;
        text2.setFont(g_font); 
        text2.setString("Arrived: " + to_string(numArrived));
        text2.setCharacterSize(20);
        text2.setFillColor(Color::White);
        text2.setPosition(1800, yPos);
        window.draw(text2);
        
        yPos += 40;
        
        Text text3;
        text3.setFont(g_font);
        text3.setString("Crashed: " + to_string(numCrashed));
        text3.setCharacterSize(20);
        text3.setFillColor(Color::White);
        text3.setPosition(1800, yPos);
        window.draw(text3);
    }


    bool hasTrackSprites = (trackTexture.getSize().x > 0);
    bool hasTileSprites = (tileTexture.getSize().x > 0);
    bool hasTrainSprites = (trainTexture.getSize().x > 0);
    
    
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            float x = g_gridOffsetX + (j * g_cellSize);
            float y = g_gridOffsetY + (i * g_cellSize);
            char tile = grid[i * cols + j];
            
           
            
            Sprite sprite;
            bool drewSprite = false;
            float spriteWidth = 0;
            float spriteHeight = 0;
            
            
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
               
                else if (tile == '+') {
                    sprite.setTextureRect(IntRect(443, 774, 121, 105));
                    spriteWidth = 121;
                    spriteHeight = 105;
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
                        sprite.setTextureRect(IntRect(51, 74, 113, 112));
                        spriteWidth = 113;
                        spriteHeight = 112;
                        drewSprite = true;
                        break;
                    case 'D':
                        sprite.setTextureRect(IntRect(382, 69, 115, 117));
                        spriteWidth = 115;
                        spriteHeight = 117;
                        drewSprite = true;
                        break;
                    default:

                    if (tile >= 'A' && tile <= 'Z'&& signalColor[getSwitchIndex(tile)]==2) {
                            sprite.setTextureRect(IntRect(50, 402, 113, 115));
                            spriteWidth = 113;
                            spriteHeight = 115;
                            drewSprite = true;
                        }

                        
                         else if (tile >= 'A' && tile <= 'Z'&& signalColor[getSwitchIndex(tile)]==1) {
                            sprite.setTextureRect(IntRect(220, 404, 112, 109));
                            spriteWidth = 112;
                            spriteHeight = 109;
                            drewSprite = true;
                        }
                        
                        
                   else if (tile >= 'A' && tile <= 'Z' ) {
                            sprite.setTextureRect(IntRect(382, 399, 115, 117));
                            spriteWidth = 115;
                            spriteHeight = 117;
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
    
  
    if (tile == 'S' || tile == 'D' || (tile >= 'A' && tile <= 'Z')) {
        
        RectangleShape Rectangle(Vector2f(g_cellSize, g_cellSize));
        Rectangle.setPosition(x, y);
        
        
        if (tile == 'S' || tile == 'D') Rectangle.setFillColor(Color(100, 0, 100));
        else if (tile >= 'A' && tile <= 'Z') Rectangle.setFillColor(Color(255, 180, 50));
        
        
        Rectangle.setOutlineThickness(2.0f);
        Rectangle.setOutlineColor(Color(20, 20, 20));
        
        window.draw(Rectangle);
    } 
    
}

        }
    }
    
    
  for (int i = 0; i < maxTrains; i++) {
        if (trainActive[i] == 1) {
            int trainCol = trainX[i];  
            int trainRow = trainY[i];              
            float x = g_gridOffsetX + (trainCol * g_cellSize);
            float y = g_gridOffsetY + (trainRow * g_cellSize);            
            if (hasTrainSprites) {
                 Sprite trainSprite;
                trainSprite.setTexture(trainTexture);
                
                trainSprite.setTextureRect(IntRect(113, 138, 372, 242));
                
                 float scaleX = g_cellSize / 372.0f;
                float scaleY = g_cellSize / 242.0f;
                trainSprite.setScale(scaleX, scaleY);
                trainSprite.setPosition(x, y);
                
                window.draw(trainSprite);
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
    unsigned int windowWidth = g_sidebarWidth + cols * (int)g_cellSize + 100;
    unsigned int windowHeight = rows * (int)g_cellSize + 100;
    g_window = new RenderWindow(VideoMode(windowWidth, windowHeight), "Switchback Rails");
    if (!g_window) {
        return false;
    }
    g_window->setFramerateLimit(10);
    trackTexture.loadFromFile("Sprites/5.png");
    tileTexture.loadFromFile("Sprites/7.png");
    trainTexture.loadFromFile("Sprites/2.png");
    trainSoundBuffer.loadFromFile("Sprites/train.wav");
    // g_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    if (!g_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
    printf("Warning: Could not load font\n");
    fontLoaded = false;
} else {
    printf("SUCCESS: Font loaded!\n");
    fontLoaded = true;
}

    printf("Loading sprites...\n");
    
     if (!trainSoundBuffer.loadFromFile("Sprites/train.wav")) {
        soundLoaded = false;
    } else {
        trainSound.setBuffer(trainSoundBuffer);
        trainSound.setLoop(true);
        trainSound.setVolume(30);
        soundLoaded = true;
    }
    
    if (!g_backgroundTexture.loadFromFile("Sprites/_.jpeg")) {
        printf("Warning: Could not load background image!\n");
    } else {
        g_backgroundTexture.setRepeated(true); 
        g_backgroundSprite.setTexture(g_backgroundTexture);
        
        
        unsigned int requiredWidth = cols * (int)g_cellSize + (int)g_gridOffsetX;
        unsigned int requiredHeight = rows * (int)g_cellSize + (int)g_gridOffsetY;
        
        g_backgroundSprite.setTextureRect(
            IntRect(0, 0, requiredWidth, requiredHeight) 
        );
        
        
        g_backgroundSprite.setPosition(0.0f, 0.0f);
        
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
    if (soundLoaded) {
        trainSound.play();
    }
    
    Clock clock; 
    float tickInterval = 0.5f; 
    float timeSinceLastTick = 0.0f;

    while (g_window->isOpen()) {
        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();
        timeSinceLastTick += deltaTime;

        Event event;
        while (g_window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                g_window->close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    g_window->close();
                }
                if (event.key.code == Keyboard::Space) {
                        g_isPaused = !g_isPaused;
                }
                if (event.key.code == Keyboard::Period) {
                    if (g_isPaused && !isSimulationComplete()) {
                        simulateOneTick();
                    }
                }
            }
        }

        if (!g_isPaused && !isSimulationComplete() && timeSinceLastTick >= tickInterval) {
            simulateOneTick();
            timeSinceLastTick = 0.0f;  // Reset timer
        }

        if (isSimulationComplete()) {
            if (soundLoaded && trainSound.getStatus() == sf::Sound::Playing) {
                trainSound.stop();
            }
            std::cout << "Simulation complete!" << std::endl;
            g_window->close();
        }
        

        
        g_window->clear(Color(50, 50, 50));

        if (g_backgroundTexture.getSize().x > 0) {
            g_window->draw(g_backgroundSprite);
        }



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
     if (g_window) { //if g_window pointer has a value meaning it isnt 0 or NULLPTR/
        delete g_window; //delete the heap in DMA
        g_window = nullptr; //make sure it isn't a dangling pointer now
    }
}