// USB-Drehregler EC11 (CH552G) -> Mausrad hoch/runter, Knopf = Strg
// Arduino IDE + ch55xduino
//   Tools > Board:         CH552
//   Tools > USB Settings:  USER CODE w/ 148B USB ram
//   Tools > Clock Source:  16 MHz (internal) 3.3V or 5V
//   Tools > Upload method: USB

#ifndef USER_USB_RAM
#error "Tools > USB Settings auf 'USER CODE w/ 148B USB ram' stellen"
#endif

// USB-Code liegt im Sketch-Ordner (src/), kopiert aus dem Beispiel HidKeyboardMouse
#include "src/userUsbHidKeyboardMouse/USBHIDKeyboardMouse.h"

// Arduino-Pinnummer = Port*10 + Pin (ch55xduino)
#define PIN_ENC_A 31   // Gehaeuse-Pin 7  = P3.1
#define PIN_ENC_B 30   // Gehaeuse-Pin 8  = P3.0
#define PIN_SW    33   // Gehaeuse-Pin 10 = P3.3 (Taster gegen GND)
#define PIN_LED   34   // Gehaeuse-Pin 11 = P3.4 (LED zwischen Pin 15/VCC und Pin 11 -> LOW = an)

// Rastungen: EC11 hat meist 4 Flanken pro Rastung (manche 2). Bei doppelten
// oder fehlenden Schritten hier 2 bzw. 4 ausprobieren.
#define STEPS_PER_DETENT 4

// Drehrichtung umkehren: 1 = hoch/runter vertauschen
#define REVERSE_DIRECTION 0

#define LED_ON  LOW
#define LED_OFF HIGH

static const int8_t encTable[16] = {
   0, -1,  1,  0,
   1,  0,  0, -1,
  -1,  0,  0,  1,
   0,  1, -1,  0
};

uint8_t  encState = 0;
int8_t   encAcc = 0;
bool     swStable = false;
bool     swLast = false;
uint32_t swChanged = 0;
uint32_t ledOffAt = 0;

void setup() {
  USBInit();

  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
  pinMode(PIN_SW, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LED_OFF);

  encState = (digitalRead(PIN_ENC_A) << 1) | digitalRead(PIN_ENC_B);
}

void loop() {
  // --- Encoder ---
  uint8_t cur = (digitalRead(PIN_ENC_A) << 1) | digitalRead(PIN_ENC_B);
  if (cur != (encState & 0x03)) {
    encState = ((encState << 2) | cur) & 0x0F;
    encAcc += encTable[encState];

    int8_t dir = 0;
    if (encAcc >= STEPS_PER_DETENT)       { dir = 1;  encAcc = 0; }
    else if (encAcc <= -STEPS_PER_DETENT) { dir = -1; encAcc = 0; }

    if (dir != 0) {
#if REVERSE_DIRECTION
      dir = -dir;
#endif
      Mouse_scroll(dir);          // +1 = Mausrad hoch, -1 = runter
      // digitalWrite(PIN_LED, LED_OFF);
      ledOffAt = millis() + 30;   // LED blitzt kurz bei jeder Rastung
    }
  }

  if (ledOffAt && (int32_t)(millis() - ledOffAt) >= 0) {
    // digitalWrite(PIN_LED, LED_ON);
    ledOffAt = 0;
  }

  // --- Taster (entprellt) -> Strg (gehalten, solange gedrueckt) ---
  bool raw = (digitalRead(PIN_SW) == LOW);
  if (raw != swLast) {
    swLast = raw;
    swChanged = millis();
  }
  if (raw != swStable && (millis() - swChanged) > 15) {
    swStable = raw;
    if (swStable) Keyboard_press(KEY_LEFT_CTRL);
    else          Keyboard_release(KEY_LEFT_CTRL);
  }
}
