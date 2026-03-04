# Wiring Guide - LoRa Communicator

Quick reference for wiring your ESP32 LoRa board to the keypad.

## Keypad Physical Layout

```
┌─────────────────────┐
│  1    2    3    A   │
│  4    5    6    B   │
│  7    8    9    C   │
│  *    0    #    D   │
└─────────────────────┘
```

## Keypad Pin Identification

Your 4x4 keypad has 8 pins on one edge. They represent:
- 4 Row pins (R1, R2, R3, R4)
- 4 Column pins (C1, C2, C3, C4)

**They may be labeled on the back of the keypad**, or you can identify them with a multimeter:
- When you press a button, it connects one row to one column
- For example, pressing "5" connects Row 2 to Column 2

## Connection Table

```
┌─────────────────┬──────────────┬─────────────┐
│ Keypad Pin      │ ESP32 GPIO   │ Wire Color  │
├─────────────────┼──────────────┼─────────────┤
│ Row 1           │ GPIO 13      │ (your choice)│
│ Row 2           │ GPIO 12      │             │
│ Row 3           │ GPIO 14      │             │
│ Row 4           │ GPIO 27      │             │
│ Column 1        │ GPIO 26      │             │
│ Column 2        │ GPIO 25      │             │
│ Column 3        │ GPIO 33      │             │
│ Column 4        │ GPIO 32      │             │
└─────────────────┴──────────────┴─────────────┘
```

**Tip:** Use different colored wires for rows vs columns to make troubleshooting easier.

## Visual Wiring Diagram

```
Keypad Side View:
┌───────────────────────────────────┐
│ [R1] [R2] [R3] [R4] [C1] [C2] [C3] [C4] │  ← 8 pins on edge
└───────────────────────────────────┘


Connect with jumper wires to ESP32:

Keypad          ESP32 LoRa Board
[R1]  ────────→ GPIO 13
[R2]  ────────→ GPIO 12
[R3]  ────────→ GPIO 14
[R4]  ────────→ GPIO 27
[C1]  ────────→ GPIO 26
[C2]  ────────→ GPIO 25
[C3]  ────────→ GPIO 33
[C4]  ────────→ GPIO 32
```

## ESP32 Pin Location Notes

The YELUFT ESP32 LoRa board has GPIO pins on both sides. Common layout:

**Left Side (typical):**
- GND, 3V3, GPIO pins 0-19, etc.

**Right Side (typical):**
- GPIO pins 21-39, VIN, GND, etc.

**Check your specific board's pinout** - look for labels on the PCB or check the manufacturer's diagram.

## Pins to AVOID

These pins are used by the SX1262 LoRa radio or have special boot functions:
- ❌ GPIO 0 (boot mode)
- ❌ GPIO 2 (boot mode / LED)
- ❌ GPIO 5 (SX1262 SPI)
- ❌ GPIO 15 (boot mode)
- ❌ GPIO 18 (SX1262 SPI SCK)
- ❌ GPIO 19 (SX1262 SPI MISO)
- ❌ GPIO 23 (SX1262 SPI MOSI)

**That's why we're using GPIO 12-14, 25-27, 32-33** - they're safe!

## Testing Your Wiring

Once wired:
1. Connect ESP32 to computer via USB-C
2. Upload Phase 1 code (PlatformIO > Upload)
3. Open Serial Monitor (115200 baud)
4. Press each key - should print to serial monitor

**Expected output:**
```
Key pressed: 1
Key pressed: 2
Key pressed: 3
Key pressed: A
... etc.
```

## Troubleshooting

### No keys respond
- Check all 8 wires are connected
- Verify ESP32 is powered on
- Check Serial Monitor is open and set to 115200 baud

### Wrong keys appear
- Your keypad might have rows/columns swapped
- Try swapping the row wire group with column wire group

### Some keys work, others don't
- Check individual wire connections
- Use multimeter to verify continuity
- May have a cold solder joint on keypad header

### Multiple keys register from one press
- This is contact bounce - the code has debouncing built in
- If severe, increase `DEBOUNCE_DELAY` in code (currently 50ms)

## Next Steps

✅ **Once all 16 keys work reliably**, you're ready for Phase 2!

Phase 2 will add the OLED display and T9 text input, so you can start composing messages on the device.
