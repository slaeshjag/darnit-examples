#include "sticks.h"


int playerInit() {
	if ((s.player.sprite = darnitSpriteLoad("gfx/baurn.spr", 
			0, DARNIT_PFORMAT_RGB5A1)) == NULL) {
		return -1;
	}
	
	s.player.x = 384;
	s.player.y = 224;

	return 0;
}


void playerCamera() {
	darnitMapCameraMove(s.map.handle, s.player.x - 384, s.player.y - 216);
	darnitRenderOffset(s.player.x - 384, s.player.y - 216);

	return;
}


void playerDraw() {
	int x, y, dir;
	
	dir = -2;

	if (s.keys.left) {
		s.player.x -= (int)(PLAYER_SPEED * darnitTimeLastFrameTook());
		dir = 2;
	} if (s.keys.right) {
		s.player.x += PLAYER_SPEED * darnitTimeLastFrameTook();
		dir = 3;
	} if (s.keys.up) {
		s.player.y -= (int)(PLAYER_SPEED * darnitTimeLastFrameTook());
		dir = 1;
	} if (s.keys.down) {
		s.player.y += PLAYER_SPEED * darnitTimeLastFrameTook();
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
		darnitSpriteAnimationEnable(s.player.sprite);
		darnitSpriteDirectionChange(s.player.sprite, dir);
	} else if (dir < -1)
		darnitSpriteAnimationDisable(s.player.sprite);


	darnitSpriteMove(s.player.sprite, s.player.x, s.player.y - 16);
	darnitSpriteDraw(s.player.sprite);

	return;
}
