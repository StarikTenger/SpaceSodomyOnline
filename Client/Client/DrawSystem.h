#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include "Color.h"
#include "System.h"
#include "Camera.h"
#include "Mouse.h"
#include "Animation.h"
#include "Replay.h"

class DrawSystem {
public:
	sf::RenderWindow* window;
	Camera cam;
	System* system;
	Replay* replay;
	int mode;
	bool fullscreenMode = 0;


	Mouse mouse;
	double w, h;
	double blockSize = 1;

	std::vector<Animation> animations;
	std::vector<Animation> animationsInterface;

	DrawSystem();
	~DrawSystem();

	std::vector<Color> colorMatches = {
		Color(0, 151, 255),
		Color(50, 255, 81),
		Color(255, 238, 61),
		Color(200, 61, 255),
		Color(255, 61, 141),
		Color(255, 158, 61)
	};

	void draw();
	void drawScene();
	void drawInterface();
	sf::Font font;
	
//private:
	// Basics
	void animation(std::string img, AnimationState p1, AnimationState p2, double time);
	void animationInterface(std::string img, AnimationState p1, AnimationState p2, double time);
	Color fromHSV(double H, double S, double V);
	void fillRect(double x, double y, double width, double height, Color color);
	void fillCircle(double x, double y, double r, Color color);
	void strokeRect(double x, double y, double width, double height, Color color);
	void line(double x, double y, double x1, double y1, Color color);
	void image(std::string name, double x, double y, double width, double height, double dir);
	void image(std::string name, double x, double y, double width, double height, double dir, Color color);
	void image(std::string name, double x, double y, double width, double height, double dir, double d);
	void text(std::string text, double x, double y, int size, Color color);
	void text(std::string text, double x, double y, double size, double dir, Color color);

	// Objects
	void drawWalls();
	void beam(Vec2 pos, double dir, Color col);

	// Load
	std::map<std::string, sf::Texture*> textures;
	void loadTexture(std::string name, std::string path);
	void loadTextures();
};

