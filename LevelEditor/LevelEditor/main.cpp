#include "Control.h"

int main() {
	Control control;
	while (!control.stop) {
		control.step();
	}
}