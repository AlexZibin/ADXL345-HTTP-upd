#include "ArduinoLog.h"
// #include "build_defs.h"
#include "ADXL.h"
#include "pins.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>


#include <FastLED.h>
#define NUM_LEDS 2
CRGB leds[NUM_LEDS];


/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// SDA/21	    SDA (синий)
// SCL/22	    SCL (фиолетовый)

void displaySensorDetails(void) {
    sensor_t sensor;
    accel.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
    Serial.println("------------------------------------");
    Serial.println("");
    delay(5);
}

void displayDataRate(void) {
  Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  Serial.println(" Hz");  
}

void displayRange (void) {
    Serial.print ("Range:         +/- "); 
    
    switch(accel.getRange()) {
        case ADXL345_RANGE_16_G:
        Serial.print  ("16 "); 
        break;
        case ADXL345_RANGE_8_G:
        Serial.print  ("8 "); 
        break;
        case ADXL345_RANGE_4_G:
        Serial.print  ("4 "); 
        break;
        case ADXL345_RANGE_2_G:
        Serial.print  ("2 "); 
        break;
        default:
        Serial.print  ("?? "); 
        break;
    }  
    Serial.println(" g");  
}

int scanI2C () {
    byte error, address;
    int nDevices;
    
    Serial.println ("Scanning I2C bus...");
    
    nDevices = 0;
    for (address = 1; address < 127; address++ ) {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
    
        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.print(address,HEX);
            Serial.println("  !");
        
            nDevices++;
        }
        else if (error==4) {
            Serial.print("Unknown error at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.println(address,HEX);
        }    
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
    
    // delay(5000);           // wait 5 seconds for next scan
    return nDevices;
}

void fastledBlink (int on) {
    static CRGB::HTMLColorCode color = CRGB::Blue;

    if (on) {
        color = (color == CRGB::Blue ? CRGB::DarkGreen : CRGB::Blue);
        leds[1] = color;
        FastLED.show ();
    } else {
        leds[1] = CRGB::Black;
        FastLED.show ();
    }
}

void setupADXL (void) {
    // Serial.begin (115200);
    Serial.println ("Accelerometer Test"); Serial.println("");
    
    // With CS tied high to VDD I/O, the ADXL345 is in I2C mode
    // pinMode (18, OUTPUT);
    // digitalWrite (18, HIGH);

    FastLED.addLeds<WS2812, LED_DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
    leds[0] = leds[1] = CRGB::Black;
    FastLED.setBrightness (255);
    FastLED.show();
    
    pinMode (BUZZER_PIN, OUTPUT);
    digitalWrite (BUZZER_PIN, LOW);

    // Wire.begin (22, 21);
    Wire.begin ();

    int nDevices = scanI2C ();
    for (int i = 8; i; i--) {
        leds[1] = nDevices > 1 ? CRGB::Green : CRGB::Red;
        FastLED.show ();
        Serial.println (i);

        if (i > 5)
            digitalWrite (BUZZER_PIN, HIGH);

        delay (500);
        leds[1] = CRGB::Black;
        FastLED.show ();
        digitalWrite (BUZZER_PIN, LOW);

        delay (500);
    }
    
    // With the ALT ADDRESS pin
    // high, the 7-bit I2C address for the device is 0x1D, followed by
    // the R/W bit. This translates to 0x3A for a write and 0x3B for a
    // read. An alternate I2C address of 0x53 (followed by the R/W bit)
    // can be chosen by grounding the ALT ADDRESS pin (Pin 12).
    // This translates to 0xA6 for a write and 0xA7 for a read.
    // #define ADXL345_DEFAULT_ADDRESS (0x53) ///< Assumes ALT address pin low

    if (!accel.begin (0x1D)) {
        /* There was a problem detecting the ADXL345 ... check your connections */
        Serial.println ("Ooops, no ADXL345 detected ... Check your wiring!");
        leds[1] = CRGB::Blue;
        FastLED.show ();
        delay (1500);
        // leds[1] = CRGB::Black;
        // FastLED.show ();
        // while (1);
    } else {
        leds[1] = CRGB::Green;
        FastLED.show ();
    }

    /* Set the range to whatever is appropriate for your project */
    accel.setRange(ADXL345_RANGE_16_G);
    // accel.setRange(ADXL345_RANGE_8_G);
    // accel.setRange(ADXL345_RANGE_4_G);
    // accel.setRange(ADXL345_RANGE_2_G);
    
    /* Display some basic information on this sensor */
    displaySensorDetails ();
    
    /* Display additional settings (outside the scope of sensor_t) */
    displayDataRate ();
    displayRange ();
    Serial.println ("");
}

void loopADXL (void) {
    /* Get a new sensor event */ 
    sensors_event_t event; 
    accel.getEvent(&event);
    
    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

    if (
            event.acceleration.x > 5
                &&
            event.acceleration.y > -3
                &&
            event.acceleration.y < 3
                &&
            event.acceleration.z > 1
                &&
            event.acceleration.z < 5
        ) {
        // digitalWrite (BUZZER_PIN, HIGH);
        // leds[0] = CRGB::Red;
        leds[1] = CRGB::Red;
        // leds[1] = CRGB::Green;
        FastLED.show ();
    } else {
        digitalWrite (BUZZER_PIN, LOW);
        // leds[0] = CRGB::Blue;
        leds[1] = CRGB::Green;
        FastLED.show ();
    }

    delay (200);
}