#include "sticks.h"


int init() {
	d_fs_mount("gfx.ldi");
	
	if (playerInit() == -1);
	else if (mapInit() == -1);
	else if (stickInit() == -1);
	else if (textInit() == -1);
	else return 0;

	return -1;
}


int main(int argc, char **argv) {
	if (d_init("darnitSticks!", "sticks", NULL) == NULL) {
		fprintf(stderr, "libDarnit failed to init. Lets just give up...\n");
		return -1;
	}

	if (init() == -1)
		d_quit();

	for (;;) {
		s.keys = d_keys_get();
		d_render_begin();
		
		playerCamera();
		mapDraw();
		d_render_blend_enable();
		stickLoop();
		playerDraw();
		d_render_offset(0, 0);
		textLoop();
		d_render_blend_disable();

		d_render_end();
		d_loop();
	}

	return 0;
}
