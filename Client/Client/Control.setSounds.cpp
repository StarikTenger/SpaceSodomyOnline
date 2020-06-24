#include "Control.h"
#include <iostream>

void Control::setSounds() {
	for (auto& object : sys.objects)
		if (object.type == Object::SHIP)
			sys.players[object.id].object = &object;

	for (auto& object : sysPrev.objects)
		if (object.type == Object::SHIP)
			sysPrev.players[object.id].object = &object;

	//std::cout << " Q";
	if (!sysPrev.players.size())
		return;
	for (auto& p : sys.players) {
		int id = p.first;
		Player& player = p.second;
		Player& playerPrev = sysPrev.players[id];
		

		Object* object = p.second.object;
		Object* objectPrev = playerPrev.object;

		if (objectPrev && player.deaths > playerPrev.deaths) {
			std::cout << "death\n";
			audio.play("death", objectPrev->pos, 100, drawSys.cam);

			drawSys.animation("shipAura",
				AnimationState(objectPrev->pos, Vec2(objectPrev->r * 2, objectPrev->r * 2), objectPrev->dir, { 255, 255, 255 }),
				AnimationState(objectPrev->pos + geom::direction(random::floatRandom(0, M_PI * 2, 2)) * 0.05, Vec2(objectPrev->r * 2, objectPrev->r * 2) * 1.5, objectPrev->dir, { 255, 255, 255, 0 }),
				0.2);
		}
		if (object && objectPrev && object->hp < objectPrev->hp) {
			std::cout << "knock\n";
			audio.play("knock", object->pos, 100, drawSys.cam);
		}

		if (!objectPrev || !object || geom::distance({ 0, 0 }, object->pos) > 1e5)
			continue;

		int bulletsPrev = 0;
		for (const auto& o : sysPrev.objects)
			if (o.type == Object::BULLET && o.id == id && geom::distance(o.pos, objectPrev->pos) < 1)
				bulletsPrev++;
		int bullets = 0;
		for (const auto& o : sys.objects)
			if (o.type == Object::BULLET && o.id == id && geom::distance(o.pos, object->pos) < 1)
				bullets++;

	
		if (bulletsPrev < bullets) {
			
			audio.play("launch", object->pos, 100, drawSys.cam);
			//sounds.push_back({ "launch", {0, 0} });
		}
		
		
		if (object->activeAbility != objectPrev->activeAbility)
			audio.play("bip", object->pos, 100, drawSys.cam);
		if (object->effects[Bonus::LASER] && !objectPrev->effects[Bonus::LASER])
			audio.play("laser", object->pos, 100, drawSys.cam);
	}
	
}