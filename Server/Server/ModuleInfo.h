#pragma once
class ModuleInfo {
public:
	double cooldownTime = 0.5;
	double energy = 0;
	double stamina = 0;

	ModuleInfo();
	ModuleInfo(double _cooldownTime, double _energy, double _stamina);
};
namespace ModuleParam {
	const double hookAccel = 5.;
	const double hookDistance = 20.;
	const double hookAngle = 0.3;
};
