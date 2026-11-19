CC      = gcc
CFLAGS  = -Wall -O2
LDLIBS  = -lwiringPi -lncurses -lpthread
TARGET  = robot

$(TARGET): src/motor_control_with_sensors.c
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

run: $(TARGET)
	sudo ./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean
