#include "Control.h"

int main(int, char**){
	random::start();
	
	Control control;

	while (!control.stop) {
		control.step();
		
	}
	return 0;
}