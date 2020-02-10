#include "LaserCarrier.h"
#include "geometry.h"

LaserCarrier::LaserCarrier(){
}

LaserCarrier::LaserCarrier(int n) {
	std::vector<double> angles = geom::angleDistribution(0, 2*M_PI, n);
	for (auto a : angles) {
		Laser l;
		l.direction = a;
		lasers.push_back(l);
	}
}

LaserCarrier::~LaserCarrier(){
}
