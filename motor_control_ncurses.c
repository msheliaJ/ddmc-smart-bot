#include <wiringPi.h>
#include <softPwm.h>
#include <ncurses.h>
#include <ctype.h>
#include <stdio.h>

#define STOP_DELAY_MS 300
#define SPEED1 50
#define SPEED2 90

#define IN1 0
#define IN2 1
#define ENA 2
#define IN3 3
#define IN4 4
#define ENB 5

typedef enum { DIR_STOP=0, DIR_FWD, DIR_BWD, DIR_LEFT, DIR_RIGHT } direction_t;

static int current_speed = SPEED1;
static direction_t current_dir = DIR_STOP;

void motor_stop(void) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    softPwmWrite(ENA, 0);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    softPwmWrite(ENB, 0);
    napms(STOP_DELAY_MS);
}

void motorA_forward(int speed){ digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);  softPwmWrite(ENA,speed); }
void motorA_backward(int speed){digitalWrite(IN1,LOW);  digitalWrite(IN2,HIGH); softPwmWrite(ENA,speed); }
void motorB_forward(int speed){ digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);  softPwmWrite(ENB,speed); }
void motorB_backward(int speed){digitalWrite(IN3,LOW);  digitalWrite(IN4,HIGH); softPwmWrite(ENB,speed); }

void both_forward(int speed){  motor_stop(); motorA_forward(speed); motorB_forward(speed);  current_dir=DIR_FWD;  }
void both_backward(int speed){ motor_stop(); motorA_backward(speed); motorB_backward(speed); current_dir=DIR_BWD;  }
void spin_left(int speed){     motor_stop(); motorA_backward(speed); motorB_forward(speed); current_dir=DIR_LEFT; }
void spin_right(int speed){    motor_stop(); motorA_forward(speed);  motorB_backward(speed); current_dir=DIR_RIGHT;}

void apply_speed_if_moving(void){
    if (current_dir != DIR_STOP){
        softPwmWrite(ENA, current_speed);
        softPwmWrite(ENB, current_speed);
    }
}

void draw_header(void){
    mvprintw(0, 0, "Raspberry Pi Robot Control (Ncurses, BLOCKING input)");
    mvhline(1, 0, 0, COLS);
}

const char* dir_text(direction_t d){
    switch(d){
        case DIR_FWD:  return "Forward";
        case DIR_BWD:  return "Backward";
        case DIR_LEFT: return "Left (spin)";
        case DIR_RIGHT:return "Right (spin)";
        default:       return "Stopped";
    }
}

void draw_controls(void){
    mvprintw(3, 0, "Controls (press key, program waits for input each time):");
    mvprintw(4,  2, "w = forward   | s = backward");
    mvprintw(5,  2, "a = left      | d = right");
    mvprintw(6,  2, "x = stop      | SPACE or ESC = exit");
    mvprintw(7,  2, "1 = speed 50%% | 2 = speed 90%%");
    mvprintw(9,  0, "Press 'h' to reprint controls.");
}

void draw_status(const char* info){
    mvprintw(11, 0, "Status:");
    mvprintw(12, 2, "Direction: %s", dir_text(current_dir));
    mvprintw(13, 2, "Speed: %d%%", current_speed);
    if (info && *info){
        mvprintw(15, 0, "%s", info);
    } else {
        mvprintw(15, 0, "%-*s", COLS-1, "");
    }
    refresh();
}

int main(void){
    if (wiringPiSetup() == -1){
        fprintf(stderr, "wiringPi setup failed\n");
        return 1;
    }
    pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
    if (softPwmCreate(ENA, 0, 100) != 0){ fprintf(stderr,"softPwm ENA fail\n"); return 1; }
    if (softPwmCreate(ENB, 0, 100) != 0){ fprintf(stderr,"softPwm ENB fail\n"); return 1; }
    motor_stop();
    current_dir = DIR_STOP;
    current_speed = SPEED1;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    clear();
    draw_header();
    draw_controls();
    draw_status("Ready.");

    int running = 1;
    while (running){
        int ch = getch();
        int redraw_controls = 0;
        const char* message = "";

        if (ch >= 0 && ch <= 255) ch = tolower(ch);
        if (ch == 27 || ch == ' '){
            running = 0;
            break;
        }

        switch (ch){
            case 'w':
            case KEY_UP:
                both_forward(current_speed);
                message = "Moving forward.";
                break;
            case 's':
            case KEY_DOWN:
                both_backward(current_speed);
                message = "Moving backward.";
                break;
            case 'a':
            case KEY_LEFT:
                spin_left(current_speed);
                message = "Turning left.";
                break;
            case 'd':
            case KEY_RIGHT:
                spin_right(current_speed);
                message = "Turning right.";
                break;
            case 'x':
                motor_stop();
                current_dir = DIR_STOP;
                message = "Motors stopped.";
                break;
            case '1':
                current_speed = SPEED1;
                apply_speed_if_moving();
                message = "Speed set to 50%% (SLOW).";
                break;
            case '2':
                current_speed = SPEED2;
                apply_speed_if_moving();
                message = "Speed set to 90%% (FAST).";
                break;
            case 'h':
                clear();
                draw_header();
                draw_controls();
                redraw_controls = 1;
                message = "Controls refreshed.";
                break;
            default:
                message = "Invalid input — refer to controls menu (press 'h').";
                break;
        }
        if (redraw_controls == 0){
            draw_header();
            draw_controls();
        }
        draw_status(message);
    }
    motor_stop();
    endwin();
    return 0;
}
