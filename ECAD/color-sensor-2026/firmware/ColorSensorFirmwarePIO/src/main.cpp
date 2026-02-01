#include <Arduino.h>
#include <tinyNeoPixel.h>

#define PIXEL_PIN   PIN_PA3
#define SENSOR_PIN  PIN_PA1
#define TIME_GET_COLOR 30

tinyNeoPixel pixel = tinyNeoPixel(1, PIXEL_PIN, NEO_GRB);

static uint16_t avgRead(uint8_t n) {
  uint32_t s = 0;
  for (uint8_t i = 0; i < n; i++) s += analogRead(SENSOR_PIN);
  return (uint16_t)(s / n);
}

void setup() {
  pixel.begin();
  pixel.setBrightness(20);
  pixel.setPixelColor(0, 0);  // off
  pixel.show();

  pinMode(SENSOR_PIN, INPUT);
  analogReference(DEFAULT);
  analogRead(SENSOR_PIN);     // dummy

  Serial.begin(9600);
}

void loop() {
  uint16_t ambient, r, g, b;
  char c = 'u';

  // OFF (ambient)
  pixel.setPixelColor(0, 0);
  pixel.show();
  delay(TIME_GET_COLOR);
  ambient = avgRead(16);

  // R
  pixel.setPixelColor(0, 255, 0, 0);
  pixel.show();
  delay(TIME_GET_COLOR);
  r = avgRead(8);
  if (r > ambient) r -= ambient; else r = 0;

  // G
  pixel.setPixelColor(0, 0, 255, 0);
  pixel.show();
  delay(TIME_GET_COLOR);
  g = avgRead(8);
  if (g > ambient) g -= ambient; else g = 0;

  // B
  pixel.setPixelColor(0, 0, 0, 255);
  pixel.show();
  delay(TIME_GET_COLOR);
  b = avgRead(8);
  if (b > ambient) b -= ambient; else b = 0;

  // LED OFF
  pixel.setPixelColor(0, 0);
  pixel.show();

// Decision (dominante + jaune)
uint16_t rg_diff = (r > g) ? (r - g) : (g - r);

// seuils a ajuster empiriquement
if (r > b && g > b && rg_diff < 20) {
  c = 'y'; // yellow
}
else if (r > g && r > b) {
  c = 'r';
}
else if (g > r && g > b) {
  c = 'g';
}
else if (b > r && b > g) {
  c = 'b';
}
else {
  c = 'u';
}


  // Sortie ultra compacte: A R G B C
  Serial.print(ambient); Serial.print(' ');
  Serial.print(r);       Serial.print(' ');
  Serial.print(g);       Serial.print(' ');
  Serial.print(b);       Serial.print(' ');
  Serial.println(c);

  delay(200);
}
