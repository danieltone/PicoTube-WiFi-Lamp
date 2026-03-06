#include <Adafruit_NeoPixel.h>

#if defined(ARDUINO_ARCH_RP2040)
#include <WiFi.h>
#include <WebServer.h>
#define ENABLE_WIFI_CONTROL 1
#else
#define ENABLE_WIFI_CONTROL 0
#endif

#ifndef LED_PIN
#define LED_PIN 6
#endif

#ifndef LED_COUNT
#define LED_COUNT 8
#endif

#ifndef BRIGHTNESS
#define BRIGHTNESS 40
#endif

#define AP_SSID "picoled"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#if ENABLE_WIFI_CONTROL
WebServer server(80);
#endif

enum Mode {
  MODE_MANUAL,
  MODE_RAINBOW,
  MODE_THEATER,
  MODE_BREATHE,
  MODE_ALT_CANDY,
  MODE_ALT_OCEAN,
  MODE_VACUUM_AMBER
};

Mode currentMode = MODE_RAINBOW;

uint8_t manualR = 255;
uint8_t manualG = 90;
uint8_t manualB = 0;

uint16_t rainbowIndex = 0;
int16_t breatheLevel = 0;
int8_t breatheDelta = 5;
int16_t amberLevel = 185;
int8_t amberDelta = 2;
uint8_t theaterOffset = 0;
bool altPhase = false;
unsigned long lastStepMs = 0;

uint8_t clampToByte(int value) {
  if (value < 0) return 0;
  if (value > 255) return 255;
  return (uint8_t)value;
}

uint32_t wheel(byte pos) {
  pos = 255 - pos;
  if (pos < 85) {
    return strip.Color(255 - pos * 3, 0, pos * 3);
  }
  if (pos < 170) {
    pos -= 85;
    return strip.Color(0, pos * 3, 255 - pos * 3);
  }
  pos -= 170;
  return strip.Color(pos * 3, 255 - pos * 3, 0);
}

void showSolid(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void allOff() {
  strip.clear();
  strip.show();
}

void runRainbowStep() {
  for (int i = 0; i < LED_COUNT; i++) {
    uint8_t hueIndex = (i * 256 / LED_COUNT + rainbowIndex) & 255;
    strip.setPixelColor(i, wheel(hueIndex));
  }
  strip.show();
  rainbowIndex++;
}

void runTheaterStep() {
  strip.clear();
  for (int i = theaterOffset; i < LED_COUNT; i += 3) {
    strip.setPixelColor(i, strip.Color(0, 180, 180));
  }
  strip.show();
  theaterOffset = (theaterOffset + 1) % 3;
}

void runBreatheStep() {
  strip.fill(strip.Color(breatheLevel, breatheLevel, breatheLevel), 0, LED_COUNT);
  strip.show();
  breatheLevel += breatheDelta;
  if (breatheLevel >= 255) {
    breatheLevel = 255;
    breatheDelta = -5;
  } else if (breatheLevel <= 0) {
    breatheLevel = 0;
    breatheDelta = 5;
  }
}

void runAlternatingStep(uint32_t evenColor, uint32_t oddColor) {
  uint32_t first = altPhase ? oddColor : evenColor;
  uint32_t second = altPhase ? evenColor : oddColor;
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, (i % 2 == 0) ? first : second);
  }
  strip.show();
  altPhase = !altPhase;
}

void runVacuumAmberStep() {
  uint8_t red = clampToByte(amberLevel);
  uint8_t green = clampToByte((amberLevel * 70) / 255);
  strip.fill(strip.Color(red, green, 0), 0, LED_COUNT);
  strip.show();

  amberLevel += amberDelta;
  if (amberLevel >= 230) {
    amberLevel = 230;
    amberDelta = -2;
  } else if (amberLevel <= 120) {
    amberLevel = 120;
    amberDelta = 2;
  }
}

void setMode(Mode nextMode) {
  currentMode = nextMode;
  lastStepMs = 0;
}

void applyManualColor() {
  setMode(MODE_MANUAL);
  showSolid(manualR, manualG, manualB);
}

void updateAnimation() {
  if (currentMode == MODE_MANUAL) {
    return;
  }

  const unsigned long now = millis();
  unsigned long intervalMs;

  if (currentMode == MODE_RAINBOW) {
    intervalMs = 24;
  } else if (currentMode == MODE_THEATER) {
    intervalMs = 120;
  } else if (currentMode == MODE_BREATHE) {
    intervalMs = 28;
  } else if (currentMode == MODE_VACUUM_AMBER) {
    intervalMs = 38;
  } else {
    intervalMs = 260;
  }

  if (lastStepMs != 0 && (now - lastStepMs) < intervalMs) {
    return;
  }
  lastStepMs = now;

  if (currentMode == MODE_RAINBOW) {
    runRainbowStep();
  } else if (currentMode == MODE_THEATER) {
    runTheaterStep();
  } else if (currentMode == MODE_BREATHE) {
    runBreatheStep();
  } else if (currentMode == MODE_ALT_CANDY) {
    runAlternatingStep(strip.Color(255, 30, 140), strip.Color(255, 130, 0));
  } else if (currentMode == MODE_ALT_OCEAN) {
    runAlternatingStep(strip.Color(0, 90, 255), strip.Color(0, 255, 180));
  } else if (currentMode == MODE_VACUUM_AMBER) {
    runVacuumAmberStep();
  }
}

