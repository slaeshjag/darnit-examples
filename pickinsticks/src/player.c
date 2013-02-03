#include "sticks.h"


int playerInit() {
	if ((s.player.sprite = d_sprite_load("gfx/baurn.spr", 
			0, DARNIT_PFORMAT_RGB5A1)) == NULL) {
		return -1;
	}
	
	s.player.x = 384;
	s.player.y = 224;

	return 0;
}


void playerCamera() {
	d_map_camera_move(s.map.handle, s.player.x - 384, s.player.y - 216);
	d_render_offset(s.player.x - 384, s.player.y - 216);

	return;
}


void playerDraw() {
	int x, y, dir;
	
	dir = -2;

	if (s.keys.left) {
		s.player.x -= (int)(PLAYER_SPEED * d_last_frame_time());
		dir = 2;
	} if (s.keys.right) {
		s.player.x += PLAYER_SPEED * d_last_frame_time();
		dir = 3;
	} if (s.keys.up) {
		s.player.y -= (int)(PLAYER_SPEED * d_last_frame_time());
		dir = 1;
	} if (s.keys.down) {
		s.player.y += PLAYER_SPEED * d_last_frame_time();
		dir = 0;
	}

	if (dir == s.player.dir)
		dir = -1;
	else
		s.player.dir = dir;
	x = s.player.x;
	y = s.player.y;

	if (x > MAP_W - 32) 
		s.player.x = MAP_W - 32;
	if (x < 0)
		s.player.x = 0;
	if (y > MAP_H - 32)
		s.player.y = MAP_H - 32;
	if (y < 0)
		s.player.y = 0;

	if (dir >= 0) {
		d_sprite_animate_start(s.player.sprite);
		d_sprite_direction_set(s.player.sprite, dir);
	} else if (dir < -1)
		d_sprite_animate_stop(s.player.sprite);


	d_sprite_move(s.player.sprite, s.player.x, s.player.y - 16);
	d_sprite_draw(s.player.sprite);

	return;
}
