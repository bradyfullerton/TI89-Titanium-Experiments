#include <tigcclib.h>

// Define constants for visualization
#define NUM_DROPS 20
#define MAX_LENGTH 10
#define MIN_LENGTH 3
#define SCREEN_WIDTH 30
#define SCREEN_HEIGHT 10
#define DELAY_FACTOR 5

typedef struct {
    int x;           // x position (column)
    int y;           // y position of head (row)
    int length;      // length of the raindrop
    int active;      // 1 = falling, 0 = shrinking at bottom
} Raindrop;

// Function prototypes
void initializeRaindrops(Raindrop drops[], int count);
void updateRaindrop(Raindrop *drop);
void drawMatrix(Raindrop drops[], int count);
char generateRandomChar();

void _main(void)
{
    // Clear screen and disable cursor
    ClrScr();
    
    // Initialize random number generator
    randomize();
    
    // Create and initialize raindrops
    Raindrop raindrops[NUM_DROPS];
    initializeRaindrops(raindrops, NUM_DROPS);
    
    // Main animation loop
    while (!kbhit()) {
        // Update positions
        for (int i = 0; i < NUM_DROPS; i++) {
            updateRaindrop(&raindrops[i]);
        }
        
        // Draw the matrix
        drawMatrix(raindrops, NUM_DROPS);
        
        // Add delay
        for (int d = 0; d < DELAY_FACTOR * 1000; d++) {
            // Empty loop for delay
        }
    }
    
    // Clean up and exit
    ClrScr();
}

void initializeRaindrops(Raindrop drops[], int count)
{
    for (int i = 0; i < count; i++) {
        drops[i].x = random(SCREEN_WIDTH);
        drops[i].length = random(MAX_LENGTH - MIN_LENGTH + 1) + MIN_LENGTH;
        drops[i].y = -random(5) - 1; // Start above screen
        drops[i].active = 1;
    }
}

void updateRaindrop(Raindrop *drop)
{
    if (drop->active) {
        // Move the head down
        drop->y++;
        
        // If head reaches bottom, switch to shrinking mode
        if (drop->y >= SCREEN_HEIGHT) {
            drop->y = SCREEN_HEIGHT - 1;
            drop->active = 0;
        }
    } else {
        // In shrinking mode, if the tail moves out of screen, reset
        if (drop->y - drop->length >= SCREEN_HEIGHT - 1) {
            drop->x = random(SCREEN_WIDTH);
            drop->length = random(MAX_LENGTH - MIN_LENGTH + 1) + MIN_LENGTH;
            drop->y = 0; // Start at top
            drop->active = 1;
        }
    }
}

void drawMatrix(Raindrop drops[], int count)
{
    // Clear the screen first
    ClrScr();
    
    // Draw each raindrop
    for (int i = 0; i < count; i++) {
        // Calculate tail position
        int tailPos = drops[i].y - drops[i].length;
        
        // Draw the raindrop trail
        for (int j = tailPos + 1; j <= drops[i].y; j++) {
            if (j >= 0 && j < SCREEN_HEIGHT) {
                // Only draw visible parts
                char ch = generateRandomChar();
                DrawChar(drops[i].x * 8, j * 10, ch, A_NORMAL);
            }
        }
    }
    
    // Update screen
    UpdateScr();
}

char generateRandomChar()
{
    int charCode = random(64);
    
    // Skip letters and use only symbols
    if (charCode < 33)
        charCode += 32;  // Start at ASCII 32 (space)
    else if (charCode < 59)
        charCode += 7;   // Skip uppercase A-Z
    else
        charCode += 39;  // Skip lowercase a-z
    
    return (char)charCode;
}