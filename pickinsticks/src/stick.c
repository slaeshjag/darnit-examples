#include "sticks.h"


void stickMove() {
	s.stick.x = (rand() % s.map.handle->layer->tilemap->w) * s.map.handle->layer->tile_w;
	s.stick.y = (rand() % s.map.handle->layer->tilemap->h) * s.map.handle->layer->tile_h;
	darnitRenderTileSet(s.stick.tile, 0, s.stick.ts, rand() % 3);
	darnitRenderTileMove(s.stick.tile, 0, s.stick.ts, s.stick.x, s.stick.y);
	s.stick.count++;

	return;
}


int stickInit() {
	if ((s.stick.tile = darnitRenderTileAlloc(1)) == NULL)
		return -1;

	if ((s.stick.ts = darnitRenderTilesheetLoad("gfx/sticks.png", 32, 32, DARNIT_PFORMAT_RGB5A1)) == NULL) {
		fprintf(stderr, "Unable to open gfx/sticks.png\n");
		return -1;
	}

	srand(time(NULL));
	stickMove();

	s.stick.count = 0;

	return 0;
}


void stickLoop() {
	darnitRenderTileDraw(s.stick.tile, s.stick.ts, 1);
	if (s.player.x >= s.stick.x + 32 || s.player.x + 32 <= s.stick.x)
		return;
	if (s.player.y >= s.stick.y + 32 || s.player.y + 32 <= s.stick.y)
		return;
	
	stickMove();
	
	return;
}
