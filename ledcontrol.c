#include <wiringPi.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define LED_PIN 1   // GPIO pin (WiringPi numbering)
#define PERIOD 1000 // 1 second PWM period divided into 1ms intervals

volatile int dutyCycle = 0;   // Duty cycle percentage (0–100)
volatile int counter = 0;     // counter

// PWM handler called every 1ms
void pwmHandler(int sig) {
    counter = (counter + 1) % PERIOD;

    if (counter < (dutyCycle * PERIOD / 100)) {
        digitalWrite(LED_PIN, HIGH); // ON for duty cycle
    } else {
        digitalWrite(LED_PIN, LOW);  // OFF otherwise
    }

    ualarm(1000, 0); // Reschedule next alarm in 1ms
}

int main() {
    char input;

    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        printf("Error: WiringPi setup failed!\n");
        return 1;
    }

    pinMode(LED_PIN, OUTPUT);

    // Setup signal handler
    signal(SIGALRM, pwmHandler);
    ualarm(1000, 0); // Start first 1ms timer

    printf("PWM LED Control Program\n");
    printf("Inputs: a=25%%, s=50%%, d=75%%, w=100%%, q=off, x=exit\n");

    // Loop for input handling
    while (1) {
        input = getchar();

        switch (input) {
            case 'a': dutyCycle = 25; printf("Duty Cycle = 25%%\n"); break;
            case 's': dutyCycle = 50; printf("Duty Cycle = 50%%\n"); break;
            case 'd': dutyCycle = 75; printf("Duty Cycle = 75%%\n"); break;
            case 'w': dutyCycle = 100; printf("Duty Cycle = 100%%\n"); break;
            case 'q': dutyCycle = 0; printf("LEDs OFF\n"); break;
            case 'x': 
                printf("Exiting program...\n"); 
                digitalWrite(LED_PIN, LOW);
                exit(0);
                break;
            case '\n': // ignore newlines
                break;
            default: 
                printf("Invalid input\n");
                break;
        }
    }

    return 0;
}