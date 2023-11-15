#include <Arduino.h>

typedef enum {
	sleeping,
	welcome,
	proceeding,
	entered,
	washing,
	exiting,
	leaved
} washState;

washState state;

void setup() {
	state = sleeping;
}

void loop() {
	switch (state) {
		case sleeping:
			// sleep();
			break;
		case welcome:
			// leds->turnOn(1);
			// lcd->print("Welcome");
			break;
		case proceeding:
			// gate->open();
			// led->blink(1);
			// lcd->print("Proceed to the Wasing Area");
			break;
		case entered:
			// gate->close();
			// leds->turnOn(2);
			// lcd->print("Ready to Wash");
			break;
		case washing:
			// leds->blink(2, 0.5);
			// lcd->dispCountDown(countdown);
			break;
		case exiting:
			// leds->turnOff(2);
			// leds->turnOn(3);
			// lcd->print("Washing complete, you can leave the area");
			// gate->open();
			break;
		case leaved:
			// gate->close();
			// leds->turnOff(3);
			break;
	}
}