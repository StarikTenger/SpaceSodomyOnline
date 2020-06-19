#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum Commands {
	NONE,
	MOVE_FORWARD, 
	MOVE_BACKWARD, 
	MOVE_FORWARD1,
	MOVE_BACKWARD1,
	MOVE_LEFT, 
	MOVE_RIGHT, 
	TURN_LEFT, 
	TURN_RIGHT, 
	SHOOT, 
	ZOOM_IN, 
	ZOOM_OUT, 
	STABILIZE_ROTATION, 
	STABILIZE_ROTATION_AUTO, 
	ACTIVATE,
	MODULE_1,
	MODULE_2,
	MODULE_3,
	MODULE_4,
	MODULE_5,
	RESTART, 
	EXIT,
	/////////////////////////////////////////
	RIGHT,
	LEFT,
	DOWN,
	UP,
	SPACE,
	SHIFT,
	ESCAPE,
	ALT,
	Q, W, E, R, T, Y, U, I, O, P, A, S, D, F, G, H, J, K, L, Z, X, C, V, B, N, M,
	NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0,
};


inline Commands getKey(sf::Keyboard::Key key) {
	switch (key) {
	case sf::Keyboard::Right:
		return RIGHT;
	case sf::Keyboard::Left:
		return LEFT;
	case sf::Keyboard::Down:
		return DOWN;
	case sf::Keyboard::Up:
		return UP;
	case sf::Keyboard::Space:
		return SPACE;
	case sf::Keyboard::LShift:
		return SHIFT;
	case sf::Keyboard::Escape:
		return ESCAPE;
	case sf::Keyboard::LAlt:
		return ALT;

	case sf::Keyboard::Q:
		return Q;
	case sf::Keyboard::W:
		return W;
	case sf::Keyboard::E:
		return E;
	case sf::Keyboard::R:
		return R;
	case sf::Keyboard::T:
		return T;
	case sf::Keyboard::Y:
		return Y;
	case sf::Keyboard::U:
		return U;
	case sf::Keyboard::I:
		return I;
	case sf::Keyboard::O:
		return O;
	case sf::Keyboard::P:
		return P;
	case sf::Keyboard::A:
		return A;
	case sf::Keyboard::S:
		return S;
	case sf::Keyboard::D:
		return D;
	case sf::Keyboard::F:
		return F;
	case sf::Keyboard::G:
		return G;
	case sf::Keyboard::H:
		return H;
	case sf::Keyboard::J:
		return J;
	case sf::Keyboard::K:
		return K;
	case sf::Keyboard::L:
		return L;
	case sf::Keyboard::Z:
		return Z;
	case sf::Keyboard::X:
		return X;
	case sf::Keyboard::C:
		return C;
	case sf::Keyboard::V:
		return V;
	case sf::Keyboard::B:
		return B;
	case sf::Keyboard::N:
		return N;
	case sf::Keyboard::M:
		return M;
	case sf::Keyboard::Num1:
		return NUM1;
	case sf::Keyboard::Num2:
		return NUM2;
	case sf::Keyboard::Num3:
		return NUM3;
	case sf::Keyboard::Num4:
		return NUM4;
	case sf::Keyboard::Num5:
		return NUM5;
	case sf::Keyboard::Num6:
		return NUM6;
	case sf::Keyboard::Num7:
		return NUM7;
	case sf::Keyboard::Num8:
		return NUM8;
	case sf::Keyboard::Num9:
		return NUM9;
	case sf::Keyboard::Num0:
		return NUM0;
	}
	return NONE;
}

inline Commands getKey(std::string s) {
	if(s == "RIGHT")
		return RIGHT;
	if (s == "LEFT")
		return LEFT;
	if (s == "DOWN")
		return DOWN;
	if (s == "UP")
		return UP;
	if (s == "SPACE")
		return SPACE;
	if (s == "SHIFT")
		return SHIFT;
	if (s == "ESCAPE")
		return ESCAPE;
	if (s == "ALT")
		return ALT;

	if (s == "Q")
		return Q;
	if (s == "W")
		return W;
	if (s == "E")
		return E;
	if (s == "R")
		return R;
	if (s == "T")
		return T;
	if (s == "Y")
		return Y;
	if (s == "U")
		return U;
	if (s == "I")
		return I;
	if (s == "O")
		return O;
	if (s == "P")
		return P;
	if (s == "A")
		return A;
	if (s == "S")
		return S;
	if (s == "D")
		return D;
	if (s == "F")
		return F;
	if (s == "G")
		return G;
	if (s == "H")
		return H;
	if (s == "J")
		return J;
	if (s == "K")
		return K;
	if (s == "L")
		return L;
	if (s == "Z")
		return Z;
	if (s == "X")
		return X;
	if (s == "C")
		return C;
	if (s == "V")
		return V;
	if (s == "NUM1")
		return NUM1;
	if (s == "NUM2")
		return NUM2;
	if (s == "NUM3")
		return NUM3;
	if (s == "NUM4")
		return NUM4;
	if (s == "NUM5")
		return NUM5;
	if (s == "NUM6")
		return NUM6;
	if (s == "NUM7")
		return NUM7;
	if (s == "NUM8")
		return NUM8;
	if (s == "NUM9")
		return NUM9;
	if (s == "NUM0")
		return NUM0;

	if (s == "MOVE_FORWARD")
		return MOVE_FORWARD;
	if (s == "MOVE_BACKWARD")
		return MOVE_BACKWARD;
	if (s == "MOVE_FORWARD1")
		return MOVE_FORWARD1;
	if (s == "MOVE_BACKWARD1")
		return MOVE_BACKWARD1;
	if (s == "MOVE_LEFT")
		return MOVE_LEFT;
	if (s == "MOVE_RIGHT")
		return MOVE_RIGHT;
	if (s == "TURN_LEFT")
		return TURN_LEFT;
	if (s == "TURN_RIGHT")
		return TURN_RIGHT;
	if (s == "SHOOT")
		return SHOOT;
	if (s == "ZOOM_IN")
		return ZOOM_IN;
	if (s == "ZOOM_OUT")
		return ZOOM_OUT;
	if (s == "STABILIZE_ROTATION")
		return STABILIZE_ROTATION;
	if (s == "STABILIZE_ROTATION_AUTO")
		return STABILIZE_ROTATION_AUTO;
	if (s == "ACTIVATE")
		return ACTIVATE;
	if (s == "MODULE_1")
		return MODULE_1;
	if (s == "MODULE_2")
		return MODULE_2;
	if (s == "MODULE_3")
		return MODULE_1;
	if (s == "MODULE_4")
		return MODULE_1;
	if (s == "MODULE_5")
		return MODULE_1;
	if (s == "ACTIVATE")
		return ACTIVATE;
	if (s == "RESTART")
		return RESTART;
	if (s == "EXIT")
		return EXIT;
	return NONE;
}

