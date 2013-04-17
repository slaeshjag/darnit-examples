#include "pong.h"

void player_init() {
	p.player.x = 16;
	p.player.y = (480 - 32) / 2 * 1000;

	return;
}


void player_loop() {
	DARNIT_KEYS keys;

	keys = d_keys_get();
	if (keys.up)
		p.player.y -= d_last_frame_time() * BAT_SPEED;
	if (keys.down)
		p.player.y += d_last_frame_time() * BAT_SPEED;
	
	if (p.player.y < 0)
		p.player.y = 0;
	else if (p.player.y > (480 - 64) * 1000)
		p.player.y = (480 - 64) * 1000;

	return;
}
