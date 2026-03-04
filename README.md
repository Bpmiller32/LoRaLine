# LoRa Communicator - Build Guide

Off-grid encrypted text communicator using ESP32 LoRa and physical keypad.

## Overview

This project builds handheld LoRa communicators for emergency/off-grid use. The devices:
- Send/receive encrypted messages via LoRa (915 MHz)
- Work completely standalone (no phone, no internet)
- Use T9 text input via physical 4x4 keypad
- Display messages on built-in OLED screen
- Store message history in flash memory

## Hardware Required

1. **YELUFT ESP32 LoRa V4 Development Board** (with integrated 0.96" OLED)
   - ESP32 MCU + SX1262 LoRa radio
   - 915 MHz frequency
   - Built-in OLED display (0.96", 128x64)
   - USB-C connector

2. **4x4 Matrix Keypad** (16 keys with pre-soldered headers)

3. **Battery System**
   - 3000 mAh Li-ion or LiPo battery (3.7V)
   - Physical power switch (inline with battery)
   - JST connector for battery connection

4. **Antenna**
   - 915 MHz LoRa antenna (SMA connector)
   - ~10 dBi gain recommended

5. **Wiring**
   - Jumper wires (male-to-female)
   - Or direct soldering for permanent installation

## Development Phases

This project is built in phases. **Complete each phase before moving to the next.**

### 📍 Phase 1: Keypad Testing (CURRENT PHASE)
**Goal:** Verify all hardware connections work before writing complex code

**Tasks:**
- [ ] Wire 4x4 keypad to ESP32 GPIO pins
- [ ] Upload Phase 1 code to ESP32
- [ ] Open Serial Monitor (115200 baud)
- [ ] Test all 16 keys (1-9, 0, *, #, A, B, C, D)
- [ ] Verify each key prints correct character
- [ ] Fix any wiring issues

**Estimated Time:** 30-60 minutes  
**Status:** ✅ Code ready in `src/main.cpp` - Ready to test!

---

### Phase 2: T9 Input + Display
**Goal:** Get the user interface working - type messages and see them on screen

**What you'll add:**
- Initialize OLED display (built-in on your board)
- Implement T9 text input (like old phone texting)
- Handle shift key (*) for capitals
- Handle punctuation key (#)
- Show typed text on OLED screen
- Test backspace (C key)

**New functions added:**
- `initDisplay()` - Initialize the OLED
- `getT9Char()` - Convert keypad number to letter (2=ABC, 3=DEF, etc.)
- `handleTextInput()` - Process key presses into text
- `drawComposeScreen()` - Show current message being typed

**Estimated Time:** 2-3 hours  
**Status:** ⏳ Code will be provided after Phase 1 complete

---

### Phase 3: LoRa Communication
**Goal:** Get two devices talking to each other (unencrypted first)

**What you'll add:**
- Initialize SX1262 LoRa radio with RadioLib
- Configure radio parameters (frequency, spreading factor, bandwidth)
- Send test packets when D key is pressed
- Receive packets and display on screen
- Test with both devices simultaneously

**New functions added:**
- `initLoRa()` - Set up the SX1262 radio chip
- `sendLoRaPacket()` - Transmit a message
- `checkForIncomingPackets()` - Listen for received messages
- `onReceive()` - Handle received packet

**Key settings:**
- Frequency: 915.0 MHz
- Spreading Factor: 9 (good range/speed balance)
- Bandwidth: 125 kHz
- Output Power: 22 dBm (maximum legal)

**Estimated Time:** 3-4 hours  
**Status:** ⏳ Code will be provided after Phase 2 complete

---

### Phase 4: Encryption
**Goal:** Secure all messages with AES-256 encryption

**What you'll add:**
- Generate shared encryption key (same on all your devices)
- Encrypt messages before sending
- Decrypt received messages
- Auto-generate device ID from ESP32 MAC address
- Add sender ID to each packet

**New functions added:**
- `encryptMessage()` - AES-256 encryption
- `decryptMessage()` - AES-256 decryption
- `getDeviceId()` - Get unique ID from MAC address
- Packet structure with sender ID and encrypted payload

**Security:**
- AES-256 encryption (same key on all your devices)
- Device ID shows who sent each message
- Pre-shared key (you control which devices can communicate)

**Estimated Time:** 2-3 hours  
**Status:** ⏳ Code will be provided after Phase 3 complete

---

### Phase 5: Message Storage
**Goal:** Save message history that persists across power cycles

**What you'll add:**
- Store received messages in ESP32 NVS (flash memory)
- Implement circular buffer (oldest message deleted when full)
- Inbox screen to read old messages
- Navigate with A key (cycles through messages)
- Delete messages with C key
- Message counter showing X/Y (current/total)

**New functions added:**
- `saveMessage()` - Store message to flash
- `loadMessage()` - Retrieve message from flash
- `deleteMessage()` - Remove message and shift others
- `drawInboxScreen()` - Display received messages
- `nextMessage()` - Cycle through inbox

**Storage:**
- 50 message capacity (circular buffer)
- Newest messages shown first
- Persists across power cycles
- Sender ID shown with each message

**Estimated Time:** 3-4 hours  
**Status:** ⏳ Code will be provided after Phase 4 complete

---

### Phase 6: Polish & Final Features
**Goal:** Add the finishing touches to make it production-ready

**What you'll add:**
- Battery level indicator (top right corner)
- Unread message indicator (dot near battery)
- Send confirmation ("✓ SENT!" message)
- Error handling ("✗ FAILED" for radio errors)
- Button hints on screen (line 4)
- Message counter in inbox
- Unread counter on compose screen

**New functions added:**
- `getBatteryPercent()` - Read battery voltage
- `drawBatteryIcon()` - Display battery bars
- `showSendConfirmation()` - "SENT!" feedback
- `showSendError()` - Error screen
- Enhanced UI with all indicators

**UI Elements:**
- Battery: `[▮▮▮▮]` (4 levels)
- Unread: `●` (dot indicator)
- Button hints: "A:Inbox C:Del D:Send"
- Message counter: "3A [2/15]" (sender + position)

**Estimated Time:** 2-3 hours  
**Status:** ⏳ Code will be provided after Phase 5 complete

---

### Phase 7: Final Testing & Optimization
**Goal:** Test complete system and fix any issues

**Testing checklist:**
- [ ] Both devices power on correctly
- [ ] All 16 keys work on both devices
- [ ] Can type messages with T9 input
- [ ] Send message from Device A
- [ ] Receive message on Device B
- [ ] Reply from Device B to Device A
- [ ] Messages are encrypted (verify in Serial Monitor)
- [ ] Sender ID shows correctly
- [ ] Messages persist after power off/on
- [ ] Inbox navigation works (A key cycles messages)
- [ ] Delete messages works (C key in inbox)
- [ ] Battery indicator updates
- [ ] Unread indicator appears/disappears correctly
- [ ] Send confirmation shows
- [ ] Range test at various distances

**Estimated Time:** 2-4 hours  
**Status:** ⏳ After Phase 6 complete

---

## Total Project Time Estimate

- **Phase 1:** 0.5-1 hour (wire + test keypad)
- **Phase 2:** 2-3 hours (T9 + display)
- **Phase 3:** 3-4 hours (LoRa radio)
- **Phase 4:** 2-3 hours (encryption)
- **Phase 5:** 3-4 hours (message storage)
- **Phase 6:** 2-3 hours (polish)
- **Phase 7:** 2-4 hours (testing)

**Total: 15-22 hours** (2-4 days of focused work)

---

## Current Progress Tracker

```
✅ Project setup complete
✅ Phase 1 code written
⏳ Phase 1 hardware testing (YOU ARE HERE)
⬜ Phase 2 - T9 Input + Display
⬜ Phase 3 - LoRa Communication
⬜ Phase 4 - Encryption
⬜ Phase 5 - Message Storage
⬜ Phase 6 - Polish Features
⬜ Phase 7 - Final Testing
```

## Wiring Instructions

### Keypad to ESP32

**Keypad Layout:**
```
    Col1  Col2  Col3  Col4
Row1  1     2     3     A
Row2  4     5     6     B
Row3  7     8     9     C
Row4  *     0     #     D
```

**Pin Connections:**
```
Keypad Pin → ESP32 GPIO
─────────────────────────
Row 1      → GPIO 13
Row 2      → GPIO 12
Row 3      → GPIO 14
Row 4      → GPIO 27
Col 1      → GPIO 26
Col 2      → GPIO 25
Col 3      → GPIO 33
Col 4      → GPIO 32
```

### Power System

```
Battery+ (3.7V) ──→ [Power Switch] ──→ ESP32 BAT/VIN
Battery- (GND)  ─────────────────────→ ESP32 GND
```

**Important:** 
- The power switch completely disconnects the battery
- Zero drain when switched off
- Most ESP32 LoRa boards have a JST connector for LiPo batteries
- Check your board's pinout for battery connection points

### Antenna

- Screw the 915 MHz antenna onto the SMA connector
- **CRITICAL:** Never power on and transmit without the antenna connected (damages the radio)

## Getting Started - Phase 1

### 1. Install PlatformIO

If you don't have PlatformIO installed:
- Install [VS Code](https://code.visualstudio.com/)
- Install the PlatformIO IDE extension

### 2. Open Project

```bash
cd /Users/billymiller/Desktop/communicator
code .
```

### 3. Wire the Keypad

Follow the wiring diagram above. Use jumper wires for initial testing.

### 4. Upload Phase 1 Code

1. Connect ESP32 to computer via USB-C
2. In VS Code, click the PlatformIO icon
3. Click "Upload" (arrow button)
4. Wait for upload to complete

### 5. Test the Keypad

1. Open Serial Monitor (115200 baud)
2. Press each key on the keypad
3. Verify correct character appears in Serial Monitor
4. Check all 16 keys work correctly

**Troubleshooting:**
- If keys don't respond: Check wiring connections
- If wrong keys appear: Your keypad might have rows/columns swapped
- Use a multimeter to verify keypad contact continuity

### 6. Once All Keys Work

Let me know and I'll provide Phase 2 code (T9 input + OLED display).

## Key Mapping (Final Device)

### Text Input
- `2-9` = T9 text input (ABC, DEF, GHI, JKL, MNO, PQRS, TUV, WXYZ)
- `0` = Space
- `*` = Shift (capitals/symbols)
- `#` = Punctuation (.!?,;:)

### Navigation
- `A` = View inbox / Next message
- `B` = Back to compose
- `C` = Backspace / Delete message (in inbox)
- `D` = Send message

## Features

### Core Functionality
- ✅ Broadcast messaging to all devices in range
- ✅ AES-256 encryption (pre-shared key)
- ✅ Auto device ID (from ESP32 MAC address)
- ✅ Message history (50 messages, circular buffer)
- ✅ Persistent storage (survives power cycles)

### User Interface
- ✅ Dead simple - two screens only (Compose + Inbox)
- ✅ Battery level indicator
- ✅ Unread message indicator
- ✅ Message counter (X/Y format, newest first)
- ✅ Send confirmation feedback
- ✅ Error handling for failed sends
- ✅ Button hints always visible

### Design Philosophy
- Works when stressed, cold, tired, or panicked
- Understandable in under 10 seconds
- No menus, no hidden states
- Every button does exactly one thing

## Expected Performance

### Range (915 MHz, SF9, ~10 dBi antenna)
- Dense urban: 1-2 miles
- Suburban: 2-4 miles
- Open terrain: 5-10 miles
- Line of sight: 20+ miles

### Battery Life (~3000 mAh)
- Light usage: 5-10 days
- Moderate usage: 2-4 days
- Heavy usage: 1-2 days
- Power off: Infinite (zero drain with switch)

## Code Style

This code is written for **long-term maintainability**:
- ✅ Heavy comments explaining everything
- ✅ Simple C-style code (minimal C++ complexity)
- ✅ Clear function names and organization
- ✅ No fancy templates or modern C++ features
- ✅ Easy to understand years later

## License

This is an emergency communication device. Feel free to use, modify, and share.

## Safety Notes

- LoRa operates on 915 MHz ISM band (US) - legal for unlicensed use
- Max output power: 22 dBm (compliant with FCC regulations)
- Messages are encrypted but not guaranteed secure for classified information
- This is not a replacement for emergency services (911)
- Always carry backup communication methods

## Next Steps

**YOU ARE HERE:** Phase 1 - Keypad Testing

1. ✅ Wire the keypad
2. ✅ Upload Phase 1 code
3. ⏳ Test all 16 keys
4. ⏳ Confirm all keys work
5. ⏳ Request Phase 2 code

Once your keypad is working, let me know and we'll proceed to Phase 2!
