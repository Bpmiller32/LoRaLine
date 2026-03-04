/*
 * LoRa Communicator - Phase 1: Keypad Testing
 * 
 * This is the first phase - test your keypad wiring before proceeding.
 * 
 * WIRING:
 * Keypad Layout:
 *     C1   C2   C3   C4
 * R1  1    2    3    A
 * R2  4    5    6    B
 * R3  7    8    9    C
 * R4  *    0    #    D
 * 
 * Connect to ESP32:
 * Row 1 (R1) -> GPIO 13
 * Row 2 (R2) -> GPIO 12
 * Row 3 (R3) -> GPIO 14
 * Row 4 (R4) -> GPIO 27
 * Col 1 (C1) -> GPIO 26
 * Col 2 (C2) -> GPIO 25
 * Col 3 (C3) -> GPIO 33
 * Col 4 (C4) -> GPIO 32
 * 
 * TESTING:
 * 1. Upload this code
 * 2. Open Serial Monitor (115200 baud)
 * 3. Press each key on keypad
 * 4. Verify correct character appears in Serial Monitor
 * 5. If all 16 keys work, proceed to Phase 2
 */

#include <Arduino.h>

// ============================================================================
// PIN DEFINITIONS
// ============================================================================

/* Keypad row pins (outputs - we drive these LOW one at a time) */
const int ROW_PINS[4] = {13, 12, 14, 27};

/* Keypad column pins (inputs with pullups - we read these) */
const int COL_PINS[4] = {26, 25, 33, 32};

/* Number of rows and columns in our 4x4 keypad matrix */
const int NUM_ROWS = 4;
const int NUM_COLS = 4;

// ============================================================================
// KEYPAD CHARACTER MAP
// ============================================================================

/* 
 * Map of what character each button represents
 * Array layout matches physical keypad layout
 */
const char KEYPAD_CHARS[4][4] = {
    {'1', '2', '3', 'A'},  /* Row 1 */
    {'4', '5', '6', 'B'},  /* Row 2 */
    {'7', '8', '9', 'C'},  /* Row 3 */
    {'*', '0', '#', 'D'}   /* Row 4 */
};

// ============================================================================
// DEBOUNCING VARIABLES
// ============================================================================

/*
 * Debouncing prevents a single button press from registering multiple times
 * due to mechanical bounce in the switch contacts
 */

/* Last key that was detected (row * 4 + col, or -1 if none) */
int lastKeyIndex = -1;

/* Time (in milliseconds) when the last key was first detected */
unsigned long lastKeyTime = 0;

/* How long to wait before accepting a new key press (milliseconds) */
const unsigned long DEBOUNCE_DELAY = 50;

// ============================================================================
// FUNCTION: initKeypad
// Initialize the keypad GPIO pins
// ============================================================================
void initKeypad() {
    /* Set up row pins as outputs (we'll drive them LOW to scan) */
    for (int i = 0; i < NUM_ROWS; i++) {
        pinMode(ROW_PINS[i], OUTPUT);
        digitalWrite(ROW_PINS[i], HIGH);  /* Start HIGH (inactive) */
    }
    
    /* Set up column pins as inputs with internal pullup resistors */
    for (int i = 0; i < NUM_COLS; i++) {
        pinMode(COL_PINS[i], INPUT_PULLUP);
    }
    
    Serial.println("Keypad initialized");
    Serial.println("Press keys to test...");
    Serial.println();
}

// ============================================================================
// FUNCTION: scanKeypad
// Scan the keypad matrix and return the pressed key (or '\0' if none)
// ============================================================================
char scanKeypad() {
    unsigned long currentTime = millis();
    
    /*
     * Scan through each row of the keypad matrix
     * For each row:
     * 1. Drive that row LOW
     * 2. Check each column - if any column reads LOW, that button is pressed
     * 3. Drive the row back HIGH
     */
    for (int row = 0; row < NUM_ROWS; row++) {
        /* Activate this row (drive it LOW) */
        digitalWrite(ROW_PINS[row], LOW);
        
        /* Small delay to let the signal stabilize */
        delayMicroseconds(10);
        
        /* Check each column in this row */
        for (int col = 0; col < NUM_COLS; col++) {
            /* If this column reads LOW, the button at [row][col] is pressed */
            if (digitalRead(COL_PINS[col]) == LOW) {
                /* Calculate unique index for this key (0-15) */
                int keyIndex = row * NUM_COLS + col;
                
                /*
                 * Debouncing logic:
                 * Only register a new key if either:
                 * 1. It's a different key than before, OR
                 * 2. Enough time has passed since the last key press
                 */
                if (keyIndex != lastKeyIndex || 
                    (currentTime - lastKeyTime) > DEBOUNCE_DELAY) {
                    
                    lastKeyIndex = keyIndex;
                    lastKeyTime = currentTime;
                    
                    /* Deactivate this row before returning */
                    digitalWrite(ROW_PINS[row], HIGH);
                    
                    /* Return the character for this key */
                    return KEYPAD_CHARS[row][col];
                }
                
                /* Still in debounce period - ignore this press */
                digitalWrite(ROW_PINS[row], HIGH);
                return '\0';
            }
        }
        
        /* Deactivate this row (drive it back HIGH) */
        digitalWrite(ROW_PINS[row], HIGH);
    }
    
    /*
     * No key is currently pressed
     * Reset lastKeyIndex so next press will register immediately
     */
    lastKeyIndex = -1;
    return '\0';
}

// ============================================================================
// FUNCTION: setup
// Arduino setup function - runs once at startup
// ============================================================================
void setup() {
    /* Initialize serial communication for debugging */
    Serial.begin(115200);
    delay(1000);  /* Wait for serial monitor to connect */
    
    Serial.println();
    Serial.println("==================================");
    Serial.println("LoRa Communicator - Keypad Test");
    Serial.println("==================================");
    Serial.println();
    
    /* Initialize the keypad */
    initKeypad();
}

// ============================================================================
// FUNCTION: loop
// Arduino main loop - runs repeatedly
// ============================================================================
void loop() {
    /* Scan the keypad */
    char key = scanKeypad();
    
    /* If a key was pressed, print it to Serial Monitor */
    if (key != '\0') {
        Serial.print("Key pressed: ");
        Serial.println(key);
    }
    
    /* Small delay to prevent overwhelming the serial output */
    delay(10);
}

/*
 * ============================================================================
 * TESTING CHECKLIST:
 * ============================================================================
 * 
 * Test each key and verify correct output:
 * 
 * Row 1: [ ] 1  [ ] 2  [ ] 3  [ ] A
 * Row 2: [ ] 4  [ ] 5  [ ] 6  [ ] B
 * Row 3: [ ] 7  [ ] 8  [ ] 9  [ ] C
 * Row 4: [ ] *  [ ] 0  [ ] #  [ ] D
 * 
 * Common issues if keys don't work:
 * - Check wiring connections
 * - Verify row/column pins are correct for your keypad
 * - Some keypads have rows/columns reversed - try swapping
 * - Use a multimeter to verify keypad contacts
 * 
 * Once all 16 keys work correctly, you're ready for Phase 2!
 * 
 * ============================================================================
 */
