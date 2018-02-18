// The library "Adafruit NeoPixel by Adafruit" needs to be installed.
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

// The LED strip is wired to the D1 pin.
#define LED D1

// The sensor is wired to the A0 analogue pin.
#define SENSOR A0

// If set to true, debug logs are written to the serial port.
#define DEBUG false

// Initialising the the NeoPixel strip library with the model we have
// installed.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, LED, NEO_GRBW + NEO_KHZ800);

// The parameter holds the current state of the LED strip.
bool isOn = false;

// The parameter holds the time of the last sensor reading.
// We use this and the sensorReadingThreshold to define how often take readings
// of the sensor.
uint32_t lastSensorReading = millis();

// The minimum time to pass in milliseconds before taking another reading 
// on the proximty sensor.
uint32_t sensorReadingThreshold = 2000;

void setup() {

  // Init serial port for debugging.
  if DEBUG == true {
    Serial.begin(9600);
  }
 
  // Init IR Sensor PIN
  pinMode(SENSOR, INPUT);

  // Init Strip
  strip.begin();
  
  // Initialize all pixels to 'off'
  strip.show(); 
}

void loop() {

  // The analogue sensor reading
  float reading = analogRead(SENSOR);

//  // These can be used to calculate the distance of the hand to the sensor
//  // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
//  float volts = analogRead(SENSOR)*0.0048828125;   
//  // worked out from graph 65 = theretical distance / (1/Volts)
//  float distance = 65*pow(volts, -1.10);          

  if DEBUG == true {
    Serial.println(reading);
  }
  
  if (reading == 0) {
    uint32_t currentTime = millis();
    uint32_t delta = currentTime - lastSensorReading;

    if DEBUG == true {
      Serial.println(delta);
      Serial.println(reading);
    }

    // Dismiss the reading if last reading was within the treshold.
    if (delta < sensorReadingThreshold) {
      return;
    }

    if (isOn) {
      turnOff();
    } else {
      lightUp();
    }

    isOn = !isOn;
    lastSensorReading = currentTime;
  }
}

// Called to turn on the strip. 
// We only address the WHITE pixel.
void lightUp() {
  for (uint16_t i=0; i<strip.numPixels(); i++) {
//    // Only turning on every second pixel to reduce brightness
//    if (i % 2) {
//      continue;
//    }
    strip.setPixelColor(i, strip.Color(0, 0, 0, 255));
    strip.show();
  }
}

// Called to turn off the strip.
void turnOff() {
  for (uint16_t i=0; i<strip.numPixels(); i++) {
//    // Only turning on every second pixel to reduce brightness
//    if (i % 2) {
//      continue;
//    }
    strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    strip.show();
  }
}
