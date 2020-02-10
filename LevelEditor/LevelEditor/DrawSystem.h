#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include "Color.h"
#include "System.h"
#include "Camera.h"
#include "Mouse.h"
#include "System.h"
#include "Menu.h"
#include "Generator.h"

class DrawSystem{
public:
	sf::RenderWindow* window;
	Camera cam;
	System* system;
	Menu* menu;
	bool fullscreenMode = 0;
	DrawSystem();
	~DrawSystem();
	Mouse mouse;
	double w, h;
	double blockSize = 1;

	void draw();
	void drawScene();
	void drawInterface();
	sf::Font font;
	
	Vector2d getCursorPos();
//private:
	//basics
	void fillRect(double x, double y, double width, double height, Color color);
	void strokeRect(double x, double y, double width, double height, Color color);
	void image(std::string name, double x, double y, double width, double height, double angle);
	void image(std::string name, double x, double y, double width, double height, double angle, Color color);
	void image(std::string name, double x, double y, double width, double height, double angle, double d);
	void text(std::string text, double x, double y, int size, Color color);
	//objects
	void drawWalls();
	void drawTurret(Turret* s);
	void drawExit(Exit* s);
	void drawBonus(Bonus* s);
	void drawRocketLauncher(RocketLauncher* s);
	void drawLaserCarrier(LaserCarrier* s);
	void drawLaserCarrierBase(LaserCarrier* s);
	void drawRobot(Robot* s);
	void drawShip(Ship* s);
	void drawGenerator(Generator* s);
	//load
	std::map<std::string, sf::Texture*> textures;
	void loadTexture(std::string name, std::string path);
	void loadTextures();
};

