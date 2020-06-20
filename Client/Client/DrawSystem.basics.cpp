#include "DrawSystem.h"
#include <math.h>
#include <algorithm>

#include <iostream>

Color DrawSystem::fromHSV(double H, double S, double V) {
	double C = S * V;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double Rs, Gs, Bs;

	if (H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if (H >= 60 && H < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if (H >= 120 && H < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if (H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if (H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	Color color;
	color.r = (Rs + m) * 255;
	color.g = (Gs + m) * 255;
	color.b = (Bs + m) * 255;
	return color;
}

void DrawSystem::animation(std::string img, AnimationState p1, AnimationState p2, double time) {
	animations.push_back(
		Animation(
			img, p1, p2, system->time, system->time + time
		)
	);
}

void DrawSystem::animationInterface(std::string img, AnimationState p1, AnimationState p2, double time) {
	animationsInterface.push_back(
		Animation(
			img, p1, p2, system->time, system->time + time
		)
	);
}

void DrawSystem::fillRect(double x, double y, double width, double height, Color color) {
	sf::RectangleShape rectangle;
	rectangle.setOrigin(width / 2, height / 2);
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	rectangle.setPosition(x, y);
	window->draw(rectangle);
}

void DrawSystem::strokeRect(double x, double y, double width, double height, Color color) {
	sf::RectangleShape rectangle;
	rectangle.setOrigin(width / 2, height / 2);
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
	rectangle.setFillColor(sf::Color(0, 0, 0, 0));
	rectangle.setOutlineThickness(1);
	rectangle.setPosition(x, y);
	window->draw(rectangle);
}

void DrawSystem::fillCircle(double x, double y, double r, Color color) {
	sf::CircleShape circle;
	circle.setOrigin(r, r);
	circle.setRadius(r);
	circle.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	circle.setPosition(x, y);
	window->draw(circle);
}

void DrawSystem::line(double x, double y, double x1, double y1, Color color) {
	sf::Vertex l[] =
	{
		sf::Vertex(sf::Vector2f(x, y)),
		sf::Vertex(sf::Vector2f(x1, y1))
	};
	l[0].color = l[1].color = sf::Color(color.r, color.g, color.b, color.a);
	window->draw(l, 2, sf::Lines);
}

void DrawSystem::image(std::string name, double x, double y, double width, double height, double dir, Color color) {
	sf::Texture& tex = *textures[name];
	Vec2 scale = { width / tex.getSize().x  , height / tex.getSize().y };
	Vec2 pos = { x, y };// cam.transform({ x, y });
	sf::Sprite sprite;
	sprite.setOrigin(tex.getSize().x / 2, tex.getSize().y / 2);
	sprite.setTexture(tex);
	sprite.setScale(scale.x, scale.y);
	sprite.setPosition(x, y);
	sprite.setColor(sf::Color(std::min(255, (int)color.r), std::min(255, (int)color.g), std::min(255, (int)color.b), std::min(255, (int)color.a)));
	sprite.setRotation(dir * 180 / M_PI);
	window->draw(sprite);
}

void DrawSystem::image(std::string name, double x, double y, double width, double height, double dir, Color color, Vec2 a, Vec2 b) {
	sf::Texture& tex = *textures[name];
	Vec2 scale = { width / tex.getSize().x  , height / tex.getSize().y };
	Vec2 pos = { x, y };// cam.transform({ x, y });
	sf::Sprite sprite;
	sprite.setOrigin(tex.getSize().x / 2, tex.getSize().y / 2);
	sprite.setTexture(tex);
	sprite.setScale(scale.x, scale.y);
	a.x *= tex.getSize().x;
	a.y *= tex.getSize().y;
	b.x *= tex.getSize().x;
	b.y *= tex.getSize().y;
	sprite.setTextureRect(sf::IntRect(a.x, a.y, b.x, b.y));
	sprite.setPosition(x, y);
	sprite.setColor(sf::Color(std::min(255, (int)color.r), std::min(255, (int)color.g), std::min(255, (int)color.b), std::min(255, (int)color.a)));
	sprite.setRotation(dir * 180 / M_PI);
	window->draw(sprite);
}

void DrawSystem::image(std::string name, double x, double y, double width, double height, double dir) {
	image(name, x, y, width, height, dir, Color(255, 255, 255, 255));
}

void DrawSystem::image(std::string name, double x, double y, double width, double height, double dir, double d) {
	Vec2 pos(x, y);
	pos = cam.pos + (pos - cam.pos) * d;
	image(name, pos.x, pos.y, width * d, height * d, dir);
}

void DrawSystem::text(std::string text, double x, double y, int size, Color color) {
	sf::Text drawnText;
	drawnText.setFont(font);
	drawnText.setString(text);
	drawnText.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	drawnText.setCharacterSize(size);
	drawnText.setOrigin(floor(drawnText.getLocalBounds().width / 2), floor(drawnText.getLocalBounds().height));
	drawnText.setPosition(x, y);
	window->draw(drawnText);
}

void DrawSystem::text(std::string text, double x, double y, double size, double dir, Color color) {
	sf::Text drawnText;
	drawnText.setFont(font);
	drawnText.setString(text);
	drawnText.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	drawnText.setCharacterSize(60);
	drawnText.setScale(size / 2, size);
	drawnText.setOrigin(floor(drawnText.getLocalBounds().width / 2), floor(drawnText.getLocalBounds().height));
	drawnText.setPosition(x, y);
	drawnText.setRotation(dir * 180 / M_PI);
	window->draw(drawnText);
}

// basics using Vec2

void DrawSystem::fillRect(Vec2 pos, Vec2 box, Color color) {
	fillRect(pos.x, pos.y, box.x, box.y, color);
}
void DrawSystem::fillCircle(Vec2 pos, double r, Color color) {
	fillCircle(pos.x, pos.y, r, color);
}
void DrawSystem::strokeRect(Vec2 pos, Vec2 box, Color color) {
	strokeRect(pos.x, pos.y, box.x, box.y, color);
}
void DrawSystem::line(Vec2 pos, Vec2 pos1, Color color) {
	line(pos.x, pos.y, pos1.x, pos1.y, color);
}
void DrawSystem::image(std::string name, Vec2 pos, Vec2 box, double angle) {
	image(name, pos.x, pos.y, box.x, box.y, angle);
}
void DrawSystem::image(std::string name, Vec2 pos, Vec2 box, double angle, Color color) {
	image(name, pos.x, pos.y, box.x, box.y, angle, color);
}
void DrawSystem::text(std::string str, Vec2 pos, int size, Color color) {
	text(str, pos.x, pos.y, size, color);
}

void DrawSystem::image(std::string name, Vec2 pos, Vec2 box, double angle, Color color, Vec2 a, Vec2 b) {
	image(name, pos.x, pos.y, box.x, box.y, angle, color, a, b);
}