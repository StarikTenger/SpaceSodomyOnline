#include "Control.h"

int main(){
	random::start();
	
	Control control;

	while (!control.stop) {
		control.step();
		
	}
	return 0;
}