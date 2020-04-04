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

		int bulletsPrev = 0;
		for (const auto& o : sysPrev.objects)
			if (o.type == Object::BULLET && o.id == id && geom::distance(o.pos, objectPrev->pos) < 1)
				bulletsPrev++;
		int bullets = 0;
		for (const auto& o : sys.objects)
			if (o.type == Object::BULLET && o.id == id && geom::distance(o.pos, object->pos) < 1)
				bullets++;

		if (object && objectPrev && bulletsPrev < bullets) {
			std::cout << " s";
			audio.play("launch", object->pos, 100, drawSys.cam);
			//sounds.push_back({ "launch", {0, 0} });
		}
		if(object && objectPrev && object->hp < objectPrev->hp)
			audio.play("death", object->pos, 100, drawSys.cam);
	}
	
}