#include "Gamepad.h"

Gamepad::Gamepad() {
}

Gamepad::~Gamepad() {
}

int& Gamepad::get(int button) {
	switch (button) {
	case 0:
		return button1;
	case 1:
		return button2;
	case 2:
		return button3;
	case 3:
		return button4;
	case 4:
		return leftUp;
	case 5:
		return rightUp;
	case 6:
		return leftDown;
	case 7:
		return rightDown;
	case 8:
		return button9;
	case 9:
		return button10;
	}
}
