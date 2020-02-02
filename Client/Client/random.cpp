#include "random.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
void random::start() {
	std::srand(std::time(nullptr));
	return;
}
int random::intRandom(int min, int max) {
	return std::rand() % (max + 1 - min) + min;
}
double random::floatRandom(double min, double max, double sign) {
	double k = pow(10, sign);
	return intRandom(int(min*k), int(max*k))/k;
}