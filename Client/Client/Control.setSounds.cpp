#include "Control.h"
#include <iostream>

void Control::setSounds() {
	for (auto& object : sys.objects)
		if (object.type == Object::SHIP)
			sys.players[object.id].object = &object;

	for (auto& object : sysPrev.objects)
		if (object.type == Object::SHIP)
			sysPrev.players[object.id].object = &object;

	// Signals
	if (sys.mainPlayer.hp < sysPrev.mainPlayer.hp) {
		audio.play("damage", sys.mainPlayer.pos, 100, drawSys.cam);
		std::cout << "d";
	}

	//std::cout << " Q";
	if (!sysPrev.players.size())
		return;
	for (auto& p : sys.players) {
		int id = p.first;
		Player& player = p.second;
		Player& playerPrev = sysPrev.players[id];
		

		Object* object = p.second.object;
		Object* objectPrev = playerPrev.object;

		// Death
		if (objectPrev && player.deaths > playerPrev.deaths) {
			audio.play("death", objectPrev->pos, 100, drawSys.cam);

			drawSys.animation("shipAura",
				AnimationState(objectPrev->pos, Vec2(objectPrev->r * 2, objectPrev->r * 2), objectPrev->dir, { 255, 255, 255 }),
				AnimationState(objectPrev->pos + geom::direction(random::floatRandom(0, M_PI * 2, 2)) * 0.05, Vec2(objectPrev->r * 2, objectPrev->r * 2) * 1.5, objectPrev->dir, { 255, 255, 255, 0 }),
				0.2);
		}

		if (!objectPrev || !object || geom::distance({ 0, 0 }, object->pos) > 1e5) // smth went wrong
			continue;

		// Wall kick
		if (geom::distance(object->vel, objectPrev->vel) > 0.5) {
			audio.play("knock", object->pos, 100 * geom::distance(object->vel, objectPrev->vel), drawSys.cam);
		}

		// Modules

		// Impulse
		for (int i = 0; i < 2; i++) {
			if (player.modulesType[i] == 3 && player.modulesCooldown[i] > playerPrev.modulesCooldown[i]) {
				for (int j = 0; j < 10; j++) {
					auto col1 = objectPrev->color;
					col1.a = 0;
					drawSys.animation("particleFlame",
						AnimationState(objectPrev->pos, Vec2(objectPrev->r, objectPrev->r), objectPrev->dir, objectPrev->color),
						AnimationState(objectPrev->pos - geom::direction(objectPrev->dir + random::floatRandom(-1, 1, 2) * 0.1) * 2, Vec2(objectPrev->r, objectPrev->r), objectPrev->dir, col1),
						0.5);
				}
				audio.play("impulse", objectPrev->pos, 100, drawSys.cam);
			}
		}

		// Blink
		for (int i = 0; i < 2; i++) {
			if (player.modulesType[i] == 7 && player.modulesCooldown[i] > playerPrev.modulesCooldown[i]) {
				auto col1 = objectPrev->color;
				col1.a = 0;
				drawSys.animation("shipAura",
					AnimationState(objectPrev->pos, Vec2(objectPrev->r, objectPrev->r) * 2, objectPrev->dir, objectPrev->color),
					AnimationState(objectPrev->pos + objectPrev->vel * 0.1, Vec2(objectPrev->r, objectPrev->r) * 2, objectPrev->dir, col1),
					0.5);
				drawSys.animation("shipAura",
					AnimationState(object->pos, Vec2(object->r, object->r) * 2, object->dir, object->color),
					AnimationState(object->pos, Vec2(object->r, object->r) * 4, object->dir, col1),
					0.1);
				audio.play("blink", objectPrev->pos, 100, drawSys.cam);
			}
		}

		// Splash
		for (int i = 0; i < 2; i++) {
			if (player.modulesType[i] == 5 && player.modulesCooldown[i] > playerPrev.modulesCooldown[i]) {
				auto col1 = Color(255, 255, 255);
				col1.a = 0;
				drawSys.animation("sector",
					AnimationState(objectPrev->pos, Vec2(objectPrev->r, objectPrev->r) * 2 * 0, objectPrev->dir, {255, 255, 255}),
					AnimationState(objectPrev->pos + objectPrev->vel * 0.1, Vec2(objectPrev->r, objectPrev->r) * 2 * 10, objectPrev->dir, col1),
					0.2);
				audio.play("splash", objectPrev->pos, 100, drawSys.cam);
			}
		}
		

		// Engine
		// On
		if(0) {
			if (object->orders[Object::MOVE_FORWARD] && !objectPrev->orders[Object::MOVE_FORWARD])
				audio.play("engineOn", object->pos, 100, drawSys.cam);
			if (object->orders[Object::MOVE_BACKWARD] && !objectPrev->orders[Object::MOVE_BACKWARD])
				audio.play("engineOn", object->pos, 100, drawSys.cam);
			if (object->orders[Object::MOVE_LEFT] && !objectPrev->orders[Object::MOVE_LEFT])
				audio.play("engineOn", object->pos, 100, drawSys.cam);
			if (object->orders[Object::MOVE_RIGHT] && !objectPrev->orders[Object::MOVE_RIGHT])
				audio.play("engineOn", object->pos, 100, drawSys.cam);
			// Off
			if (!object->orders[Object::MOVE_FORWARD] && objectPrev->orders[Object::MOVE_FORWARD])
				audio.play("engineOff", object->pos, 100, drawSys.cam);
			if (!object->orders[Object::MOVE_BACKWARD] && objectPrev->orders[Object::MOVE_BACKWARD])
				audio.play("engineOff", object->pos, 100, drawSys.cam);
			if (!object->orders[Object::MOVE_LEFT] && objectPrev->orders[Object::MOVE_LEFT])
				audio.play("engineOff", object->pos, 100, drawSys.cam);
			if (!object->orders[Object::MOVE_RIGHT] && objectPrev->orders[Object::MOVE_RIGHT])
				audio.play("engineOff", object->pos, 100, drawSys.cam);
		}

		// Damage
		if (object && objectPrev && object->hp < objectPrev->hp) {
			std::cout << "knock\n";
			audio.play("knock", object->pos, 100, drawSys.cam);
		}

		// Counting bullets
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
	
	for (auto& object : sys.objects) {
		if (object.type == Object::EXPLOSION) {
			int con = 0;
			for (auto& objectPrev : sysPrev.objects)
				if (objectPrev.type == Object::EXPLOSION && geom::distance(object.pos, objectPrev.pos) < 1) {
					con = 1;
					break;
				}
			if (con)
				continue;

			audio.play("explosion", object.pos, 30, drawSys.cam);

		}
	}
}