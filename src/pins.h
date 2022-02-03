#define PRODUCTION_PCB

#ifdef PRODUCTION_PCB
    #define LED_DATA_PIN 27
    #define BUZZER_PIN 25
    #define MODE_PIN 34
#else
    #define LED_DATA_PIN 27
    #define BUZZER_PIN 4
    #define MODE_PIN 4
#endif
