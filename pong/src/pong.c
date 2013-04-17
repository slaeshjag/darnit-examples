#include "pong.h"


int init() {

	if (!(p.player_ts = d_render_tilesheet_load("bat.png", 16, 64, DARNIT_PFORMAT_RGB5A1)))
		return 0;
	if (!(p.ball_ts = d_render_tilesheet_load("ball.png", 32, 32, DARNIT_PFORMAT_RGB5A1)))
		return 0;

	player_init();

	return 1;
}


int main(int argc, char **argv) {
	d_init("pong", "Pong!", NULL);

	if (!init()) {
		fprintf(stderr, "libDarnit didn't init\n");
		return -1;
	}

	d_render_clearcolor_set(0, 0, 192);

	for (;;) {
		player_loop();
		ball_loop();

		d_render_begin();

		d_render_tile_blit(p.player_ts, 0, p.player.x, p.player.y / 1000);
		d_render_end();
		d_loop();
	}

	d_quit();

	return 0;
}
