#include "Engine.h"



Engine::Engine()
{
}

Engine::Engine(double _mainForce, double _backForce, double _turnForce) {
	mainForce = _mainForce;
	backForce = _backForce;
	turnForce = _turnForce;
}

Engine::~Engine()
{
}