#if ENABLE_WIFI_CONTROL
String htmlPage() {
  return R"HTML(
<!doctype html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>Pico LED Control</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 18px; background:#111827; color:#e5e7eb; }
    .card { background:#1f2937; border-radius:10px; padding:14px; margin-bottom:14px; }
    button, .palette-btn { padding:10px 12px; border-radius:8px; border:0; margin:4px; background:#2563eb; color:white; display:inline-block; text-decoration:none; }
    button.alt { background:#7c3aed; }
    button.stop { background:#dc2626; }
    input[type='range'] { width:100%; }
  </style>
</head>
<body>
  <h2>Pico LED (AP: picoled)</h2>
  <div class="card">
    <h3>Manual Color</h3>
    <p>Tap <b>Open Color Palette</b>, pick a color, then tap <b>Apply Manual Color</b>.</p>
    <label for="picker" class="palette-btn">Open Color Palette</label>
    <input id="picker" type="color" value="#ff5a00" style="width:100%; height:42px;" />
    <p>Brightness</p>
    <input id="bri" type="range" min="1" max="255" value="230" />
    <button onclick="setManual()">Apply Manual Color</button>
  </div>
  <div class="card">
    <h3>Colorful Presets</h3>
    <button onclick="setPreset('rainbow')">Rainbow Flow</button>
    <button onclick="setPreset('theater')">Theater Chase</button>
    <button onclick="setPreset('breathe')">Breathing White</button>
    <button onclick="setPreset('vacuum_amber')">Classic Vacuum Tube Amber</button>
    <button class="alt" onclick="setPreset('alt_candy')">Candy Alternating</button>
    <button class="alt" onclick="setPreset('alt_ocean')">Ocean Alternating</button>
    <button class="stop" onclick="allOff()">All Off</button>
  </div>
  <script>
    async function setManual() {
      const hex = document.getElementById('picker').value;
      const bri = document.getElementById('bri').value;
      const r = parseInt(hex.substr(1,2), 16);
      const g = parseInt(hex.substr(3,2), 16);
      const b = parseInt(hex.substr(5,2), 16);
      await fetch(`/set?r=${r}&g=${g}&b=${b}&br=${bri}`);
    }
    async function setPreset(name) {
      const bri = document.getElementById('bri').value;
      await fetch(`/preset?name=${name}&br=${bri}`);
    }
    async function allOff() {
      await fetch('/off');
    }
  </script>
</body>
</html>
  )HTML";
}

void updateBrightnessFromArgs() {
  if (server.hasArg("br")) {
    strip.setBrightness(clampToByte(server.arg("br").toInt()));
    strip.show();
  }
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleManualSet() {
  if (server.hasArg("r")) manualR = clampToByte(server.arg("r").toInt());
  if (server.hasArg("g")) manualG = clampToByte(server.arg("g").toInt());
  if (server.hasArg("b")) manualB = clampToByte(server.arg("b").toInt());
  updateBrightnessFromArgs();
  applyManualColor();
  server.send(200, "text/plain", "OK");
}

void handlePresetSet() {
  updateBrightnessFromArgs();
  String name = server.arg("name");

  if (name == "rainbow") {
    setMode(MODE_RAINBOW);
  } else if (name == "theater") {
    setMode(MODE_THEATER);
  } else if (name == "breathe") {
    setMode(MODE_BREATHE);
  } else if (name == "vacuum_amber") {
    setMode(MODE_VACUUM_AMBER);
  } else if (name == "alt_candy") {
    setMode(MODE_ALT_CANDY);
  } else if (name == "alt_ocean") {
    setMode(MODE_ALT_OCEAN);
  } else {
    server.send(400, "text/plain", "Unknown preset");
    return;
  }

  server.send(200, "text/plain", "OK");
}

void handleOff() {
  allOff();
  setMode(MODE_MANUAL);
  server.send(200, "text/plain", "OK");
}

void setupAccessPoint() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID);
  server.on("/", handleRoot);
  server.on("/set", handleManualSet);
  server.on("/preset", handlePresetSet);
  server.on("/off", handleOff);
  server.begin();
}
#endif

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  allOff();

#if ENABLE_WIFI_CONTROL
  setupAccessPoint();
#endif

  setMode(MODE_RAINBOW);
}

void loop() {
#if ENABLE_WIFI_CONTROL
  server.handleClient();
#endif
  updateAnimation();
}
