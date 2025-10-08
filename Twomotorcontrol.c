#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>

// Adjustable safety stop delay (milliseconds)
#define STOP_DELAY_MS 300

// Motor A pins
#define IN1 0   // GPIO17
#define IN2 1   // GPIO18
#define ENA 2   // GPIO27 (PWM)

// Motor B pins
#define IN3 3   // GPIO22
#define IN4 4   // GPIO23
#define ENB 5   // GPIO24 (PWM)

// --- Utility Functions ---
void motor_stop() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    softPwmWrite(ENA, 0);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    softPwmWrite(ENB, 0);

    printf("Motors stopped\n");
    delay(STOP_DELAY_MS);  // adjustable safety delay
}

void motorA_forward(int speed) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    softPwmWrite(ENA, speed);
}

void motorA_backward(int speed) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    softPwmWrite(ENA, speed);
}

void motorB_forward(int speed) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    softPwmWrite(ENB, speed);
}

void motorB_backward(int speed) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    softPwmWrite(ENB, speed);
}

// --- Combined Moves (with safety stop) ---
void both_forward(int speed) {
    motor_stop();
    motorA_forward(speed);
    motorB_forward(speed);
    printf("Both motors forward at %d%% duty\n", speed);
}

void both_backward(int speed) {
    motor_stop();
    motorA_backward(speed);
    motorB_backward(speed);
    printf("Both motors backward at %d%% duty\n", speed);
}

void spin_left(int speed) {
    motor_stop();
    motorA_backward(speed);
    motorB_forward(speed);
    printf("Spin left\n");
}

void spin_right(int speed) {
    motor_stop();
    motorA_forward(speed);
    motorB_backward(speed);
    printf("Spin right\n");
}

// --- Main Program ---
int main() {
    wiringPiSetup();

    // Setup Motor A pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    softPwmCreate(ENA, 0, 100);

    // Setup Motor B pins
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    softPwmCreate(ENB, 0, 100);

    int running = 1;
    char cmd;

    printf("Dual Motor Control Program\n");
    printf("Commands:\n");
    printf(" f = forward, b = backward\n");
    printf(" l = spin left, r = spin right\n");
    printf(" s = stop, x = exit\n");

    while (running == 1) {
        printf("\nEnter command: ");
        scanf(" %c", &cmd);

        if (cmd == 'f') {
            both_forward(60);
        } else if (cmd == 'b') {
            both_backward(60);
        } else if (cmd == 'l') {
            spin_left(60);
        } else if (cmd == 'r') {
            spin_right(60);
        } else if (cmd == 's') {
            motor_stop();
        } else if (cmd == 'x') {
            motor_stop();
            printf("Exiting program...\n");
            running = 0;
        } else {
            printf("Invalid command!\n");
        }
    }

    return 0;
}