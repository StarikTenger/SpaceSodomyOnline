#include "Player.h"

bool operator<(const Player& a, const Player& b) {
	if(a.kills == b.kills)
		return a.deaths > b.deaths;
	return a.kills < b.kills;
}
